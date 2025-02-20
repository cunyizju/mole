// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStrain/qplanestrain.h"
#include "fei/fei2dquadquad.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(QPlaneStrain);

FEI2dQuadQuad QPlaneStrain :: interpolation(1, 2);

QPlaneStrain :: QPlaneStrain(int n, Domain *aDomain) :
    PlaneStrainElement(n, aDomain), ZZNodalRecoveryModelInterface(this)
{
    numberOfDofMans  = 8;
    numberOfGaussPoints = 4;
}

FEInterpolation *
QPlaneStrain :: giveInterpolation() const { return & interpolation; }

Interface *
QPlaneStrain :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    }

    return NULL;
}

} // end namespace oofem
