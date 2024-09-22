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

#include "sm/Elements/PlaneStrain/qtrplanestrain.h"
#include "fei/fei2dtrquad.h"
#include "arrays/floatmatrix.h"
#include "arrays/floatarray.h"
#include "arrays/intarray.h"
#include "classfactory.h"


namespace oofem {
REGISTER_Element(QTrPlaneStrain);

FEI2dTrQuad QTrPlaneStrain :: interpolation(1, 2);

QTrPlaneStrain :: QTrPlaneStrain(int n, Domain *aDomain) :
    PlaneStrainElement(n, aDomain), SpatialLocalizerInterface(this), ZZNodalRecoveryModelInterface(this)
{
    numberOfDofMans = 6;
    numberOfGaussPoints = 4;
}


FEInterpolation *
QTrPlaneStrain :: giveInterpolation() const { return & interpolation; }

Interface *
QTrPlaneStrain :: giveInterface(InterfaceType interface)
{
    //if (interface == NodalAveragingRecoveryModelInterfaceType) return (NodalAveragingRecoveryModelInterface*) this;
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
QTrPlaneStrain :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(3);
    pap.at(1) = this->giveNode(1)->giveNumber();
    pap.at(2) = this->giveNode(2)->giveNumber();
    pap.at(3) = this->giveNode(3)->giveNumber();
}

void
QTrPlaneStrain :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    answer.resize(3);
    if ( pap == this->giveNode(1)->giveNumber() ) {
        answer.at(1) = pap;
        answer.at(2) = this->giveNode(4)->giveNumber();
        answer.at(3) = this->giveNode(6)->giveNumber();
    } else if ( pap == this->giveNode(2)->giveNumber() ) {
        answer.at(1) = pap;
        answer.at(2) = this->giveNode(5)->giveNumber();
        answer.at(3) = this->giveNode(4)->giveNumber();
    } else if ( pap == this->giveNode(3)->giveNumber() ) {
        answer.at(1) = pap;
        answer.at(2) = this->giveNode(6)->giveNumber();
        answer.at(3) = this->giveNode(5)->giveNumber();
    } else {
        OOFEM_ERROR("node unknown");
    }
}

int
QTrPlaneStrain :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return numberOfGaussPoints;
}


SPRPatchType
QTrPlaneStrain :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dquadratic;
}

} // end namespace oofem
