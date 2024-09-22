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

#include "sm/Elements/3D/lspace.h"
#include "fei/fei3dhexalin.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "arrays/floatmatrix.h"
#include "arrays/floatarray.h"
#include "arrays/intarray.h"
#include "input/domain.h"
#include "math/mathfem.h"
#include "cs/crosssection.h"
#include "classfactory.h"



namespace oofem {
REGISTER_Element(LSpace);

FEI3dHexaLin LSpace :: interpolation;

LSpace :: LSpace(int n, Domain *aDomain) : Structural3DElement(n, aDomain), ZZNodalRecoveryModelInterface(this),
    SPRNodalRecoveryModelInterface(), SpatialLocalizerInterface(this),
    HuertaErrorEstimatorInterface()
    // Constructor.
{
    numberOfDofMans  = 8;
    numberOfGaussPoints = 8;
}


FEInterpolation *LSpace :: giveInterpolation() const { return & interpolation; }

Interface *
LSpace :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == NodalAveragingRecoveryModelInterfaceType ) {
        return static_cast< NodalAveragingRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    } else if ( interface == HuertaErrorEstimatorInterfaceType ) {
        return static_cast< HuertaErrorEstimatorInterface * >(this);
    }

    return NULL;
}


void
LSpace :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 8;
    Structural3DElement :: initializeFrom(ir);
    this->reducedShearIntegration = ir.hasField(_IFT_LSpace_reducedShearIntegration);
}



void
LSpace :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
// Returns the [ 6 x (nno*3) ] strain-displacement matrix {B} of the receiver, eva-
// luated at gp.
// B matrix  -  6 rows : epsilon-X, epsilon-Y, epsilon-Z, gamma-YZ, gamma-ZX, gamma-XY  :
{
    FEInterpolation *interp = this->giveInterpolation();
    FloatMatrix dNdx, dNdxShear;
    interp->evaldNdx( dNdx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    if ( this->reducedShearIntegration ) {
        interp->evaldNdx( dNdxShear, { 0., 0., 0. }, FEIElementGeometryWrapper(this) );
    } else {
        dNdxShear =  dNdx;
    }

    answer.resize(6, dNdx.giveNumberOfRows() * 3);
    answer.zero();

    for ( int i = 1; i <= dNdx.giveNumberOfRows(); i++ ) {
        answer.at(1, 3 * i - 2) = dNdx.at(i, 1);
        answer.at(2, 3 * i - 1) = dNdx.at(i, 2);
        answer.at(3, 3 * i - 0) = dNdx.at(i, 3);

        answer.at(5, 3 * i - 2) = answer.at(4, 3 * i - 1) = dNdxShear.at(i, 3);
        answer.at(6, 3 * i - 2) = answer.at(4, 3 * i - 0) = dNdxShear.at(i, 2);
        answer.at(6, 3 * i - 1) = answer.at(5, 3 * i - 0) = dNdxShear.at(i, 1);
    }
}




void
LSpace :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(numberOfDofMans);
    for ( int i = 1; i <= numberOfDofMans; i++ ) {
        pap.at(i) = this->giveNode(i)->giveNumber();
    }
}


void
LSpace :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    int found = 0;
    answer.resize(1);

    for ( int i = 1; i <= numberOfDofMans; i++ ) {
        if ( this->giveNode(i)->giveNumber() == pap ) {
            found = 1;
        }
    }

    if ( found ) {
        answer.at(1) = pap;
    } else {
        OOFEM_ERROR("unknown node number %d", pap);
    }
}


int
LSpace :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return this->giveDefaultIntegrationRulePtr()->giveNumberOfIntegrationPoints();
}


SPRPatchType
LSpace :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_3dBiLin;
}


void
LSpace :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                     InternalStateType type, TimeStep *tStep)
{
    double x1 = 0.0, x2 = 0.0, x3 = 0.0, y = 0.0;
    FloatMatrix A(4, 4);
    FloatMatrix b, r;
    FloatArray val;
    double u, v, w;

    int size = 0;

    for ( GaussPoint *gp : *integrationRulesArray [ 0 ] ) {
        giveIPValue(val, gp, type, tStep);
        if ( size == 0 ) {
            size = val.giveSize();
            b.resize(4, size);
            r.resize(4, size);
            A.zero();
            r.zero();
        }

        const FloatArray &coord = gp->giveNaturalCoordinates();
        u = coord.at(1);
        v = coord.at(2);
        w = coord.at(3);

        A.at(1, 1) += 1;
        A.at(1, 2) += u;
        A.at(1, 3) += v;
        A.at(1, 4) += w;
        A.at(2, 1) += u;
        A.at(2, 2) += u * u;
        A.at(2, 3) += u * v;
        A.at(2, 4) += u * w;
        A.at(3, 1) += v;
        A.at(3, 2) += v * u;
        A.at(3, 3) += v * v;
        A.at(3, 4) += v * w;
        A.at(4, 1) += w;
        A.at(4, 2) += w * u;
        A.at(4, 3) += w * v;
        A.at(4, 4) += w * w;

        for ( int j = 1; j <= size; j++ ) {
            y = val.at(j);
            r.at(1, j) += y;
            r.at(2, j) += y * u;
            r.at(3, j) += y * v;
            r.at(4, j) += y * w;
        }
    }

    A.solveForRhs(r, b);

    switch ( node ) {
    case 1:
        x1 =  1.0;
        x2 =  1.0;
        x3 =  1.0;
        break;
    case 2:
        x1 = -1.0;
        x2 =  1.0;
        x3 =  1.0;
        break;
    case 3:
        x1 = -1.0;
        x2 = -1.0;
        x3 =  1.0;
        break;
    case 4:
        x1 =  1.0;
        x2 = -1.0;
        x3 =  1.0;
        break;
    case 5:
        x1 =  1.0;
        x2 =  1.0;
        x3 = -1.0;
        break;
    case 6:
        x1 = -1.0;
        x2 =  1.0;
        x3 = -1.0;
        break;
    case 7:
        x1 = -1.0;
        x2 = -1.0;
        x3 = -1.0;
        break;
    case 8:
        x1 =  1.0;
        x2 = -1.0;
        x3 = -1.0;
        break;
    default:
        OOFEM_ERROR("unsupported node");
    }

    answer.resize(size);
    for ( int j = 1; j <= size; j++ ) {
        answer.at(j) = b.at(1, j) + x1 *b.at(2, j) * x2 * b.at(3, j) * x3 * b.at(4, j);
    }
}


void
LSpace :: HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                           IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                           HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                           int &localNodeId, int &localElemId, int &localBcId,
                                                           IntArray &controlNode, IntArray &controlDof,
                                                           HuertaErrorEstimator :: AnalysisMode aMode)
{
    double x = 0.0, y = 0.0, z = 0.0;
    int nodes = 8, sides = 12, faces = 6;

    static int sideNode [ 12 ] [ 2 ] = { { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 }, // bottom
                                         { 5, 6 }, { 6, 7 }, { 7, 8 }, { 8, 5 }, // top
                                         { 1, 5 }, { 2, 6 }, { 3, 7 }, { 4, 8 } }; // vertical
    static int faceNode [ 6 ] [ 4 ] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, // bottom, top
                                        { 1, 2, 6, 5 }, { 2, 3, 7, 6 }, { 3, 4, 8, 7 }, { 4, 1, 5, 8 } }; // side

    /* ordering of side and face hexa nodes must be compatible with refinedElement connectivity ordering;
     * generally the ordering is: corner side side face side face face center;
     * however the concrete ordering is element dependent (see refineMeshGlobally source if in doubts) */

    static int hexaSideNode [ 8 ] [ 3 ] = { { 1, 4, 9 }, { 2, 1, 10 }, { 3, 2, 11 }, { 4, 3, 12 },
                                            { 8, 5, 9 }, { 5, 6, 10 }, { 6, 7, 11 }, { 7, 8, 12 } };
    static int hexaFaceNode [ 8 ] [ 3 ] = { { 1, 3, 6 }, { 1, 4, 3 }, { 1, 5, 4 }, { 1, 6, 5 },
                                            { 2, 6, 3 }, { 2, 3, 4 }, { 2, 4, 5 }, { 2, 5, 6 } };

    FloatArray corner [ 8 ], midSide [ 12 ], midFace [ 6 ], midNode;
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
            for ( int inode = 0; inode < 4; inode++ ) {
                int nd = faceNode [ iface ] [ inode ] - 1;
                x += corner [ nd ].at(1);
                y += corner [ nd ].at(2);
                z += corner [ nd ].at(3);
            }

            midFace [ iface ].resize(3);

            midFace [ iface ].at(1) = x / 4;
            midFace [ iface ].at(2) = y / 4;
            midFace [ iface ].at(3) = z / 4;
        }
    }

    this->setupRefinedElementProblem3D(this, refinedElement, level, nodeId, localNodeIdArray, globalNodeIdArray,
                                       sMode, tStep, nodes, corner, midSide, midFace, midNode,
                                       localNodeId, localElemId, localBcId, hexaSideNode, hexaFaceNode,
                                       controlNode, controlDof, aMode, "LSpace");
}


void LSpace :: HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    computeNmatrixAt(gp->giveSubPatchCoordinates(), answer);
}



int
LSpace :: computeLoadLSToLRotationMatrix(FloatMatrix &answer, int isurf, GaussPoint *gp)
{
    // returns transformation matrix from
    // surface local coordinate system
    // to element local c.s
    // (same as global c.s in this case)
    //
    // i.e. f(element local) = T * f(edge local)

    // definition of local c.s on surface:
    // local z axis - perpendicular to surface, pointing outwards from element
    // local x axis - is in global xy plane (perpendicular to global z axis)
    // local y axis - completes the righ hand side cs.

    /*
     * OOFEM_ERROR("surface local coordinate system not supported");
     * return 1;
     */
    FloatArray gc(3);
    FloatArray h1(3), h2(3), nn(3), n(3);

    answer.resize(3, 3);

    const auto &snodes = this->interpolation.computeSurfaceMapping(dofManArray, isurf);
    for ( int i = 1; i <= 4; i++ ) {
        gc.add( domain->giveNode( snodes.at(i) )->giveCoordinates() );
    }

    gc.times(1. / 4.);
    // determine "average normal"
    for ( int i = 1; i <= 4; i++ ) {
        int j = ( i ) % 4 + 1;
        h1 = domain->giveNode( snodes.at(i) )->giveCoordinates();
        h1.subtract(gc);
        h1.normalize();
        h2 = domain->giveNode( snodes.at(j) )->giveCoordinates();
        h2.subtract(gc);
        h2.normalize();	
        n.beVectorProductOf(h1, h2);

        nn.add(n);
    }

    nn.times(1. / 4.);
    if ( nn.computeSquaredNorm() < 1.e-6 ) {
        answer.zero();
        return 1;
    }

    nn.normalize();
    for ( int i = 1; i <= 3; i++ ) {
        answer.at(i, 3) = nn.at(i);
    }

    // determine lcs of surface
    // local x axis in xy plane
    double test = fabs(fabs( nn.at(3) ) - 1.0);
    if ( test < 1.e-5 ) {
        h1.at(1) = answer.at(1, 1) = 1.0;
        h1.at(2) = answer.at(2, 1) = 0.0;
    } else {
        h1.at(1) = answer.at(1, 1) = answer.at(2, 3);
        h1.at(2) = answer.at(2, 1) = -answer.at(1, 3);
    }

    h1.at(3) = answer.at(3, 1) = 0.0;
    // local y axis perpendicular to local x,z axes
    h2.beVectorProductOf(nn, h1);
    for ( int i = 1; i <= 3; i++ ) {
        answer.at(i, 2) = h2.at(i);
    }

    return 1;
}
} // end namespace oofem
