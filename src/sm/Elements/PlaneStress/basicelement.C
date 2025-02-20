// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStress/basicelement.h"
#include "fei/fei2dtrlin.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(BasicElement);

// Interpolator describing shape functions for the approximated unknowns.
// 1 -> first spatial index, 2 -> second spatial index
FEI2dTrLin BasicElement :: interp(1, 2);

BasicElement :: BasicElement(int n, Domain *aDomain) : PlaneStressElement(n, aDomain)
{
    this->numberOfGaussPoints = 1;
}


FEInterpolation *BasicElement :: giveInterpolation() const 
{ 
    /* Returns the interpolator used for the element which provide
     * shape functions, their derivatives, area of the element etc.
     */
    return & interp; 
}


} // end namespace oofem
