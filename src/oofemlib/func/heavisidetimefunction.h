// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef heavisidetimefunction_h
#define heavisidetimefunction_h

#include "math/floatarray.h"
#include "func/function.h"

///@name Input fields for HeavisideTimeFunction
//@{
#define _IFT_HeavisideTimeFunction_Name "heavisideltf"
#define _IFT_HeavisideTimeFunction_origin "origin"
#define _IFT_HeavisideTimeFunction_value "value"
//@}

namespace oofem {
/**
 * This class implements a Heaviside step load time function.
 *
 * The function is defined by the origin of step and value.
 * The result is value*H(t-origin),
 * where
 * @f[
 * H(t) = \begin{cases} 0,& t\leq 0  \\ 1, & t>0 \end{cases}
 * @f]
 */
class OOFEM_EXPORT HeavisideTimeFunction : public Function
{
private:
    double origin, value;

public:
    HeavisideTimeFunction(int i, Domain * d) : Function(i, d)
    {
        origin = value = 0.;
    }
    virtual ~HeavisideTimeFunction() { }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    const char *giveClassName() const override { return "HeavisideTimeFunction"; }
    const char *giveInputRecordName() const override { return _IFT_HeavisideTimeFunction_Name; }

    double evaluateAtTime(double) override;
    double evaluateVelocityAtTime(double t) override { return 0.; }
    double evaluateAccelerationAtTime(double t) override { return 0.; }
};
} // end namespace oofem
#endif // heavisidetimefunction_h
