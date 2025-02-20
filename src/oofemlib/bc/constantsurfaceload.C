// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/constantsurfaceload.h"
#include "input/dynamicinputrecord.h"
#include "func/function.h"
#include "math/floatarray.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(ConstantSurfaceLoad);

ConstantSurfaceLoad :: ConstantSurfaceLoad(int i, Domain *d) : SurfaceLoad(i, d)
{
    this->loadOffset = 0.0;
}

void
ConstantSurfaceLoad :: initializeFrom(InputRecord &ir)
{
    SurfaceLoad :: initializeFrom(ir);

    IR_GIVE_OPTIONAL_FIELD(ir, this->loadOffset, _IFT_ConstantSurfaceLoad_LoadOffset);
}

void
ConstantSurfaceLoad :: giveInputRecord(DynamicInputRecord &input)
{
    SurfaceLoad :: giveInputRecord(input);
    input.setField(this->loadOffset, _IFT_ConstantSurfaceLoad_LoadOffset);
}

void
ConstantSurfaceLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    if ( ( mode != VM_Total ) && ( mode != VM_Incremental ) ) {
        OOFEM_ERROR("mode not supported");
    }

    double factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer.beScaled(factor, componentArray);
}
} // end namespace oofem
