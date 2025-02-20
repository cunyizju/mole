// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "structtemperatureload.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(StructuralTemperatureLoad);

void StructuralTemperatureLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    this->computeComponentArrayAt(answer, tStep, mode);
}
}
