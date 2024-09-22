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
 *               Copyright (C) 1993 - 2014   Borek Patzak
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

#include "interfaceelem2dlin.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatmatrixf.h"
#include "math/floatarray.h"
#include "math/floatarrayf.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "fei/fei2dlinelin.h"
#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(InterfaceElem2dLin);

FEI2dLineLin InterfaceElem2dLin :: interp(1, 2);


InterfaceElem2dLin :: InterfaceElem2dLin(int n, Domain *aDomain) :
    StructuralElement(n, aDomain)
{
    numberOfDofMans       = 4;
    axisymmode            = false;
}


void
InterfaceElem2dLin :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
//
// Returns linear part of geometrical equations of the receiver at gp.
// Returns the linear part of the B matrix
//
{
    FloatArray n;
    this->interp.evalN (n, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this));

    answer.resize(2, 8);
    answer.zero();

    answer.at(1, 2) = answer.at(2, 1) = -n.at(1);
    answer.at(1, 4) = answer.at(2, 3) = -n.at(2);

    answer.at(1, 6) = answer.at(2, 5) = n.at(1);
    answer.at(1, 8) = answer.at(2, 7) = n.at(2);
}


void
InterfaceElem2dLin :: computeGaussPoints()
// Sets up the array of Gauss Points of the receiver.
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ] = std::make_unique<GaussIntegrationRule>(1, this, 1, 2);
        integrationRulesArray [ 0 ]->SetUpPointsOnLine(2, _2dInterface); 
    }
}


double
InterfaceElem2dLin :: computeVolumeAround(GaussPoint *gp)
// Returns the length of the receiver. This method is valid only if 1
// Gauss point is used.
{
    double r = 1.0;
    if (this->axisymmode) {
        FloatArray n(2);
        this->interp.evalN( n, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
        r = n.at(1)*this->giveNode(1)->giveCoordinate(1) + n.at(2)*this->giveNode(2)->giveCoordinate(1);
    }


    double result = this->interp.giveTransformationJacobian(gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this));
    return result * gp->giveWeight() * r;
}


void
InterfaceElem2dLin :: computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->giveEngTraction_2d(strain, gp, tStep);
}


void
InterfaceElem2dLin :: computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->give2dStiffnessMatrix_Eng(rMode, gp, tStep);
}


void
InterfaceElem2dLin :: initializeFrom(InputRecord &ir)
{
    this->axisymmode = ir.hasField(_IFT_InterfaceElem2dLin_axisymmode);
    StructuralElement :: initializeFrom(ir);
}


void
InterfaceElem2dLin :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = {D_u, D_v};
}


bool
InterfaceElem2dLin :: computeGtoLRotationMatrix(FloatMatrix &answer)
{
    FloatArray grad(2);

    // tangent
    grad.at(1) = this->giveNode(2)->giveCoordinate(1) - this->giveNode(1)->giveCoordinate(1);
    grad.at(2) = this->giveNode(2)->giveCoordinate(2) - this->giveNode(1)->giveCoordinate(2);
    grad.normalize();

    answer.resize(8, 8);
    for ( int i = 0; i < 4; i++ ) {
        answer.at(i * 2 + 1, i * 2 + 1) = grad.at(1);
        answer.at(i * 2 + 1, i * 2 + 2) = grad.at(2);
        answer.at(i * 2 + 2, i * 2 + 1) = -grad.at(2);
        answer.at(i * 2 + 2, i * 2 + 2) = grad.at(1);
    }

    return 1;
}


FEInterpolation *
InterfaceElem2dLin :: giveInterpolation() const
{
    return & interp;
}
} // end namespace oofem
