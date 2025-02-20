// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "interfaceelement1d.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "fei/feinterpol.h"
#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "engng/classfactory.h"
#include "math/floatmatrixf.h"

namespace oofem {
REGISTER_Element(InterfaceElem1d);

InterfaceElem1d :: InterfaceElem1d(int n, Domain *aDomain) :
    StructuralElement(n, aDomain)
{
    numberOfDofMans = 2;
    referenceNode = 0;
    normal.resize(3);
    normal.zero();
}


void
InterfaceElem1d :: setCoordMode()
{
    switch ( domain->giveNumberOfSpatialDimensions() ) {
    case 1:
        this->mode = ie1d_1d;
        break;
    case 2:
        this->mode = ie1d_2d;
        break;
    case 3:
        this->mode = ie1d_3d;
        break;
    default:
        OOFEM_ERROR("Unsupported domain type")
    }
}


void
InterfaceElem1d :: computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    setCoordMode();
    switch ( mode ) {
        case ie1d_1d: answer = FloatArray{static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->giveEngTraction_1d(strain.at(1), gp, tStep)}; return;
        case ie1d_2d: answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->giveEngTraction_2d(strain, gp, tStep); return;
        case ie1d_3d: answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->giveEngTraction_3d(strain, gp, tStep); return;
    }
}


void
InterfaceElem1d :: computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    setCoordMode();
    switch ( mode ) {
    case ie1d_1d: answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->give1dStiffnessMatrix_Eng(rMode, gp, tStep); return;
    case ie1d_2d: answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->give2dStiffnessMatrix_Eng(rMode, gp, tStep); return;
    case ie1d_3d: answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->give3dStiffnessMatrix_Eng(rMode, gp, tStep); return;
    }
}



MaterialMode
InterfaceElem1d :: giveMaterialMode()
{
    setCoordMode();
    switch ( mode ) {
    case ie1d_1d: return _1dInterface;

    case ie1d_2d: return _2dInterface;

    case ie1d_3d: return _3dInterface;

    default: OOFEM_ERROR("Unsupported coord mode");
    }
    return _1dInterface; // to make the compiler happy
}


void
InterfaceElem1d :: computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep)
// Returns the lumped mass matrix (which is zero matrix) of the receiver. This expression is
// valid in both local and global axes.
{
    int ndofs = this->computeNumberOfDofs();
    answer.resize(ndofs, ndofs);
    answer.zero();
}


void
InterfaceElem1d :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
//
// Returns linear part of geometrical equations of the receiver at gp.
// Returns the linear part of the B matrix
//
{
    setCoordMode();
    //double area = this->giveCrossSection()->give(CS_Area);
    double area = 1.;
    this->computeLocalSlipDir(normal);
    FloatMatrix bLoc, lcs;
    evaluateLocalCoordinateSystem(lcs);
    switch ( mode ) {
    case ie1d_1d:
        bLoc.resize(1, 2);
        bLoc.at(1, 1) = -1.0;
        bLoc.at(1, 2) = +1.0;
        break;
    case ie1d_2d:
        bLoc.resize(2, 4);
        bLoc.zero();
        bLoc.at(1, 1) = -1.0;
        bLoc.at(1, 3) = +1.0;
        bLoc.at(2, 2) = -1.0;
        bLoc.at(2, 4) = +1.0;
        break;
    case ie1d_3d:
        bLoc.resize(3, 6);
        bLoc.zero();
        bLoc.at(1, 1) = -1.0;
        bLoc.at(1, 4) = +1.0;
        bLoc.at(2, 2) = -1.0;
        bLoc.at(2, 5) = +1.0;
        bLoc.at(3, 3) = -1.0;
        bLoc.at(3, 6) = +1.0;
        break;
    default:
        OOFEM_ERROR("unsupported mode");
    }

    bLoc.times(area);
    answer.beProductOf(lcs, bLoc);
}


void
InterfaceElem1d :: evaluateLocalCoordinateSystem(FloatMatrix &lcs)
//
// Computes unit vectors of local coordinate system, stored by rows.
//
{
    setCoordMode();
    switch ( mode ) {
    case ie1d_1d:
        lcs.resize(1, 1);
        lcs.at(1, 1) = 1.;
        return;

    case ie1d_2d:
        lcs.resize(2, 2);
        lcs.at(1, 1) =  normal.at(1);
        lcs.at(1, 2) =  normal.at(2);
        lcs.at(2, 1) = -normal.at(2);
        lcs.at(2, 2) =  normal.at(1);
        return;

    case ie1d_3d:
    {
        FloatArray ly(3), lz(3);
        normal.normalize();
        ly.zero();
        if ( fabs( normal.at(1) ) > fabs( normal.at(2) ) ) {
            ly.at(2) = 1.;
        } else {
            ly.at(1) = 1.;
        }

        lz.beVectorProductOf(normal, ly);
        lz.normalize();
        ly.beVectorProductOf(lz, normal);
        ly.normalize();

        lcs.resize(3, 3);
        int i;
        for ( i = 1; i <= 3; i++ ) {
            lcs.at(1, i) = normal.at(i);
            lcs.at(2, i) = ly.at(i);
            lcs.at(3, i) = lz.at(i);
        }

        return;
    }

    default:
        OOFEM_ERROR("unsupported mode");
    }
}


void
InterfaceElem1d :: computeGaussPoints()
// Sets up the array of Gauss Points of the receiver.
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ] = std::make_unique<GaussIntegrationRule>(1, this, 1, 2);
        integrationRulesArray [ 0 ]->SetUpPointsOnLine(1, this->giveMaterialMode() );
    }
}


int
InterfaceElem1d :: computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords)
{
    answer = this->giveNode(1)->giveCoordinates();

    return 1;
}


bool
InterfaceElem1d :: computeLocalCoordinates(FloatArray &answer, const FloatArray &gcoords)
{
    OOFEM_ERROR("Not implemented");
    return false;
}

double
InterfaceElem1d :: computeVolumeAround(GaussPoint *gp)
// Returns the length of the receiver. This method is valid only if 1
// Gauss point is used.
{
    return 1.0; //MUST be set to 1.0
}


void
InterfaceElem1d :: initializeFrom(InputRecord &ir)
{
    StructuralElement :: initializeFrom(ir);

    IR_GIVE_OPTIONAL_FIELD(ir, referenceNode, _IFT_InterfaceElem1d_refnode);
    IR_GIVE_OPTIONAL_FIELD(ir, normal, _IFT_InterfaceElem1d_normal);
    if ( referenceNode == 0 && normal.at(1) == 0 && normal.at(2) == 0 && normal.at(1) == 0 && normal.at(3) == 0 ) {
        OOFEM_ERROR("wrong reference node or normal specified");
    }
    if ( ir.hasField(_IFT_InterfaceElem1d_dofIDs) ) {
        IR_GIVE_FIELD(ir, dofids, _IFT_InterfaceElem1d_refnode);
    } else {
        switch ( domain->giveNumberOfSpatialDimensions() ) {
        case 1:
            this->dofids = IntArray{D_u};
            break;
        case 2:
            this->dofids = {D_u, D_v};
            break;
        case 3:
            this->dofids = {D_u, D_v, D_w};
            break;
        default:
            OOFEM_ERROR("Unsupported domain type")
        }
    }

    this->computeLocalSlipDir(normal); ///@todo Move into postInitialize ?
}


int
InterfaceElem1d :: computeNumberOfDofs()
{
    setCoordMode();
    switch ( mode ) {
    case ie1d_1d:
        return 2;

    case ie1d_2d:
        return 4;

    case ie1d_3d:
        return 6;

    default:
        OOFEM_ERROR("unsupported mode");
    }

    return 0; // to suppress compiler warning
}


void
InterfaceElem1d :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = this->dofids;
}


void
InterfaceElem1d :: computeLocalSlipDir(FloatArray &normal)
{
    normal.resizeWithValues(3);
    if ( this->referenceNode ) {
        // normal
        normal.at(1) = domain->giveNode(this->referenceNode)->giveCoordinate(1) - this->giveNode(1)->giveCoordinate(1);
        normal.at(2) = domain->giveNode(this->referenceNode)->giveCoordinate(2) - this->giveNode(1)->giveCoordinate(2);
        normal.at(3) = domain->giveNode(this->referenceNode)->giveCoordinate(3) - this->giveNode(1)->giveCoordinate(3);
    } else {
        if ( normal.at(1) == 0 && normal.at(2) == 0 && normal.at(3) == 0 ) {
            OOFEM_ERROR("normal is not defined (referenceNode=0,normal=(0,0,0))");
        }
    }

    normal.normalize();
}

} // end namespace oofem
