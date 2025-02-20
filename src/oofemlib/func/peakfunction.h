// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef peakfunction_h
#define peakfunction_h

#include "func/function.h"

///@name Input fields for PeakFunction
//@{
#define _IFT_PeakFunction_Name "peakfunction"
#define _IFT_PeakFunction_t "t"
#define _IFT_PeakFunction_ft "f(t)"
//@}

namespace oofem {
/**
 * This class implements a function that is 0 everywhere, except in a single
 * point.
 */
class OOFEM_EXPORT PeakFunction : public Function
{
private:
    /// Specific time when function is nonzero.
    double t;
    /// Value of function at nonzero time.
    double value;

public:
    PeakFunction(int i, Domain * d) : Function(i, d)
    {
        t = 0.0;
        value = 0.0;
    }
    virtual ~PeakFunction() { }

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "PeakFunction"; }
    const char *giveInputRecordName() const override { return _IFT_PeakFunction_Name; }

    double evaluateAtTime(double) override;
    double evaluateVelocityAtTime(double t) override { return 0.; }
    double evaluateAccelerationAtTime(double t) override { return 0.; }
};
} // end namespace oofem
#endif // peakfunction_h
