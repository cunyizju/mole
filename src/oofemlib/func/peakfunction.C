// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "func/peakfunction.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Function(PeakFunction);

double
PeakFunction :: evaluateAtTime(double time)
// Returns the value of the receiver at time 'time'.
{
    double precision = 1e-6;

    if ( fabs(t - time) < precision ) {
        return value;
    } else {
        return 0.;
    }
}

void
PeakFunction :: initializeFrom(InputRecord &ir)
{
    Function :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, t, _IFT_PeakFunction_t);
    IR_GIVE_FIELD(ir, value, _IFT_PeakFunction_ft);
}
} // end namespace oofem
