// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "intelline2intpen.h"

#include "dofman/dofmanager.h"
#include "fei/fei2dlinelin.h"
#include "fei/fei2dlinequad.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"


namespace oofem {
REGISTER_Element(IntElLine2IntPen);

IntElLine2IntPen::IntElLine2IntPen(int n, Domain * d):IntElLine2(n, d)
{
    numberOfDofMans = 6;
    numberOfGaussPoints = 4;
}


void
IntElLine2IntPen :: initializeFrom(InputRecord &ir)
{
    StructuralInterfaceElement :: initializeFrom(ir);
    this->axisymmode = ir.hasField(_IFT_IntElLine1_axisymmode);

    // Check if node numbering is ok
//    printf("this->dofManArray: "); this->dofManArray.printYourself();
//    DofManager *node1 = this->giveDofManager(1);
//    const FloatArray &x1 = *(node1->giveCoordinates());
//    printf("x1: "); x1.printYourself();

//    DofManager *node2 = this->giveDofManager(2);
//    const FloatArray &x2 = *(node2->giveCoordinates());
//    printf("x2: "); x2.printYourself();

//    DofManager *node3 = this->giveDofManager(3);
//    const FloatArray &x3 = *(node3->giveCoordinates());
//    printf("x3: "); x3.printYourself();

//    DofManager *node4 = this->giveDofManager(4);
//    const FloatArray &x4 = *(node4->giveCoordinates());
//    printf("x4: "); x4.printYourself();

//    IntArray nodeInd = {dofManArray.at(2), dofManArray.at(3), dofManArray.at(4)};

//    double L2 = distance_square(x1, x2);
//    printf("L2: %e\n", L2);

//    double L3 = distance_square(x1, x3);
//    printf("L3: %e\n", L3);

//    double L4 = distance_square(x1, x4);
//    printf("L4: %e\n", L4);
//
//    FloatArray dist2 = {L2, L3, L4};
//
//    double minDist2 = 1.0e20, maxDist2 =-1.0;
//    int minDistInd = -1, maxDistInd = -1;

//    for(int i = 1; i <= 3; i++ ) {
//    	if(dist2.at(i) <= minDist2) {
//    		minDist2 = dist2.at(i);
//    		minDistInd = i;
//    	}
//
//    	if(dist2.at(i) >= maxDist2) {
//    		maxDist2 = dist2.at(i);
//    		maxDistInd = i;
//    	}
//    }
//
//    printf("minDistInd: %d\n", minDistInd);
//    printf("maxDistInd: %d\n", maxDistInd);


//    int midDistInd = -1;
//    for(int i = 1; i <= 3; i++ ) {
//    	if(i != minDistInd && i != maxDistInd) {
//    		midDistInd = i;
//    	}
//    }
//    printf("midDistInd: %d\n", midDistInd);


//    if(L2 < L3) {
//    	printf("Renumbering element %d\n.\n", this->giveNumber());
//    	std::swap(dofManArray.at(2), dofManArray.at(3));
//    }
}


FloatArrayF<2>
IntElLine2IntPen :: computeCovarBaseVectorAt(IntegrationPoint *ip) const
{
    //printf("Entering IntElLine2IntPen :: computeCovarBaseVectorAt\n");

    // Since we are averaging over the whole element, always evaluate the base vectors at xi = 0.

	FloatArray xi_0 = {0.0};

    FEInterpolation *interp = this->giveInterpolation();
    FloatMatrix dNdxi;
//    interp->evaldNdxi( dNdxi, ip->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    interp->evaldNdxi( dNdxi, xi_0, FEIElementGeometryWrapper(this) );

    FloatArrayF<2> G;
    int numNodes = this->giveNumberOfNodes();
    for ( int i = 1; i <= dNdxi.giveNumberOfRows(); i++ ) {
        double X1_i = 0.5 * ( this->giveNode(i)->giveCoordinate(1) + this->giveNode(i + numNodes / 2)->giveCoordinate(1) ); // (mean) point on the fictious mid surface
        double X2_i = 0.5 * ( this->giveNode(i)->giveCoordinate(2) + this->giveNode(i + numNodes / 2)->giveCoordinate(2) );
        G.at(1) += dNdxi.at(i, 1) * X1_i;
        G.at(2) += dNdxi.at(i, 1) * X2_i;
    }
    return G;
}

void
IntElLine2IntPen :: computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep)
{
    // Computes the stiffness matrix of the receiver K_cohesive = int_A ( N^t * dT/dj * N ) dA
    FloatMatrix N, D, DN;
    bool matStiffSymmFlag = this->giveCrossSection()->isCharacteristicMtrxSymmetric(rMode);
    answer.clear();

    FloatMatrix rotationMatGtoL;
    FloatArray u;

    // First loop over GP: compute projection of test function and traction.
    // The setting is as follows: we have an interface with quadratic interpolation and we
    // wish to project onto the space of constant functions on each element.

    // Projecting the basis functions gives a constant for each basis function.
    FloatMatrix proj_N;
    proj_N.clear();

    FloatMatrix proj_DN;
    proj_DN.clear();

    double area = 0.;

    for ( auto &ip: *this->giveDefaultIntegrationRulePtr() ) {

        this->computeNmatrixAt(ip, N);

        if ( this->nlGeometry == 0 ) {
            this->giveStiffnessMatrix_Eng(D, rMode, ip, tStep);
        } else if ( this->nlGeometry == 1 ) {
            this->giveStiffnessMatrix_dTdj(D, rMode, ip, tStep);
        } else {
            OOFEM_ERROR("nlgeometry must be 0 or 1!")
        }

        this->computeTransformationMatrixAt(ip, rotationMatGtoL);
        D.rotatedWith(rotationMatGtoL, 'n');                      // transform stiffness to global coord system

        this->computeNmatrixAt(ip, N);
        DN.beProductOf(D, N);


        double dA = this->computeAreaAround(ip);
        area += dA;

        proj_N.add(dA, N);
        proj_DN.add(dA, DN);
    }

//    printf("area: %e\n", area);
    proj_N.times(1./area);
    proj_DN.times(1./area);

//    printf("proj_N: "); proj_N.printYourself();

    for ( auto &ip: *this->giveDefaultIntegrationRulePtr() ) {

        if ( this->nlGeometry == 0 ) {
            this->giveStiffnessMatrix_Eng(D, rMode, ip, tStep);
        } else if ( this->nlGeometry == 1 ) {
            this->giveStiffnessMatrix_dTdj(D, rMode, ip, tStep);
        } else {
            OOFEM_ERROR("nlgeometry must be 0 or 1!")
        }

        this->computeTransformationMatrixAt(ip, rotationMatGtoL);
        D.rotatedWith(rotationMatGtoL, 'n');                      // transform stiffness to global coord system

        DN.beProductOf(D, proj_N);


        double dA = this->computeAreaAround(ip);
        if ( matStiffSymmFlag ) {
            answer.plusProductSymmUpper(proj_N, DN, dA);
        } else {
            answer.plusProductUnsym(proj_N, DN, dA);
        }
    }


    if ( matStiffSymmFlag ) {
        answer.symmetrized();
    }
}


void
IntElLine2IntPen :: giveInternalForcesVector(FloatArray &answer,
                                                       TimeStep *tStep, int useUpdatedGpRecord)
{
    // Computes internal forces
	// For this element we use an "interior penalty" formulation, where
	// the cohesive zone contribution is weakened, i.e. the traction and
	// test function for the cohesive zone are projected onto a reduced
	// space. The purpose of the projection is to improve the stability
	// properties of the formulation, thereby avoiding traction oscilations.

    FloatMatrix N;
    FloatArray u, traction, jump;

    this->computeVectorOf(VM_Total, tStep, u);
    // subtract initial displacements, if defined
    if ( initialDisplacements.giveSize() ) {
        u.subtract(initialDisplacements);
    }

    // zero answer will resize accordingly when adding first contribution
    answer.clear();



    // First loop over GP: compute projection of test function and traction.
    // The setting is as follows: we have an interface with quadratic interpolation and we
    // wish to project onto the space of constant functions on each element.

    // The projection of t becomes a constant
//    FloatArray proj_t;
//    proj_t.clear();


    FloatArray proj_jump;
    proj_jump.clear();

    // Projecting the basis functions gives a constant for each basis function.
    FloatMatrix proj_N;
    proj_N.clear();

    double area = 0.;

    for ( auto &ip: *this->giveDefaultIntegrationRulePtr() ) {

        this->computeNmatrixAt(ip, N);
        jump.beProductOf(N, u);
//        this->computeTraction(traction, ip, jump, tStep);

        double dA = this->computeAreaAround(ip);
        area += dA;

        proj_jump.add(dA, jump);
        proj_N.add(dA, N);
    }

//    printf("area: %e\n", area);
    proj_jump.times(1./area);
    proj_N.times(1./area);

//    printf("proj_N: "); proj_N.printYourself();


    // Second loop over GP: assemble contribution to internal forces as we are used to.
    for ( auto &ip: *this->giveDefaultIntegrationRulePtr() ) {
//        this->computeNmatrixAt(ip, N);
//        jump.beProductOf(N, u);
        this->computeTraction(traction, ip, proj_jump, tStep);

        // compute internal cohesive forces as f = N^T*traction dA
        double dA = this->computeAreaAround(ip);
        answer.plusProduct(proj_N, traction, dA);
    }

}



} /* namespace oofem */
