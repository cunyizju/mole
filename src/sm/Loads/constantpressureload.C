// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "constantpressureload.h"
#include "input/dynamicinputrecord.h"
#include "func/function.h"
#include "math/floatarray.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(ConstantPressureLoad);

ConstantPressureLoad :: ConstantPressureLoad(int i, Domain *d) : SurfaceLoad(i, d)
{
    this->loadOffset = 0.0;
}

void
ConstantPressureLoad :: initializeFrom(InputRecord &ir)
{
    BoundaryLoad :: initializeFrom(ir);

    IR_GIVE_OPTIONAL_FIELD(ir, this->loadOffset, _IFT_ConstantPressureLoad_LoadOffset);
#if 1
    if ( ir.hasField(_IFT_GeneralBoundaryCondition_dofs) ) {
        throw ValueInputException(ir, _IFT_GeneralBoundaryCondition_dofs, "Constant pressure load should not specify DOFs");
    }
#endif
}


void
ConstantPressureLoad :: giveInputRecord(DynamicInputRecord &input)
{
    BoundaryLoad :: giveInputRecord(input);
    input.setField(this->loadOffset, _IFT_ConstantPressureLoad_LoadOffset);
}

void
ConstantPressureLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    // we overload general implementation on the boundary load level due
    // to implementation efficiency

    if ( ( mode != VM_Total ) && ( mode != VM_Incremental ) ) {
        OOFEM_ERROR("mode not supported");
    }

    // ask time distribution
    double factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer = componentArray;
    answer.times(factor);
}

void
ConstantPressureLoad :: computeValues(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, const IntArray &dofids, ValueModeType mode)
{
    OOFEM_ERROR("Should not be called for constant pressure loads");
}

} // end namespace oofem
