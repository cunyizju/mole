// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include <algorithm>

#include "isointerfacedamage02.h"
#include "math/gausspoint.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/mathfem.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_Material(IsoInterfaceDamageMaterial_2);

IsoInterfaceDamageMaterial_2 :: IsoInterfaceDamageMaterial_2(int n, Domain *d) : StructuralInterfaceMaterial(n, d)
{}


FloatArrayF<3>
IsoInterfaceDamageMaterial_2 :: giveEngTraction_3d(const FloatArrayF<3> &jump, GaussPoint *gp, TimeStep *tStep) const
{
    IsoInterfaceDamageMaterialStatus_2 *status = static_cast< IsoInterfaceDamageMaterialStatus_2 * >( this->giveStatus(gp) );

    // compute equivalent strain
    double equivStrain = macbra( jump.at(1) );

    // compute value of loading function if strainLevel crit apply
    double f = equivStrain - status->giveKappa();

    double tempKappa = 0.0, omega = 0.0;
    if ( f <= 0.0 ) {
        // damage do not grow
        tempKappa = status->giveKappa();
        omega     = status->giveDamage();
    } else {
        // damage grows
        tempKappa = equivStrain;
        // evaluate damage parameter
        omega = this->computeDamageParam(tempKappa, jump, gp);
    }

    auto de = this->give3dStiffnessMatrix_Eng(ElasticStiffness, gp, tStep);
    auto answer = dot(de, jump);
    // damage in tension only
    if ( equivStrain >= 0.0 ) {
        answer *= 1.0 - omega;
    }

    // update gp
    status->letTempJumpBe(jump);
    status->letTempTractionBe(answer);
    status->setTempKappa(tempKappa);
    status->setTempDamage(omega);

    return answer;
}


FloatMatrixF<3,3>
IsoInterfaceDamageMaterial_2 :: give3dStiffnessMatrix_Eng(MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) const
{
    IsoInterfaceDamageMaterialStatus_2 *status = static_cast< IsoInterfaceDamageMaterialStatus_2 * >( this->giveStatus(gp) );

    // assemble eleastic stiffness
    auto answer = diag<3>({kn, ks, ks});

    if ( rMode == ElasticStiffness ) {
        return answer;
    }

    double om = min(status->giveTempDamage(), maxOmega);
    double un = status->giveTempJump().at(1);
    if ( rMode == SecantStiffness ) {
        // damage in tension only
        if ( un >= 0 ) {
            answer *= 1.0 - om;
        }

        return answer;
    } else { // Tangent Stiffness
        // damage in tension only
        if ( un >= 0 ) {
            answer *= 1.0 - om;
#if 0
            // Unreachable code - commented out to supress compiler warnings
            double dom = -( -e0 / un / un * exp( -( ft / gf ) * ( un - e0 ) ) + e0 / un * exp( -( ft / gf ) * ( un - e0 ) ) * ( -( ft / gf ) ) );
            if ( ( om > 0. ) && ( status->giveTempKappa() > status->giveKappa() ) ) {
                const auto &e = status->giveTempJump();
                auto se = dot(answer, e);
                answer.at(1, 1) -= se.at(1) * dom;
                answer.at(2, 1) -= se.at(2) * dom;
            }
#endif
        }
        return answer;
    }
}


int
IsoInterfaceDamageMaterial_2 :: giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep)
{
    IsoInterfaceDamageMaterialStatus_2 *status = static_cast< IsoInterfaceDamageMaterialStatus_2 * >( this->giveStatus(gp) );
    if ( type == IST_DamageTensor ) {
        answer.resize(6);
        answer.zero();
        answer.at(1) = answer.at(2) = answer.at(3) = status->giveDamage();
        return 1;
    } else if ( type == IST_DamageTensorTemp ) {
        answer.resize(6);
        answer.zero();
        answer.at(1) = answer.at(2) = answer.at(3) = status->giveTempDamage();
        return 1;
    } else if ( type == IST_PrincipalDamageTensor ) {
        answer.resize(3);
        answer.at(1) = answer.at(2) = answer.at(3) = status->giveDamage();
        return 1;
    } else if ( type == IST_PrincipalDamageTempTensor ) {
        answer.resize(3);
        answer.at(1) = answer.at(2) = answer.at(3) = status->giveTempDamage();
        return 1;
    } else if ( type == IST_MaxEquivalentStrainLevel ) {
        answer.resize(1);
        answer.at(1) = status->giveKappa();
        return 1;
    } else {
        return StructuralInterfaceMaterial :: giveIPValue(answer, gp, type, tStep);
    }
}


void
IsoInterfaceDamageMaterial_2 :: initializeFrom(InputRecord &ir)
{
    StructuralInterfaceMaterial :: initializeFrom(ir);

    std :: ifstream is;
    int nbrOfLinesToRead;

    IR_GIVE_FIELD(ir, kn, _IFT_IsoInterfaceDamageMaterial_2_kn);
    IR_GIVE_FIELD(ir, ks, _IFT_IsoInterfaceDamageMaterial_2_ks);
    IR_GIVE_FIELD(ir, ft, _IFT_IsoInterfaceDamageMaterial_2_ft);

    this->e0 = ft / kn;

    //Set limit on the maximum isotropic damage parameter if needed
    IR_GIVE_OPTIONAL_FIELD(ir, maxOmega, _IFT_IsoInterfaceDamageMaterial_2_maxOmega);
    maxOmega = min(maxOmega, 0.999999);
    maxOmega = max(maxOmega, 0.0);

    // Parse the table file
    IR_GIVE_FIELD(ir, tablename, _IFT_IsoInterfaceDamageMaterial_2_tablename);

    is.open(tablename.c_str(), std :: ifstream :: in);
    if ( !is.is_open() ) {
        OOFEM_ERROR("Can't open table file %s.", tablename.c_str() );
    }

    // Read first line
    if ( is >> nbrOfLinesToRead ) {
        printf("NumberofLinestoRead: %d\n", nbrOfLinesToRead);
    } else {
        OOFEM_ERROR("Error reading table file, first line should be "
                    "an integer stating how many strain damage pairs that exist in the file.");
    }

    damages.resize(nbrOfLinesToRead + 1);
    strains.resize(nbrOfLinesToRead + 1);

    // Insert a (0,0) pair
    strains(0) = damages(0) = 0.0;

    for ( int i = 0; i < nbrOfLinesToRead; i++ ) {
        if ( !( is >> strains(i + 1) >> damages(i + 1) ) ) {
            OOFEM_ERROR("Error reading table file at line %d, expected a "
                         "strain damage pair.", i + 2);
        }

        if ( ( damages(i + 1) < damages(i) ) || ( strains(i + 1) < strains(i) ) ) {
            OOFEM_ERROR("Error reading table file at line %d, strain "
                         "and damage must be given in an increasing order and be positive.", i + 2);
        }
    }

    // We add e0 to the strains since strains should be given as the increase in
    // strain relative to e0.
    strains.add(e0);
}


void
IsoInterfaceDamageMaterial_2 :: giveInputRecord(DynamicInputRecord &input)
{
    StructuralInterfaceMaterial :: giveInputRecord(input);

    input.setField(this->kn, _IFT_IsoInterfaceDamageMaterial_2_kn);
    input.setField(this->ks, _IFT_IsoInterfaceDamageMaterial_2_ks);
    input.setField(this->ft, _IFT_IsoInterfaceDamageMaterial_2_ft);
    input.setField(this->tablename, _IFT_IsoInterfaceDamageMaterial_2_tablename);
    input.setField(this->maxOmega, _IFT_IsoInterfaceDamageMaterial_2_maxOmega);
}


double
IsoInterfaceDamageMaterial_2 :: computeEquivalentStrain(const FloatArrayF<3> &strain, GaussPoint *gp, TimeStep *tStep) const
{
    return macbra( strain.at(1) );
}

double
IsoInterfaceDamageMaterial_2 :: computeDamageParam(double kappa, const FloatArrayF<3> &strain, GaussPoint *gp) const
{
    if ( kappa > this->e0 ) {
        // Linear interpolation between table values.

        // If out of bounds damage is set to the last given damage value in the table
        if ( kappa >= strains.at( strains.giveSize() ) ) {
            return damages.at( damages.giveSize() );
        } else {
            // std::lower_bound uses binary search to find index with value bounding kappa from above
            int index = (int)(std :: lower_bound(strains.givePointer(), strains.givePointer() + strains.giveSize(), kappa) - strains.givePointer());

#if 0
            printf("e0 %lf\n", e0);
            printf( "sizeof %d\n", sizeof( strains.givePointer() ) );
            printf("pointer: %d\n", index);
            printf("value: %lf\n", * index);
            printf( "Index found: %d\n", index - strains.givePointer() );
            printf( "First index: %d\n", strains.givePointer() );
            printf( "Last index: %d\n", strains.givePointer() + strains.giveSize() );
#endif

            // Pointer arithmetic to find the values used in interpolation
            double x0 = strains(index - 1);
            double x1 = strains(index );
            double y0 = damages(index - 1);
            double y1 = damages(index );

            // Interpolation formula
            return y0 + ( y1 - y0 ) * ( kappa - x0 ) / ( x1 - x0 );
        }
    } else {
        return 0.0;
    }
}


IsoInterfaceDamageMaterialStatus_2 :: IsoInterfaceDamageMaterialStatus_2(GaussPoint *g) : 
    StructuralInterfaceMaterialStatus(g)
{
    kappa = tempKappa = 0.0;
    damage = tempDamage = 0.0;
}


void
IsoInterfaceDamageMaterialStatus_2 :: printOutputAt(FILE *file, TimeStep *tStep) const
{
    StructuralInterfaceMaterialStatus :: printOutputAt(file, tStep);
    fprintf(file, "status { ");
    if ( this->damage > 0.0 ) {
        fprintf(file, "kappa %f, damage %f ", this->kappa, this->damage);
    }

    fprintf(file, "}\n");
}


void
IsoInterfaceDamageMaterialStatus_2 :: initTempStatus()
{
    StructuralInterfaceMaterialStatus :: initTempStatus();
    this->tempKappa = this->kappa;
    this->tempDamage = this->damage;
}

void
IsoInterfaceDamageMaterialStatus_2 :: updateYourself(TimeStep *tStep)
{
    StructuralInterfaceMaterialStatus :: updateYourself(tStep);
    this->kappa = this->tempKappa;
    this->damage = this->tempDamage;
}


void
IsoInterfaceDamageMaterialStatus_2 :: saveContext(DataStream &stream, ContextMode mode)
{
    StructuralInterfaceMaterialStatus :: saveContext(stream, mode);

    if ( !stream.write(kappa) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.write(damage) ) {
        THROW_CIOERR(CIO_IOERR);
    }
}

void
IsoInterfaceDamageMaterialStatus_2 :: restoreContext(DataStream &stream, ContextMode mode)
{
    StructuralInterfaceMaterialStatus :: restoreContext(stream, mode);

    if ( !stream.read(kappa) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.read(damage) ) {
        THROW_CIOERR(CIO_IOERR);
    }
}
} // end namespace oofem
