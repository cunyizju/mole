// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/constantedgeload.h"
#include "func/function.h"
#include "math/floatarray.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(ConstantEdgeLoad);

void
ConstantEdgeLoad :: initializeFrom(InputRecord &ir)
{
    BoundaryLoad :: initializeFrom(ir);
}

void
ConstantEdgeLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    if ( ( mode != VM_Total ) && ( mode != VM_Incremental ) ) {
        OOFEM_ERROR("mode not supported");
    }

    if ( !isImposed(tStep) ) {
        answer.zero();
    } else {
        double factor = this->giveTimeFunction()->evaluate(tStep, mode);
        answer.beScaled(factor, componentArray);
    }
}
} // end namespace oofem
