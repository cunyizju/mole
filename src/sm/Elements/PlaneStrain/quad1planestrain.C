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

#include "sm/Elements/PlaneStrain/quad1planestrain.h"
#include "fei/fei2dquadlin.h"
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
REGISTER_Element(Quad1PlaneStrain);

FEI2dQuadLin Quad1PlaneStrain :: interp(1, 2);

Quad1PlaneStrain :: Quad1PlaneStrain(int n, Domain *aDomain) :
    PlaneStrainElement(n, aDomain), ZZNodalRecoveryModelInterface(this), SPRNodalRecoveryModelInterface(),
    SpatialLocalizerInterface(this),
    HuertaErrorEstimatorInterface()
{
    numberOfDofMans  = 4;
    numberOfGaussPoints = 4;
}


Quad1PlaneStrain :: ~Quad1PlaneStrain()
{ }


FEInterpolation *Quad1PlaneStrain :: giveInterpolation() const { return & interp; }


void
Quad1PlaneStrain :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
//
// Returns the [4x8] strain-displacement matrix {B} of the receiver,
// evaluated at gp.
// (epsilon_x,epsilon_y,epsilon_z,gamma_xy) = B . r
// r = ( u1,v1,u2,v2,u3,v3,u4,v4)
{
    FloatMatrix dN;

    this->interp.evaldNdx( dN, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );

    // Reshape
    answer.resize(4, 8);
    answer.zero();

#ifdef Quad1PlaneStrain_reducedVolumetricIntegration
    FloatMatrix dN_red;
    FloatArray lcoords_red(2);
    lcoords_red.zero();
    this->interp.evaldNdx( dN_red, lcoords_red, FEIElementGeometryWrapper(this) );

    for ( int i = 1; i <= 4; i++ ) {
      answer.at(1, 2 * i - 1) = ( dN.at(i, 1) + dN_red.at(i, 1) ) / 2.;
      answer.at(1, 2 * i - 0) = (-dN.at(i, 2) + dN_red.at(i, 2) ) / 2.;
      answer.at(2, 2 * i - 1) = (-dN.at(i, 1) + dN_red.at(i, 1) ) / 2.;
      answer.at(2, 2 * i - 0) = ( dN.at(i, 2) + dN_red.at(i, 2) ) / 2.;
      answer.at(4, 2 * i - 1) = dN.at(i, 2);
      answer.at(4, 2 * i - 0) = dN.at(i, 1);
    }
#else
#ifdef Quad1PlaneStrain_reducedShearIntegration
    FloatMatrix dN_red;
    FloatArray lcoords_red(2);
    lcoords_red.zero();
    this->interp.evaldNdx( dN_red, lcoords_red, FEIElementGeometryWrapper(this) );

    for ( int i = 1; i <= 4; i++ ) {
        answer.at(1, 2 * i - 1) = dN.at(i, 1);
        answer.at(2, 2 * i - 0) = dN.at(i, 2);
        answer.at(4, 2 * i - 1) = dN_red.at(i, 2);
        answer.at(4, 2 * i - 0) = dN_red.at(i, 1);
    }

#else
    for ( int i = 1; i <= 4; i++ ) {
        answer.at(1, 2 * i - 1) = dN.at(i, 1);
        answer.at(2, 2 * i - 0) = dN.at(i, 2);
        answer.at(4, 2 * i - 1) = dN.at(i, 2);
        answer.at(4, 2 * i - 0) = dN.at(i, 1);
    }

#endif
#endif
}

void
Quad1PlaneStrain :: computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer)
// Returns the [5x8] displacement gradient matrix {BH} of the receiver,
// evaluated at gp.
{
    FloatMatrix dnx;
    this->interp.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );

    answer.resize(5, 8);
    answer.zero();
    // 3rd row is zero -> dw/dz = 0
    for ( int i = 1; i <= 4; i++ ) {
        answer.at(1, 2 * i - 1) = dnx.at(i, 1);     // du/dx -1
        answer.at(2, 2 * i - 0) = dnx.at(i, 2);     // dv/dy -2
        answer.at(4, 2 * i - 1) = dnx.at(i, 2);     // du/dy -6
        answer.at(5, 2 * i - 0) = dnx.at(i, 1);     // dv/dx -9
    }
}


void
Quad1PlaneStrain :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    PlaneStrainElement :: initializeFrom(ir);

    if ( !( ( numberOfGaussPoints == 4 ) ||
            ( numberOfGaussPoints == 1 ) ||
            ( numberOfGaussPoints == 9 ) ||
            ( numberOfGaussPoints == 16 ) ) ) {
        numberOfGaussPoints = 4;
    }
}



Interface *
Quad1PlaneStrain :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    } else if ( interface == HuertaErrorEstimatorInterfaceType ) {
        return static_cast< HuertaErrorEstimatorInterface * >(this);
    }

    return NULL;
}


void
Quad1PlaneStrain :: HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                                     IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                                     HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                                     int &localNodeId, int &localElemId, int &localBcId,
                                                                     IntArray &controlNode, IntArray &controlDof,
                                                                     HuertaErrorEstimator :: AnalysisMode aMode)
{
    int nodes = 4, sides = 4;
    double x = 0.0, y = 0.0;

    static int sideNode [ 4 ] [ 2 ] = { { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 } };

    FloatArray corner [ 4 ], midSide [ 4 ], midNode, cor [ 4 ];
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


void Quad1PlaneStrain :: HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    computeNmatrixAt(gp->giveSubPatchCoordinates(), answer);
}

void
Quad1PlaneStrain :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(4);
    for ( int i = 1; i < 5; i++ ) {
        pap.at(i) = this->giveNode(i)->giveNumber();
    }
}


void
Quad1PlaneStrain :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    int found = 0;
    answer.resize(1);

    for ( int i = 1; i < 5; i++ ) {
        if ( pap == this->giveNode(i)->giveNumber() ) {
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
Quad1PlaneStrain :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return this->giveDefaultIntegrationRulePtr()->giveNumberOfIntegrationPoints();
}


SPRPatchType
Quad1PlaneStrain :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dxy;
}

} // end namespace oofem
