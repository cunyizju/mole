// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/Bars/truss3d.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "fei/fei3dlinelin.h"
#include "dofman/node.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(Truss3d);

FEI3dLineLin Truss3d::interp;

Truss3d::Truss3d(int n, Domain *aDomain) :
    NLStructuralElement(n, aDomain), ZZNodalRecoveryModelInterface(this)
{
    numberOfDofMans = 2;
}


FEInterpolation *Truss3d::giveInterpolation() const { return & interp; }


Interface *
Truss3d::giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >( this );
    } else if ( interface == NodalAveragingRecoveryModelInterfaceType ) {
        return static_cast< NodalAveragingRecoveryModelInterface * >( this );
    }

    //OOFEM_LOG_INFO("Interface on Truss3d element not supported");
    return NULL;
}


void
Truss3d::NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep)
{
    auto gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    this->giveCrossSection()->giveIPValue(answer, gp, type, tStep);
}


void
Truss3d::computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
//
// Returns linear part of geometrical equations of the receiver at gp.
// Returns the linear part of the B matrix
//
{
    FloatMatrix dN;
    this->interp.evaldNdx(dN, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );

    answer.resize(1, 6);
    answer.at(1, 1) = dN.at(1, 1);
    answer.at(1, 2) = dN.at(1, 2);
    answer.at(1, 3) = dN.at(1, 3);
    answer.at(1, 4) = dN.at(2, 1);
    answer.at(1, 5) = dN.at(2, 2);
    answer.at(1, 6) = dN.at(2, 3);
}


void
Truss3d::computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    this->computeBmatrixAt(gp, answer);
}


void
Truss3d::computeGaussPoints()
// Sets up the array of Gauss Points of the receiver.
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize(1);
        integrationRulesArray [ 0 ] = std::make_unique< GaussIntegrationRule >(1, this, 1, 2);
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], 1, this);
    }
}


double
Truss3d::computeLength()
{
    return this->interp.giveLength(FEIElementGeometryWrapper(this) );
}


void
Truss3d::computeInitialStressMatrix(FloatMatrix &answer, TimeStep *tStep)
{
    // computes initial stress matrix of receiver (or geometric stiffness matrix)
    double area, NForce, contrib;
    double l0 = this->computeLength();
    FloatArray stress, strain;
    GaussPoint *gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    
    this->computeStrainVector(strain, gp, tStep);
    this->computeStressVector(stress, strain, gp, tStep);
    area = this->giveCrossSection()->give(CS_Area, gp);
    NForce = stress.at(1)*area;
    contrib = NForce / l0;
    
    answer.resize(6, 6);
    answer.zero();

    answer.at(2,2)=answer.at(3,3)=answer.at(5,5)=answer.at(6,6) = contrib;
    answer.at(2,5)=answer.at(5,2)=answer.at(3,6)=answer.at(6,3) = -contrib;
}

void
Truss3d::computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep)
// Returns the lumped mass matrix of the receiver. This expression is
// valid in both local and global axes.
{
    answer.resize(6, 6);
    answer.zero();
    if ( !this->isActivated(tStep) ) {
        return;
    }

    GaussPoint *gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    double density = this->giveStructuralCrossSection()->give('d', gp);
    double halfMass = density * this->giveCrossSection()->give(CS_Area, gp) * this->computeLength() * 0.5;
    answer.at(1, 1) = halfMass;
    answer.at(2, 2) = halfMass;
    answer.at(3, 3) = halfMass;
    answer.at(4, 4) = halfMass;
    answer.at(5, 5) = halfMass;
    answer.at(6, 6) = halfMass;
}


void
Truss3d::computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer)
// Returns the displacement interpolation matrix {N} of the receiver, eva-
// luated at gp.
{
    FloatArray n;
    this->interp.evalN(n, iLocCoord, FEIElementGeometryWrapper(this) );
    answer.beNMatrixOf(n, 3);
}


double
Truss3d::computeVolumeAround(GaussPoint *gp)
// Returns the length of the receiver. This method is valid only if 1
// Gauss point is used.
{
    double detJ = this->interp.giveTransformationJacobian(gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    double weight  = gp->giveWeight();
    return detJ * weight * this->giveCrossSection()->give(CS_Area, gp);
}


int
Truss3d::giveLocalCoordinateSystem(FloatMatrix &answer)
//
// returns a unit vectors of local coordinate system at element
// stored rowwise (mainly used by some materials with ortho and anisotrophy)
//
{
    FloatArray lx, ly(3), lz;

    lx.beDifferenceOf(this->giveNode(2)->giveCoordinates(), this->giveNode(1)->giveCoordinates() );
    lx.normalize();

    ly(0) = lx(1);
    ly(1) = -lx(2);
    ly(2) = lx(0);

    // Construct orthogonal vector
    double npn = ly.dotProduct(lx);
    ly.add(-npn, lx);
    ly.normalize();
    lz.beVectorProductOf(ly, lx);

    answer.resize(3, 3);
    for ( int i = 1; i <= 3; i++ ) {
        answer.at(1, i) = lx.at(i);
        answer.at(2, i) = ly.at(i);
        answer.at(3, i) = lz.at(i);
    }

    return 1;
}


void
Truss3d::initializeFrom(InputRecord &ir)
{
    NLStructuralElement::initializeFrom(ir);
}


void
Truss3d::computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    answer = this->giveStructuralCrossSection()->giveRealStress_1d(strain, gp, tStep);
}

void
Truss3d::computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = this->giveStructuralCrossSection()->giveStiffnessMatrix_1d(rMode, gp, tStep);
}

void
Truss3d::computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = this->giveStructuralCrossSection()->giveStiffnessMatrix_dPdF_1d(rMode, gp, tStep);
}

void
Truss3d::giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = { D_u, D_v, D_w };
}


void
Truss3d::giveEdgeDofMapping(IntArray &answer, int iEdge) const
{
    /*
     * provides dof mapping of local edge dofs (only nonzero are taken into account)
     * to global element dofs
     */

    if ( iEdge != 1 ) {
        OOFEM_ERROR("wrong edge number");
    }

    answer.resize(6);
    answer.at(1) = 1;
    answer.at(2) = 2;
    answer.at(3) = 3;
    answer.at(4) = 4;
    answer.at(5) = 5;
    answer.at(6) = 6;
}


double
Truss3d::computeEdgeVolumeAround(GaussPoint *gp, int iEdge)
{
    if ( iEdge != 1 ) { // edge between nodes 1 2
        OOFEM_ERROR("wrong edge number");
    }

    double weight = gp->giveWeight();
    return this->interp.giveTransformationJacobian(gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) ) * weight;
}


int
Truss3d::computeLoadLEToLRotationMatrix(FloatMatrix &answer, int iEdge, GaussPoint *gp)
{
    // returns transformation matrix from
    // edge local coordinate system
    // to element local c.s
    // (same as global c.s in this case)
    //
    // i.e. f(element local) = T * f(edge local)
    //
    FloatMatrix lcs;
    this->giveLocalCoordinateSystem(lcs);
    answer.beTranspositionOf(lcs);

    return 1;
}
} // end namespace oofem
