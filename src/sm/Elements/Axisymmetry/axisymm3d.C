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

#include "sm/Elements/Axisymmetry/axisymm3d.h"
#include "fei/fei2dtrlin.h"
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
REGISTER_Element(Axisymm3d);

FEI2dTrLinAxi Axisymm3d :: interpolation(1, 2);

Axisymm3d :: Axisymm3d(int n, Domain *aDomain) :
    AxisymElement(n, aDomain), ZZNodalRecoveryModelInterface(this), NodalAveragingRecoveryModelInterface(),
    SPRNodalRecoveryModelInterface(), SpatialLocalizerInterface(this)
    // Constructor.
{
    numberOfDofMans = 3;
    area = -1;
    numberOfGaussPoints = 1;
}

Axisymm3d :: ~Axisymm3d()
// destructor
{ }


FEInterpolation *
Axisymm3d :: giveInterpolation() const { return & interpolation; }


Interface *
Axisymm3d :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == NodalAveragingRecoveryModelInterfaceType ) {
        return static_cast< NodalAveragingRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    }

    return NULL;
}


double
Axisymm3d :: giveArea()
// returns the area occupied by the receiver
{
    if ( area > 0 ) {
        return area;         // check if previously computed
    }

    area = fabs( this->interpolation.giveArea( FEIElementGeometryWrapper(this) ) );
    return area;
}


void
Axisymm3d :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 1;
    StructuralElement :: initializeFrom(ir);

    if ( !( ( numberOfGaussPoints == 1 ) ||
           ( numberOfGaussPoints == 4 ) ||
           ( numberOfGaussPoints == 7 ) ) ) {
        numberOfGaussPoints = 1;
    }
}





void
Axisymm3d :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                        InternalStateType type, TimeStep *tStep)
{
    GaussPoint *gp;

    if ( numberOfGaussPoints != 1 ) {
        answer.clear(); // for more gp's need to be refined
        return;
    }

    gp = integrationRulesArray [ 0 ]->getIntegrationPoint(0);
    this->giveIPValue(answer, gp, type, tStep);
}

void
Axisymm3d :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(3);
    pap.at(1) = this->giveNode(1)->giveNumber();
    pap.at(2) = this->giveNode(2)->giveNumber();
    pap.at(3) = this->giveNode(3)->giveNumber();
}

void
Axisymm3d :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
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
Axisymm3d :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return this->giveDefaultIntegrationRulePtr()->giveNumberOfIntegrationPoints();
}


SPRPatchType
Axisymm3d :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dxy;
}

} // end namespace oofem
