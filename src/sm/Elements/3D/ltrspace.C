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

#include "sm/Elements/3D/ltrspace.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "dofman/node.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "fei/fei3dtetlin.h"
#include "engng/classfactory.h"


namespace oofem {
REGISTER_Element(LTRSpace);

FEI3dTetLin LTRSpace :: interpolation;

LTRSpace :: LTRSpace(int n, Domain *aDomain) :
    Structural3DElement(n, aDomain), ZZNodalRecoveryModelInterface(this), NodalAveragingRecoveryModelInterface(),
    SPRNodalRecoveryModelInterface(), SpatialLocalizerInterface(this),
    ZZErrorEstimatorInterface(this),
    HuertaErrorEstimatorInterface()

{
    numberOfDofMans  = 4;
    numberOfGaussPoints = 1;
}


Interface *
LTRSpace :: giveInterface(InterfaceType interface)
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


FEInterpolation *
LTRSpace :: giveInterpolation() const
{
    return & interpolation;
}



void
LTRSpace :: computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep)
// Returns the lumped mass matrix of the receiver.
{
    GaussPoint *gp;
    double dV, mss1;

    answer.resize(12, 12);
    answer.zero();
    gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);

    dV = this->computeVolumeAround(gp);
    double density = this->giveStructuralCrossSection()->give('d', gp);
    mss1 = dV * density / 4.;

    for ( int i = 1; i <= 12; i++ ) {
        answer.at(i, i) = mss1;
    }
}


void
LTRSpace :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                       InternalStateType type, TimeStep *tStep)
{
    GaussPoint *gp;

    if ( numberOfGaussPoints != 1 ) {
        answer.clear(); // for more gp's need to be refined
        return;
    }

    gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    giveIPValue(answer, gp, type, tStep);
}


void
LTRSpace :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(4);
    pap.at(1) = this->giveNode(1)->giveNumber();
    pap.at(2) = this->giveNode(2)->giveNumber();
    pap.at(3) = this->giveNode(3)->giveNumber();
    pap.at(4) = this->giveNode(4)->giveNumber();
}


void
LTRSpace :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    int found = 0;
    answer.resize(1);

    for ( int i = 1; i <= 4; i++ ) {
        if ( this->giveNode(i)->giveNumber() == pap ) {
            found = 1;
        }
    }

    if ( found ) {
        answer.at(1) = pap;
    } else {
        OOFEM_ERROR("node unknown");
    }
}


int
LTRSpace :: SPRNodalRecoveryMI_giveNumberOfIP()
{ return this->giveDefaultIntegrationRulePtr()->giveNumberOfIntegrationPoints(); }


SPRPatchType
LTRSpace :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_3dBiLin;
}


void
LTRSpace :: HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                             IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                             HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                             int &localNodeId, int &localElemId, int &localBcId,
                                                             IntArray &controlNode, IntArray &controlDof,
                                                             HuertaErrorEstimator :: AnalysisMode aMode)
{
    double x = 0.0, y = 0.0, z = 0.0;
    int nodes = 4, sides = 6, faces = 4;

    static int sideNode [ 6 ] [ 2 ] = { { 1, 2 }, { 2, 3 }, { 3, 1 }, { 1, 4 }, { 2, 4 }, { 3, 4 } };
    static int faceNode [ 4 ] [ 3 ] = { { 1, 2, 3 }, { 1, 2, 4 }, { 2, 3, 4 }, { 3, 1, 4 } };

    /* ordering of hexa nodes must be compatible with refinedElement connectivity ordering;
     * generally the ordering is: corner side side face side face face center;
     * however the concrete ordering is element dependent (see refineMeshGlobally source if in doubts) */

    int hexaSideNode [ 4 ] [ 3 ] = { { 1, 3, 4 }, { 2, 1, 5 }, { 3, 2, 6 }, { 4, 6, 5 } };
    int hexaFaceNode [ 4 ] [ 3 ] = { { 1, 2, 4 }, { 1, 3, 2 }, { 1, 4, 3 }, { 4, 2, 3 } };

    FloatArray corner [ 4 ], midSide [ 6 ], midFace [ 4 ], midNode;
    if ( sMode == HuertaErrorEstimatorInterface :: NodeMode ||
        ( sMode == HuertaErrorEstimatorInterface :: BCMode && aMode == HuertaErrorEstimator :: HEE_linear ) ) {
        for ( int inode = 0; inode < nodes; inode++ ) {
            corner [ inode ] = this->giveNode(inode + 1)->giveCoordinates();

            x += corner [ inode ].at(1);
            y += corner [ inode ].at(2);
            z += corner [ inode ].at(3);
        }

        for ( int iside = 0; iside < sides; iside++ ) {
            midSide [ iside ].resize(3);

            int nd1 = sideNode [ iside ] [ 0 ] - 1;
            int nd2 = sideNode [ iside ] [ 1 ] - 1;

            midSide [ iside ].at(1) = ( corner [ nd1 ].at(1) + corner [ nd2 ].at(1) ) / 2.0;
            midSide [ iside ].at(2) = ( corner [ nd1 ].at(2) + corner [ nd2 ].at(2) ) / 2.0;
            midSide [ iside ].at(3) = ( corner [ nd1 ].at(3) + corner [ nd2 ].at(3) ) / 2.0;
        }

        midNode.resize(3);

        midNode.at(1) = x / nodes;
        midNode.at(2) = y / nodes;
        midNode.at(3) = z / nodes;

        for ( int iface = 0; iface < faces; iface++ ) {
            x = y = z = 0.0;
            for ( int inode = 0; inode < 3; inode++ ) {
                int nd = faceNode [ iface ] [ inode ] - 1;
                x += corner [ nd ].at(1);
                y += corner [ nd ].at(2);
                z += corner [ nd ].at(3);
            }

            midFace [ iface ].resize(3);

            midFace [ iface ].at(1) = x / 3;
            midFace [ iface ].at(2) = y / 3;
            midFace [ iface ].at(3) = z / 3;
        }
    }

    this->setupRefinedElementProblem3D(this, refinedElement, level, nodeId, localNodeIdArray, globalNodeIdArray,
                                       sMode, tStep, nodes, corner, midSide, midFace, midNode,
                                       localNodeId, localElemId, localBcId, hexaSideNode, hexaFaceNode,
                                       controlNode, controlDof, aMode, "LSpace");
}

void LTRSpace :: HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    computeNmatrixAt(gp->giveSubPatchCoordinates(), answer);
}

} // end namespace oofem
