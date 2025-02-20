// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef periodicpiecewiselinfunction_h
#define periodicpiecewiselinfunction_h

#include "math/floatarray.h"
#include "func/piecewiselinfunction.h"

///@name Input fields for PeriodicPiecewiseLinFunction
//@{
#define _IFT_PeriodicPiecewiseLinFunction_Name "periodicpiecewiselinfunction"
#define _IFT_PeriodicPiecewiseLinFunction_period "period"
#define _IFT_PeriodicPiecewiseLinFunction_addtf "addtf"
//@}

namespace oofem {
/**
 * This class implements an enhanced piecewise linear function with periodicity.
 * and possibility to add another arbitrary time function.
 *
 * The function is defined by 'numberOfPoints' points. 'dates' and 'values'
 * store respectively the abscissas (t) and the values (f(t)) of the points
 * The values are repeated after 'period'. 'AddTF' parameter specifies number
 * of function to add.
 */
class OOFEM_EXPORT PeriodicPiecewiseLinFunction : public PiecewiseLinFunction
{
private:
    /// If nonzero, the value of time function specified by addTF is added to computed value.
    int addTF;
    /**
     * If less than zero no periodicity, if >=0 date time is computed as
     * given time%period.
     * If points span more than period, span of LAST period is repeated
     */
    double period;

public:
    PeriodicPiecewiseLinFunction(int i, Domain * d) : PiecewiseLinFunction(i, d)
    {
        period = -1.0;
        addTF = 0;
    }
    virtual ~PeriodicPiecewiseLinFunction() { }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    const char *giveClassName() const override { return "PeriodicPiecewiseClass"; }
    const char *giveInputRecordName() const override { return _IFT_PeriodicPiecewiseLinFunction_Name; }

    double evaluateAtTime(double) override;
    double evaluateVelocityAtTime(double) override;
    double evaluateAccelerationAtTime(double) override { return 0.; }
};
} // end namespace oofem
#endif // periodicpiecewiselinfunction_h
