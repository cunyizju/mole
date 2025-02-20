// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/deadweight.h"
#include "solvers/timestep.h"
#include "func/function.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(DeadWeight);

void DeadWeight :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    computeComponentArrayAt(answer, tStep, mode);
}

void DeadWeight :: setDeadWeighComponents(FloatArray newComponents)
{
    this->componentArray = std :: move(newComponents);
}
} // end namespace oofem
