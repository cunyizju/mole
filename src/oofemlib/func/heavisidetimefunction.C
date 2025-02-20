// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "func/heavisidetimefunction.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Function(HeavisideTimeFunction);

double
HeavisideTimeFunction :: evaluateAtTime(double time)
{
    double relTime = time - this->origin;
    if ( relTime <= 0. ) {
        return 0.;
    }

    return value;
}


void
HeavisideTimeFunction :: initializeFrom(InputRecord &ir)
{
    Function :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, origin, _IFT_HeavisideTimeFunction_origin);
    IR_GIVE_FIELD(ir, value, _IFT_HeavisideTimeFunction_value);
}


void HeavisideTimeFunction :: giveInputRecord(DynamicInputRecord &input)
{
    Function :: giveInputRecord(input);
    input.setField(this->origin, _IFT_HeavisideTimeFunction_origin);
    input.setField(this->value, _IFT_HeavisideTimeFunction_value);
}
} // end namespace oofem
