// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/Interfaces/intelpoint.h"
#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"
//#include "math/floatarrayf.h"

namespace oofem {
REGISTER_Element(IntElPoint);

IntElPoint :: IntElPoint(int n, Domain *aDomain) :
    StructuralInterfaceElement(n, aDomain)
{
    numberOfDofMans = 2;
}


void
IntElPoint :: setCoordMode()
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


MaterialMode
IntElPoint :: giveMaterialMode()
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
IntElPoint :: computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
//
// Returns linear part of geometrical equations of the receiver at gp.
// Returns the linear part of the B matrix
//
{
    setCoordMode();

    switch ( mode ) {
    case ie1d_1d:
        answer.resize(1, 2);
        answer.at(1, 1) = -1.0;
        answer.at(1, 2) = +1.0;
        break;
    case ie1d_2d:
        answer.resize(2, 4);
        answer.zero();
        answer.at(1, 1) = -1.0;
        answer.at(1, 3) = +1.0;
        answer.at(2, 2) = -1.0;
        answer.at(2, 4) = +1.0;
        break;
    case ie1d_3d:
        answer.resize(3, 6);
        answer.zero();
        answer.at(1, 1) = -1.0;
        answer.at(1, 4) = +1.0;
        answer.at(2, 2) = -1.0;
        answer.at(2, 5) = +1.0;
        answer.at(3, 3) = -1.0;
        answer.at(3, 6) = +1.0;
        break;
    default:
        OOFEM_ERROR("Unsupported mode");
    }

}


void
IntElPoint :: computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    // Computes transformation matrix to local coordinate system.
    setCoordMode();
    switch ( mode ) {
    case ie1d_1d:
        answer.resize(1, 1);
        answer.at(1, 1) = 1.;
        return;

    case ie1d_2d:
        answer.resize(2, 2);
        answer.at(1, 1) =  normal.at(1);
        answer.at(1, 2) =  normal.at(2);
        answer.at(2, 1) = -normal.at(2);
        answer.at(2, 2) =  normal.at(1);
        return;

    case ie1d_3d:
    {
        //FloatMatrix test;
        //test.beLocalCoordSys(normal.normalize());

        FloatArrayF<3> ly;
        if ( fabs( normal.at(1) ) > fabs( normal.at(2) ) ) {
            ly.at(2) = 1.;
        } else {
            ly.at(1) = 1.;
        }

        auto lz = cross(normal, ly);
        lz /= norm(lz);
        ly = cross(lz, normal);
        ly /= norm(ly);

        answer.resize(3, 3);
        for ( int i = 1; i <= 3; i++ ) {
            answer.at(1, i) = normal.at(i);
            answer.at(2, i) = ly.at(i);
            answer.at(3, i) = lz.at(i);
        }

        return;
    }

    default:
        OOFEM_ERROR("Unsupported mode");
    }
}


void
IntElPoint :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize(1);
        integrationRulesArray [ 0 ] = std::make_unique<GaussIntegrationRule>(1, this, 1, 2);
        integrationRulesArray [ 0 ]->setUpIntegrationPoints( _Line, 1, this->giveMaterialMode() );
    }
}


int
IntElPoint :: computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords)
{
    answer = this->giveNode(1)->giveCoordinates();
    answer.add(this->giveNode(2)->giveCoordinates());
    answer.times(0.5);
    return 1;
}



double
IntElPoint :: computeAreaAround(GaussPoint *gp)
{
    if ( (this->giveCrossSection()->hasProperty(CS_Thickness)) ) {
        double thickness = this->giveCrossSection()->give(CS_Thickness, gp);
        return thickness*this->length;
    } else {
    // The modeled area/extension around the connected nodes. 
    // Compare with the cs area of a bar. ///@todo replace with cs-property? /JB
    return this->area;
    }
}


void
IntElPoint :: initializeFrom(InputRecord &ir)
{
    StructuralInterfaceElement :: initializeFrom(ir);

    if ( ir.hasField(_IFT_IntElPoint_refnode) &&ir.hasField(_IFT_IntElPoint_normal) ) {
        throw ValueInputException(ir, _IFT_IntElPoint_refnode, "Ambiguous input: 'refnode' and 'normal' cannot both be specified");
    }

    IR_GIVE_OPTIONAL_FIELD(ir, referenceNode, _IFT_IntElPoint_refnode);
    FloatArray n(3);
    IR_GIVE_OPTIONAL_FIELD(ir, n, _IFT_IntElPoint_normal);
    normal = n;
    
    /*if ( ir.hasField(_IFT_IntElPoint_refnode) ) {
        IR_GIVE_OPTIONAL_FIELD(ir, referenceNode, _IFT_IntElPoint_refnode);
        normal = *domain->giveNode(this->referenceNode)->giveCoordinates() - *this->giveNode(1)->giveCoordinates();
    } else {
        FloatArray n;
        IR_GIVE_OPTIONAL_FIELD(ir, n, _IFT_IntElPoint_normal);
        normal = n;
    }
    normal /= norm(normal);
    */

    this->area = 1.0; // Default area ///@todo Make non-optional? /JB
    IR_GIVE_OPTIONAL_FIELD(ir, this->area, _IFT_IntElPoint_area);

    this->length = 1.0;
    IR_GIVE_OPTIONAL_FIELD(ir, this->length, _IFT_IntElPoint_length);

    this->computeLocalSlipDir();     
}


int
IntElPoint :: computeNumberOfDofs()
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
        OOFEM_ERROR("Unsupported mode");
    }

    return 0; // to suppress compiler warning
}


void
IntElPoint :: giveDofManDofIDMask(int inode, IntArray &answer) const
{

    switch ( domain->giveNumberOfSpatialDimensions() ) {
    case 1:
        answer = IntArray{ D_u };
        break;
    case 2:
        answer = { D_u, D_v };
        break;
    case 3:
        answer = { D_u, D_v, D_w };
        break;
    default:
        OOFEM_ERROR("Unsupported mode");
    }

}


void
IntElPoint :: computeLocalSlipDir(void)
{
    if ( this->referenceNode ) {
        // normal
        normal = domain->giveNode(this->referenceNode)->giveCoordinates() - this->giveNode(1)->giveCoordinates();
    } else {

      if ( normal.at(1) == 0. && normal.at(2) == 0. && normal.at(3) == 0. ) {
	// let the element compute the slip direction if the nodes' cooridantes are different
        normal = this->giveNode(2)->giveCoordinates() - this->giveNode(1)->giveCoordinates();
	
	// final check
	if ( normal.at(1) == 0. && normal.at(2) == 0. && normal.at(3) == 0. ) {
	  OOFEM_ERROR("Normal is not defined (referenceNode=0,normal=(0,0,0))");
	}
      }
    }

    normal /= norm(normal);
}
  
} // end namespace oofem
