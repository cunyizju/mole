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

#include "sm/Elements/PlaneStress/planstrss.h"
#include "sm/Materials/structuralms.h"
#include "fei/fei2dquadlin.h"
#include "dofman/node.h"
#include "cs/crosssection.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/domain.h"
#include "math/mathfem.h"
#include "strainvector.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(PlaneStress2d);

FEI2dQuadLin PlaneStress2d :: interpolation(1, 2);

PlaneStress2d :: PlaneStress2d(int n, Domain *aDomain) :
    PlaneStressElement(n, aDomain), ZZNodalRecoveryModelInterface(this),
    SPRNodalRecoveryModelInterface(), SpatialLocalizerInterface(this),
    HuertaErrorEstimatorInterface()
    // Constructor.
{
    numberOfDofMans  = 4;
    numberOfGaussPoints = 4;
}

PlaneStress2d :: ~PlaneStress2d()
// Destructor
{ }

FEInterpolation *PlaneStress2d :: giveInterpolation() const { return & interpolation; }

void
PlaneStress2d :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
//
// Returns the [3x8] strain-displacement matrix {B} of the receiver,
// evaluated at gp.
// (epsilon_x,epsilon_y,gamma_xy) = B . r
// r = ( u1,v1,u2,v2,u3,v3,u4,v4)
{
    FloatMatrix dnx;

    this->interpolation.evaldNdx( dnx, gp->giveNaturalCoordinates(), *this->giveCellGeometryWrapper() );

    answer.resize(3, 8);
    answer.zero();

    for ( int i = 1; i <= 4; i++ ) {
        answer.at(1, 2 * i - 1) = dnx.at(i, 1);
        answer.at(2, 2 * i - 0) = dnx.at(i, 2);
    }

#ifdef  PlaneStress2d_reducedShearIntegration
    this->interpolation.evaldNdx( dnx, {0., 0.}, *this->giveCellGeometryWrapper() );
#endif

    for ( int i = 1; i <= 4; i++ ) {
        answer.at(3, 2 * i - 1) = dnx.at(i, 2);
        answer.at(3, 2 * i - 0) = dnx.at(i, 1);
    }
}


void
PlaneStress2d :: computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer)
//
// Returns the [4x8] displacement gradient matrix {BH} of the receiver,
// evaluated at gp.
// @todo not checked if correct
{
    FloatMatrix dnx;

    this->interpolation.evaldNdx( dnx, gp->giveNaturalCoordinates(), *this->giveCellGeometryWrapper() );

    answer.resize(4, 8);

    for ( int i = 1; i <= 4; i++ ) {
        answer.at(1, 2 * i - 1) = dnx.at(i, 1);     // du/dx -1
        answer.at(2, 2 * i - 0) = dnx.at(i, 2);     // dv/dy -2
    }

#ifdef  PlaneStress2d_reducedShearIntegration
    this->interpolation.evaldNdx( dnx, {0., 0.}, *this->giveCellGeometryWrapper() );
#endif

    for ( int i = 1; i <= 4; i++ ) {
        answer.at(3, 2 * i - 1) = dnx.at(i, 2);     // du/dy -6
        answer.at(4, 2 * i - 0) = dnx.at(i, 1);     // dv/dx -9
    }
}



void
PlaneStress2d :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    PlaneStressElement :: initializeFrom(ir);

    if ( numberOfGaussPoints != 1 && numberOfGaussPoints != 4 && numberOfGaussPoints != 9 && numberOfGaussPoints != 16 && numberOfGaussPoints != 25 ) {
        numberOfGaussPoints = 4;
        OOFEM_WARNING("Number of Gauss points enforced to 4");
    }
}




double
PlaneStress2d :: giveCharacteristicSize(GaussPoint *gp, FloatArray &normalToCrackPlane, ElementCharSizeMethod method)
//
// returns receiver's characteristic size at gp (for some material models)
// for crack formed in plane with normal normalToCrackPlane
// using the selected method
//
{
    if ( method == ECSM_SquareRootOfArea ) {
        // square root of element area
        return this->computeMeanSize();
    }

    if ( method == ECSM_Projection ) {
        // standard projection method
        return this->giveCharacteristicLength(normalToCrackPlane);
    }

    // evaluate average strain and its maximum principal direction
    FloatArray sumstrain, averageNormal;
    for ( GaussPoint *gpi: *this->giveDefaultIntegrationRulePtr() ) {
        StructuralMaterialStatus *matstatus = dynamic_cast< StructuralMaterialStatus * >( gpi->giveMaterialStatus() );
        if ( matstatus ) {
            sumstrain.add( matstatus->giveTempStrainVector() );
        }
    }

    StrainVector sumstrainvec(sumstrain, _PlaneStress);
    sumstrainvec.computeMaxPrincipalDir(averageNormal);

    if ( method == ECSM_ProjectionCentered ) {
        // projection method based on principal direction of average strain
        normalToCrackPlane = averageNormal;
        return this->giveLengthInDir(normalToCrackPlane);
    }

    if ( method == ECSM_Oliver1 || method == ECSM_Oliver1modified ) {
        // method based on derivative of auxiliary function phi at each Gauss point
        // in the maximum principal strain direction determined at
        // ECSM_Oliver1 ... at each Gauss point
        // ECSM_Oliver1modified ... at element center (from average strain)

        // coordinates of the element center
        FloatArray center(2);
        double cx = 0., cy = 0.;
        for ( int i = 1; i <= 4; i++ ) {
            cx += giveNode(i)->giveCoordinate(1);
            cy += giveNode(i)->giveCoordinate(2);
        }

        cx /= 4.;
        cy /= 4.;

        // nodal values of function phi (0 or 1)
        FloatArray phi(4);
        for ( int i = 1; i <= 4; i++ ) {
            if ( ( ( giveNode(i)->giveCoordinate(1) - cx ) * averageNormal.at(1) + ( giveNode(i)->giveCoordinate(2) - cy ) * averageNormal.at(2) ) > 0. ) {
                phi.at(i) = 1.;
            } else {
                phi.at(i) = 0.;
            }
        }

        // gradient of function phi at the current GP
        FloatMatrix dnx;
        this->interpolation.evaldNdx( dnx, gp->giveNaturalCoordinates(), *this->giveCellGeometryWrapper() );
        FloatArray gradPhi(2);
        gradPhi.zero();
        for ( int i = 1; i <= 4; i++ ) {
            gradPhi.at(1) += phi.at(i) * dnx.at(i, 1);
            gradPhi.at(2) += phi.at(i) * dnx.at(i, 2);
        }

        // scalar product of the gradient with crack normal (at GP)
        double dPhidN = 0.;
        if ( method == ECSM_Oliver1modified ) {
            normalToCrackPlane = averageNormal;
        }

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



Interface *
PlaneStress2d :: giveInterface(InterfaceType interface)
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
PlaneStress2d :: HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
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
                                       controlNode, controlDof, aMode, "PlaneStress2d");
}

void PlaneStress2d :: HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    computeNmatrixAt(gp->giveSubPatchCoordinates(), answer);
}

void
PlaneStress2d :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(4);
    for ( int i = 1; i < 5; i++ ) {
        pap.at(i) = this->giveNode(i)->giveNumber();
    }
}

void
PlaneStress2d :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
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
PlaneStress2d :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return this->giveDefaultIntegrationRulePtr()->giveNumberOfIntegrationPoints();
}


SPRPatchType
PlaneStress2d :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dxy;
}

} // end namespace oofem
