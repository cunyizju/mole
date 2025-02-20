// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStress/qtrplstr.h"
#include "fei/fei2dtrquad.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "math/gaussintegrationrule.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(QTrPlaneStress2d);

FEI2dTrQuad QTrPlaneStress2d :: interpolation(1, 2);

QTrPlaneStress2d :: QTrPlaneStress2d(int n, Domain *aDomain) :
    PlaneStressElement(n, aDomain), SpatialLocalizerInterface(this)
{
    numberOfDofMans  = 6;
    numberOfGaussPoints = 4;
}


FEInterpolation *QTrPlaneStress2d :: giveInterpolation() const { return & interpolation; }


Interface *
QTrPlaneStress2d :: giveInterface(InterfaceType interface)
{
    /*
     * Note ZZNodalRecoveryModelInterface disabled, as the
     * sum of row entries is zero for (N^T)N matrix for vertices,
     * yielding zero entries in lumped form.
     *
     * if ( interface == ZZNodalRecoveryModelInterfaceType ) {
     *    return static_cast< ZZNodalRecoveryModelInterface * >( this );
     */
    if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    }

    return NULL;
}


void
QTrPlaneStress2d :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    PlaneStressElement :: initializeFrom(ir);
}

void
QTrPlaneStress2d :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(3);
    pap.at(1) = this->giveNode(1)->giveNumber();
    pap.at(2) = this->giveNode(2)->giveNumber();
    pap.at(3) = this->giveNode(3)->giveNumber();
}


void
QTrPlaneStress2d :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
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
QTrPlaneStress2d :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return numberOfGaussPoints;
}


SPRPatchType
QTrPlaneStress2d :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_2dquadratic;
}

} // end namespace oofem
