// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fields/maskedprimaryfield.h"

namespace oofem {
int
MaskedPrimaryField :: evaluateAt(FloatArray &answer, const FloatArray &coords,
                                 ValueModeType mode, TimeStep *tStep)
{
    return this->master->__evaluateAt(answer, coords, mode, tStep, & mask);
}

int
MaskedPrimaryField :: evaluateAt(FloatArray &answer, DofManager *dman,
                                 ValueModeType mode, TimeStep *tStep)
{
    return this->master->__evaluateAt(answer, dman, mode, tStep, & mask);
}
} // end namespace oofem
