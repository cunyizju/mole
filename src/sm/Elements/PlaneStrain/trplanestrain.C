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

#include "sm/Elements/PlaneStrain/trplanestrain.h"
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
REGISTER_Element(TrPlaneStrain);

FEI2dTrLin TrPlaneStrain :: interp(1, 2);

TrPlaneStrain :: TrPlaneStrain(int n, Domain *aDomain) :
    PlaneStrainElement(n, aDomain), ZZNodalRecoveryModelInterface(this), NodalAveragingRecoveryModelInterface(),
    SPRNodalRecoveryModelInterface(), SpatialLocalizerInterface(this),
    ZZErrorEstimatorInterface(this),
    HuertaErrorEstimatorInterface()
    // Constructor.
{
    numberOfDofMans  = 3;
    area = -1;
    numberOfGaussPoints = 1;
}


FEInterpolation *TrPlaneStrain :: giveInterpolation() const { return & interp; }


Interface *
TrPlaneStrain :: giveInterface(InterfaceType interface)
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
    }

    return NULL;
}

void
TrPlaneStrain :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 1;
    PlaneStrainElement :: initializeFrom(ir);

    if ( numberOfGaussPoints != 1 ) {
        numberOfGaussPoints = 1;
    }
}



void
TrPlaneStrain :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                            InternalStateType type, TimeStep *tStep)
{
    GaussPoint *gp;
    gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    this->giveIPValue(answer, gp, type, tStep);
}


void
TrPlaneStrain :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(3);
    pap.at(1) = this->giveNode(1)->giveNumber();
    pap.at(2) = this->giveNode(2)->giveNumber();
    pap.at(3) = this->giveNode(3)->giveNumber();
}


void
TrPlaneStrain :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
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
TrPlaneStrain :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return 1;
}


SPRPatchType
TrPlaneStrain :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dxy;
}


void
TrPlaneStrain :: HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                                  IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                                  HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                                  int &localNodeId, int &localElemId, int &localBcId,
                                                                  IntArray &controlNode, IntArray &controlDof,
                                                                  HuertaErrorEstimator :: AnalysisMode aMode)
{
    int nodes = 3, sides = 3;
    FloatArray corner [ 3 ], midSide [ 3 ], midNode, cor [ 3 ];
    double x = 0.0, y = 0.0;

    static int sideNode [ 3 ] [ 2 ] = { { 1, 2 }, { 2, 3 }, { 3, 1 } };

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
                                       controlNode, controlDof, aMode, "Quad1PlaneStrain");
}


void TrPlaneStrain :: HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    computeNmatrixAt(gp->giveSubPatchCoordinates(), answer);
}

} // end namespace oofem
