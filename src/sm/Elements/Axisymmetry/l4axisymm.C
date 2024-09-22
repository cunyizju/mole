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

#include "sm/Elements/Axisymmetry/l4axisymm.h"
#include "fei/fei2dquadlin.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/domain.h"
#include "engng/engngm.h"
#include "math/mathfem.h"
#include "cs/crosssection.h"
#include "classfactory.h"


namespace oofem {
REGISTER_Element(L4Axisymm);

FEI2dQuadLinAxi L4Axisymm :: interpolation(1, 2);

L4Axisymm :: L4Axisymm(int n, Domain *aDomain) :
    AxisymElement(n, aDomain), ZZNodalRecoveryModelInterface(this), SpatialLocalizerInterface(this)
{
    numberOfDofMans  = 4;
    numberOfGaussPoints = 4;
    numberOfFiAndShGaussPoints = 1;
}


L4Axisymm :: ~L4Axisymm()
{ }


FEInterpolation *
L4Axisymm :: giveInterpolation() const { return & interpolation; }


Interface *
L4Axisymm :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    }

    return NULL;
}


void
L4Axisymm :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    NLStructuralElement :: initializeFrom(ir);

    if ( !( ( numberOfGaussPoints == 1 ) ||
           ( numberOfGaussPoints == 4 ) ||
           ( numberOfGaussPoints == 9 ) ||
           ( numberOfGaussPoints == 16 ) ) ) {
        numberOfGaussPoints = 4;
    }

    numberOfFiAndShGaussPoints = 1;
}



void
L4Axisymm :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
{
    // Returns the [ 6 x (nno*2) ] strain-displacement matrix {B} of the receiver,
    // evaluated at gp. Uses reduced integration.
    // (epsilon_x,epsilon_y,...,Gamma_xy) = B . r
    // r = ( u1,v1,u2,v2,u3,v3,u4,v4)

    FloatArray N, NRed, redCoord;
    if ( numberOfFiAndShGaussPoints == 1 ) { // Reduced integration
        redCoord  = {0.0, 0.0}; // eval in centroid
    } else {
        redCoord = gp->giveNaturalCoordinates();
    }


    FEInterpolation *interp = this->giveInterpolation();
        

    interp->evalN( N, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    interp->evalN( NRed, redCoord, FEIElementGeometryWrapper(this) );
    
    // Evaluate radius at center
    double r = 0.0;
    for ( int i = 1; i <= this->giveNumberOfDofManagers(); i++ ) {
        double x = this->giveNode(i)->giveCoordinate(1);
        r += x * NRed.at(i);
    } 
    
    FloatMatrix dNdx, dNdxRed;
    interp->evaldNdx( dNdx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    interp->evaldNdx( dNdxRed, redCoord, FEIElementGeometryWrapper(this) );
    answer.resize(6, dNdx.giveNumberOfRows() * 2);
    answer.zero();

    for ( int i = 1; i <= dNdx.giveNumberOfRows(); i++ ) {
        answer.at(1, i * 2 - 1) = dNdx.at(i, 1);
        answer.at(2, i * 2 - 0) = dNdx.at(i, 2);
        answer.at(3, i * 2 - 1) = NRed.at(i) / r;
        answer.at(6, 2 * i - 1) = dNdxRed.at(i, 2);
        answer.at(6, 2 * i - 0) = dNdxRed.at(i, 1);
    }
}

void
L4Axisymm :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(4);
    for ( int i = 1; i < 5; i++ ) {
        pap.at(i) = this->giveNode(i)->giveNumber();
    }
}

void
L4Axisymm :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
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
L4Axisymm :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return this->giveDefaultIntegrationRulePtr()->giveNumberOfIntegrationPoints();
}


SPRPatchType
L4Axisymm :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dxy;
}

} // end namespace oofem
