// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/pointload.h"
#include "func/function.h"
#include "math/floatarray.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_BoundaryCondition(PointLoad);

void
PointLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    double factor;
    // returns component array for elements which use direct formulae
    Load :: computeComponentArrayAt(answer, tStep, mode);

    // time distribution
    factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer.times(factor);
}

void
PointLoad :: initializeFrom(InputRecord &ir)
{
    Load :: initializeFrom(ir);

    int dummy;
    IR_GIVE_FIELD(ir, dummy, "ndofs");
    IR_GIVE_FIELD(ir, coords, _IFT_PointLoad_coords);

    int value = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, value, _IFT_PointLoad_loadtype);
    lType = ( bcType ) value;

    value = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, value, _IFT_PointLoad_cstype);
    coordSystemType = ( CoordSystType ) value;
}


void
PointLoad :: giveInputRecord(DynamicInputRecord &input)
{
    Load :: giveInputRecord(input);
    input.setField(this->lType, _IFT_PointLoad_loadtype);
    input.setField(this->coordSystemType, _IFT_PointLoad_cstype);
    input.setField(this->coords, _IFT_PointLoad_coords);
}
} // end namespace oofem
