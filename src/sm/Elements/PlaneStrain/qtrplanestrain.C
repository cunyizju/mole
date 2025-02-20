// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStrain/qtrplanestrain.h"
#include "fei/fei2dtrquad.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "engng/classfactory.h"


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
