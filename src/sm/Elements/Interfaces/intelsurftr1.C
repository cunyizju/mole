// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/Interfaces/intelsurftr1.h"
#include "dofman/node.h"
#include "cs/crosssection.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(IntElSurfTr1);

FEI3dTrLin IntElSurfTr1 :: interpolation;

IntElSurfTr1 :: IntElSurfTr1(int n, Domain *aDomain) :
    StructuralInterfaceElement(n, aDomain)
{
    numberOfDofMans = 6;
}


void
IntElSurfTr1 :: computeNmatrixAt(GaussPoint *ip, FloatMatrix &answer)
{
    // Returns the modified N-matrix which multiplied with u give the spatial jump.

    FloatArray N;
    this->interpolation.evalN( N, ip->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );

    answer.resize(3, 18);
    answer.zero();

    answer.at(1, 10) = answer.at(2, 11) = answer.at(3, 12) = N.at(1);
    answer.at(1, 1)  = answer.at(2, 2)  = answer.at(3, 3)  = -N.at(1);

    answer.at(1, 13) = answer.at(2, 14) = answer.at(3, 15) = N.at(2);
    answer.at(1, 4)  = answer.at(2, 5)  = answer.at(3, 6)  = -N.at(2);

    answer.at(1, 16) = answer.at(2, 17) = answer.at(3, 18) = N.at(3);
    answer.at(1, 7)  = answer.at(2, 8)  = answer.at(3, 9)  = -N.at(3);
}


void
IntElSurfTr1 :: computeGaussPoints()
{
    // Sets up the array of Gauss Points of the receiver.
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize(1);
        //integrationRulesArray[0] = std::make_unique<LobattoIntegrationRule>(1,domain, 1, 2);
        integrationRulesArray [ 0 ] = std::make_unique<GaussIntegrationRule>(1, this, 1, 3); 
        integrationRulesArray [ 0 ]->setUpIntegrationPoints(_Triangle, 4, _3dInterface); ///@todo add parameter for ngp
    }
}


void
IntElSurfTr1 :: computeCovarBaseVectorsAt(IntegrationPoint *ip, FloatArray &G1, FloatArray &G2)
{
    FloatMatrix dNdxi;
    this->interpolation.evaldNdxi( dNdxi, ip->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    G1.resize(3);
    G2.resize(3);
    G1.zero();
    G2.zero();

    FloatArray meanNode;
    int numNodes = this->giveNumberOfNodes();
    for ( int i = 1; i <= dNdxi.giveNumberOfRows(); i++ ) {
        meanNode = 0.5 * ( this->giveNode(i)->giveCoordinates() + this->giveNode(i + numNodes / 2)->giveCoordinates() );
        G1 += dNdxi.at(i, 1) * meanNode;
        G2 += dNdxi.at(i, 2) * meanNode;
    }
}


double
IntElSurfTr1 :: computeAreaAround(IntegrationPoint *ip)
{
    FloatArray G1, G2, G3;
    this->computeCovarBaseVectorsAt(ip, G1, G2);
    double weight  = ip->giveWeight();
    G3.beVectorProductOf(G1, G2);
    return 0.5 * G3.computeNorm() * weight;
}


void
IntElSurfTr1 :: initializeFrom(InputRecord &ir)
{
    StructuralInterfaceElement :: initializeFrom(ir);
}


void
IntElSurfTr1 :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = { D_u, D_v, D_w };
}

#if 0
bool
IntElSurfTr1 :: computeGtoLRotationMatrix(FloatMatrix &answer)
{
    FloatMatrix lcs;
    computeTransformationMatrixAt(this->giveDefaultIntegrationRulePtr()->getIntegrationPoint(0), lcs);
    answer.resize(18, 18);
    for ( int i = 0; i < 6; i++ ) {
        for ( int j = 1; j <= 3; j++ ) {
            answer.at(i * 3 + 1, i * 3 + j) = lcs.at(3, j);
            answer.at(i * 3 + 2, i * 3 + j) = lcs.at(1, j);
            answer.at(i * 3 + 3, i * 3 + j) = lcs.at(2, j);
        }
    }
    
    
    return 1;
}
#endif

void
IntElSurfTr1 :: computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    // Transformation matrix to the local coordinate system
    FloatArray G1, G2, Normal;
    this->computeCovarBaseVectorsAt(gp, G1, G2);
    Normal.beVectorProductOf(G1, G2);
    Normal.normalize();
    answer.beLocalCoordSys(Normal);
}



int
IntElSurfTr1 :: computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords)
{
    FloatArray N, meanNode;
    this->interpolation.evalN( N, lcoords, FEIElementGeometryWrapper(this) );
    answer.resize(3);
    answer.zero();
    for ( int i = 1; i <= 3; i++ ) {
        meanNode = 0.5 * ( this->giveNode(i)->giveCoordinates() + this->giveNode(i + 3)->giveCoordinates() );
        answer += N.at(i) * meanNode;
    }

    return 1;
}


bool
IntElSurfTr1 :: computeLocalCoordinates(FloatArray &answer, const FloatArray &gcoords)
{
    OOFEM_ERROR("Not implemented");
    return false;
}
} // end namespace oofem
