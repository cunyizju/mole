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

#include "sm/Elements/Axisymmetry/q4axisymm.h"
#include "fei/fei2dquadquad.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "arrays/floatmatrix.h"
#include "arrays/floatarray.h"
#include "arrays/intarray.h"
#include "input/domain.h"
#include "engng/engngm.h"
#include "math/mathfem.h"
#include "cs/crosssection.h"
#include "classfactory.h"



namespace oofem {
REGISTER_Element(Q4Axisymm);

FEI2dQuadQuadAxi Q4Axisymm :: interp(1, 2);

Q4Axisymm :: Q4Axisymm(int n, Domain *aDomain) :
    AxisymElement(n, aDomain), ZZNodalRecoveryModelInterface(this)
{
    numberOfDofMans = 8;
    numberOfGaussPoints = 4;
    numberOfFiAndShGaussPoints = 1;
}


Q4Axisymm :: ~Q4Axisymm()
{ }


FEInterpolation *
Q4Axisymm :: giveInterpolation() const
{
    return & interp;
}



void
Q4Axisymm :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    StructuralElement :: initializeFrom(ir);

    numberOfFiAndShGaussPoints = 1;
    ///@todo only works for 1 //JB
    IR_GIVE_OPTIONAL_FIELD(ir, numberOfFiAndShGaussPoints, _IFT_Q4Axisymm_nipfish);
}


void
Q4Axisymm :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
{
    // Returns the [ 6 x (nno*2) ] strain-displacement matrix {B} of the receiver,
    // evaluated at gp. Uses reduced integration
    // (epsilon_x,epsilon_y,...,Gamma_xy) = B . r
    // r = ( u1,v1,u2,v2,u3,v3,u4,v4)

    if ( numberOfFiAndShGaussPoints == 1 ) { // Reduced integration
        FEInterpolation *interp = this->giveInterpolation();
        
        FloatArray N, NRed, redCoord = {0.0, 0.0}; // eval in centroid
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
}





Interface *
Q4Axisymm :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    }

    return NULL;
}

} // end namespace oofem
