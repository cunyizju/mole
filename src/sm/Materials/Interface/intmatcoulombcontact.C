// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "intmatcoulombcontact.h"
#include "math/gausspoint.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/mathfem.h"
#include "utility/contextioerr.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_Material(IntMatCoulombContact);

IntMatCoulombContact :: IntMatCoulombContact(int n, Domain *d) : StructuralInterfaceMaterial(n, d) { }


FloatArrayF<3>
IntMatCoulombContact :: giveEngTraction_3d(const FloatArrayF<3> &jump, GaussPoint *gp, TimeStep *tStep) const
{
    IntMatCoulombContactStatus *status = static_cast< IntMatCoulombContactStatus * >( this->giveStatus( gp ) );
    auto tempShearStressShift = status->giveShearStressShift();

    double normalJump = jump[0];
    FloatArrayF<2> shearJump{ jump[1], jump[2] };

    double maxShearStress = 0.0;
    double shift = -this->kn * this->stiffCoeff * normalClearance;

    double normalStress;
    if ( normalJump + normalClearance <= 0. ) {
        normalStress = this->kn * ( normalJump + normalClearance ) + shift; //in compression and after the clearance gap closed
        maxShearStress = fabs( normalStress ) * this->frictCoeff;
    } else {
        normalStress = this->kn * this->stiffCoeff * ( normalJump + normalClearance ) + shift;
        maxShearStress = 0.;
    }
    auto shearStress = this->kn * shearJump - tempShearStressShift;
    double dp = norm(shearStress);
    double eps = 1.0e-15; // small number
    if ( dp > maxShearStress ) {
        shearStress *= maxShearStress / ( dp + eps );
    }
    tempShearStressShift = this->kn * shearJump - shearStress;

    // Set stress components in the traction vector
    FloatArrayF<3> answer = {normalStress, shearStress[0], shearStress[1]};

    // Update gp
    status->setTempShearStressShift( tempShearStressShift );
    status->letTempJumpBe( jump );
    status->letTempTractionBe( answer );
    return answer;
}


FloatMatrixF<3,3>
IntMatCoulombContact :: give3dStiffnessMatrix_Eng(MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) const
{
    IntMatCoulombContactStatus *status = static_cast< IntMatCoulombContactStatus * > ( this->giveStatus(gp) );
    const auto &jump = status->giveTempJump();
    double normalJump = jump[0]; //in status, normal is always the first component

    auto answer = eye<3>();
    if ( rMode == SecantStiffness || rMode == TangentStiffness ) {
        if ( normalJump + normalClearance <= 0 ) {
            answer *= this->kn; //in compression and after the clearance gap closed
        } else {
            answer *= this->kn * this->stiffCoeff;
        }
    } else {
        answer *= this->kn;
    }
    return answer;
}


void
IntMatCoulombContact :: initializeFrom(InputRecord &ir)
{
    StructuralInterfaceMaterial :: initializeFrom( ir );

    frictCoeff = 0.;
    stiffCoeff = 0.;
    normalClearance = 0.;
    IR_GIVE_FIELD(ir, kn, _IFT_IntMatCoulombContact_kn);
    IR_GIVE_OPTIONAL_FIELD(ir, frictCoeff, _IFT_IntMatCoulombContact_frictCoeff);
    IR_GIVE_OPTIONAL_FIELD(ir, stiffCoeff, _IFT_IntMatCoulombContact_stiffCoeff);
    IR_GIVE_OPTIONAL_FIELD(ir, normalClearance, _IFT_IntMatCoulombContact_normalClearance);
}


void
IntMatCoulombContact :: giveInputRecord(DynamicInputRecord &input)
{
    StructuralInterfaceMaterial :: giveInputRecord(input);
    input.setField(this->kn, _IFT_IntMatCoulombContact_kn);
    input.setField(this->frictCoeff, _IFT_IntMatCoulombContact_frictCoeff);
    input.setField(this->stiffCoeff, _IFT_IntMatCoulombContact_stiffCoeff);
    input.setField(this->normalClearance, _IFT_IntMatCoulombContact_normalClearance);
}


IntMatCoulombContactStatus :: IntMatCoulombContactStatus(GaussPoint *g) : StructuralInterfaceMaterialStatus(g)
{}


void
IntMatCoulombContactStatus :: printOutputAt(FILE *file, TimeStep *tStep) const
{
    StructuralInterfaceMaterialStatus::printOutputAt( file, tStep );
    fprintf(file, "status { ");
    fprintf( file, "shearStressShift (%f, %f)", this->shearStressShift.at( 1 ), this->shearStressShift.at( 2 ) );
    fprintf(file, "}\n");
}


void
IntMatCoulombContactStatus :: initTempStatus()
{
    StructuralInterfaceMaterialStatus::initTempStatus( );
    tempShearStressShift = shearStressShift;
}


void
IntMatCoulombContactStatus :: updateYourself(TimeStep *tStep)
{
    StructuralInterfaceMaterialStatus::updateYourself( tStep );
    shearStressShift = tempShearStressShift;
}


void
IntMatCoulombContactStatus :: saveContext(DataStream &stream, ContextMode mode)
{
    StructuralInterfaceMaterialStatus::saveContext( stream, mode );

    //if ( !stream.write(kappa) ) {
    //THROW_CIOERR(CIO_IOERR);
    //}
}


void
IntMatCoulombContactStatus :: restoreContext(DataStream &stream, ContextMode mode)
{
    StructuralInterfaceMaterialStatus::restoreContext( stream, mode );

    //if ( !stream.read(kappa) ) {
    //THROW_CIOERR(CIO_IOERR);
    //}
}
} // end namespace oofem
