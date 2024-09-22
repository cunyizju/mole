/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "sm/Elements/PlaneStress/trplanstrss.h"
#include "fei/fei2dtrlin.h"
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
REGISTER_Element(TrPlaneStress2d);

FEI2dTrLin TrPlaneStress2d :: interp(1, 2);

TrPlaneStress2d :: TrPlaneStress2d(int n, Domain *aDomain) :
    PlaneStressElement(n, aDomain), ZZNodalRecoveryModelInterface(this), NodalAveragingRecoveryModelInterface(),
    SPRNodalRecoveryModelInterface(), SpatialLocalizerInterface(this),
    ZZErrorEstimatorInterface(this),
    HuertaErrorEstimatorInterface(),
    LayeredCrossSectionInterface()
{
    numberOfDofMans  = 3;
    area = -1;
    numberOfGaussPoints = 1;
}

FEInterpolation *TrPlaneStress2d :: giveInterpolation() const { return & interp; }

Interface *
TrPlaneStress2d :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == NodalAveragingRecoveryModelInterfaceType ) {
        return static_cast< NodalAveragingRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    } else if ( interface == ZZErrorEstimatorInterfaceType ) {
        return static_cast< ZZErrorEstimatorInterface * >(this);
    } else if ( interface == HuertaErrorEstimatorInterfaceType ) {
        return static_cast< HuertaErrorEstimatorInterface * >(this);
    } else if ( interface == LayeredCrossSectionInterfaceType ) {
        return static_cast< LayeredCrossSectionInterface * >(this);
    }

    return NULL;
}

double
TrPlaneStress2d :: giveArea()
// returns the area occupied by the receiver
{
    if ( area > 0 ) {
        return area;         // check if previously computed
    }

    return ( area = fabs( this->interp.giveArea( FEIElementGeometryWrapper(this) ) ) );
}


double
TrPlaneStress2d :: giveCharacteristicSize(GaussPoint *gp, FloatArray &normalToCrackPlane, ElementCharSizeMethod method)
//
// returns receiver's characteristic size at gp (for some material models)
// for crack formed in plane with normal normalToCrackPlane
// using the selected method
//
{
    if ( method == ECSM_SquareRootOfArea ) {
        // square root of element area
        return sqrt( this->giveArea() );
    }

    if ( ( method == ECSM_Projection ) || ( method == ECSM_ProjectionCentered ) ) {
        // projection method (standard or modified - no difference for constant-strain element)
        return this->giveCharacteristicLength(normalToCrackPlane);
    }

    if ( ( method == ECSM_Oliver1 ) || ( method == ECSM_Oliver1modified ) || ( method == ECSM_Oliver2 ) ) {
        // method based on derivative of auxiliary function phi
        // in the maximum principal strain direction
        // (standard or modified - no difference for constant-strain element)

        // nodal coordinates and coordinates of the element center
        FloatArray x(3), y(3);
        double cx = 0., cy = 0.;
        for ( int i = 1; i <= 3; i++ ) {
            x.at(i) = giveNode(i)->giveCoordinate(1);
            y.at(i) = giveNode(i)->giveCoordinate(2);
            cx += x.at(i);
            cy += y.at(i);
        }

        cx /= 3.;
        cy /= 3.;

        // nodal values of function phi (0 or 1)
        FloatArray phi(3);
        for ( int i = 1; i <= 3; i++ ) {
            if ( ( ( x.at(i) - cx ) * normalToCrackPlane.at(1) + ( y.at(i) - cy ) * normalToCrackPlane.at(2) ) > 0. ) {
                phi.at(i) = 1.;
            } else {
                phi.at(i) = 0.;
            }
        }

        // derivatives of shape functions wrt global coordinates
        FloatMatrix dnx(3, 2);
        dnx.at(1, 1) = y.at(2) - y.at(3);
        dnx.at(2, 1) = y.at(3) - y.at(1);
        dnx.at(3, 1) = y.at(1) - y.at(2);
        dnx.at(1, 2) = x.at(3) - x.at(2);
        dnx.at(2, 2) = x.at(1) - x.at(3);
        dnx.at(3, 2) = x.at(2) - x.at(1);
        dnx.times( 1. / ( 2. * giveArea() ) );

        // gradient of function phi
        FloatArray gradPhi(2);
        gradPhi.zero();
        for ( int i = 1; i <= 3; i++ ) {
            gradPhi.at(1) += phi.at(i) * dnx.at(i, 1);
            gradPhi.at(2) += phi.at(i) * dnx.at(i, 2);
        }

        // scalar product of the gradient with crack normal
        double dPhidN = 0.;
        for ( int i = 1; i <= 2; i++ ) {
            dPhidN += gradPhi.at(i) * normalToCrackPlane.at(i);
        }

        if ( dPhidN == 0. ) {
            OOFEM_ERROR("Zero value of dPhidN");
        }

        return 1. / fabs(dPhidN);
    }

    OOFEM_ERROR("invalid method");
    return 0.;
}


void
TrPlaneStress2d :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                              InternalStateType type, TimeStep *tStep)
{
    GaussPoint *gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    this->giveIPValue(answer, gp, type, tStep);
}



void
TrPlaneStress2d :: HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                                    IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                                    HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                                    int &localNodeId, int &localElemId, int &localBcId,
                                                                    IntArray &controlNode, IntArray &controlDof,
                                                                    HuertaErrorEstimator :: AnalysisMode aMode)
{
    int nodes = 3, sides = 3;
    double x = 0.0, y = 0.0;

    static int sideNode [ 3 ] [ 2 ] = { { 1, 2 }, { 2, 3 }, { 3, 1 } };

    FloatArray corner [ 3 ], midSide [ 3 ], midNode, cor [ 3 ];
    if ( sMode == HuertaErrorEstimatorInterface :: NodeMode ||
        ( sMode == HuertaErrorEstimatorInterface :: BCMode && aMode == HuertaErrorEstimator :: HEE_linear ) ) {
        for ( int inode = 0; inode < nodes; inode++ ) {
            corner [ inode ] = this->giveNode(inode + 1)->giveCoordinates();
            if ( corner [ inode ].giveSize() != 3 ) {
                cor [ inode ].resize(3);
                cor [ inode ].at(1) = corner [ inode ].at(1);
                cor [ inode ].at(2) = corner [ inode ].at(2);
                cor [ inode ].at(3) = 0.0;

                corner [ inode ] = cor [ inode ];
            }

            x += corner [ inode ].at(1);
            y += corner [ inode ].at(2);
        }

        for ( int iside = 0; iside < sides; iside++ ) {
            midSide [ iside ].resize(3);

            int nd1 = sideNode [ iside ] [ 0 ] - 1;
            int nd2 = sideNode [ iside ] [ 1 ] - 1;

            midSide [ iside ].at(1) = ( corner [ nd1 ].at(1) + corner [ nd2 ].at(1) ) / 2.0;
            midSide [ iside ].at(2) = ( corner [ nd1 ].at(2) + corner [ nd2 ].at(2) ) / 2.0;
            midSide [ iside ].at(3) = 0.0;
        }

        midNode.resize(3);

        midNode.at(1) = x / nodes;
        midNode.at(2) = y / nodes;
        midNode.at(3) = 0.0;
    }

    this->setupRefinedElementProblem2D(this, refinedElement, level, nodeId, localNodeIdArray, globalNodeIdArray,
                                       sMode, tStep, nodes, corner, midSide, midNode,
                                       localNodeId, localElemId, localBcId,
                                       controlNode, controlDof, aMode, "PlaneStress2d");
}


void TrPlaneStress2d :: HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    computeNmatrixAt(gp->giveSubPatchCoordinates(), answer);
}

void
TrPlaneStress2d :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(3);
    pap.at(1) = this->giveNode(1)->giveNumber();
    pap.at(2) = this->giveNode(2)->giveNumber();
    pap.at(3) = this->giveNode(3)->giveNumber();
}

void
TrPlaneStress2d :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    answer.resize(1);
    if ( ( pap == this->giveNode(1)->giveNumber() ) ||
        ( pap == this->giveNode(2)->giveNumber() ) ||
        ( pap == this->giveNode(3)->giveNumber() ) ) {
        answer.at(1) = pap;
    } else {
        OOFEM_ERROR("node unknown");
    }
}

int
TrPlaneStress2d :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return 1;
}


SPRPatchType
TrPlaneStress2d :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dxy;
}

void
TrPlaneStress2d :: computeStrainVectorInLayer(FloatArray &answer, const FloatArray &masterGpStrain, GaussPoint *masterGp, GaussPoint *slaveGp, TimeStep *tStep)
{
  answer=masterGpStrain;
}


} // end namespace oofem
