// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef piecewiselinfunction_h
#define piecewiselinfunction_h

#include "math/floatarray.h"
#include "func/function.h"

///@name Input fields for PiecewiseLinFunction
//@{
#define _IFT_PiecewiseLinFunction_Name "piecewiselinfunction"
#define _IFT_PiecewiseLinFunction_t "t"
#define _IFT_PiecewiseLinFunction_ft "f(t)"
#define _IFT_PiecewiseLinFunction_dataFile "datafile"
//@}

namespace oofem {
/**
 * This class implements a piecewise linear function.
 * The function is defined by 'numberOfPoints' points. 'dates' and 'values'
 * store respectively the abscissas (t) and the values (f(t)) of the points
 */
class OOFEM_EXPORT PiecewiseLinFunction : public Function
{
protected:
    FloatArray dates;
    FloatArray values;

public:
    PiecewiseLinFunction(int i, Domain * d);
    virtual ~PiecewiseLinFunction() { }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    const char *giveClassName() const override { return "PiecewiseLinFunction"; }
    const char *giveInputRecordName() const override { return _IFT_PiecewiseLinFunction_Name; }

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    double evaluateAtTime(double t) override;
    double evaluateVelocityAtTime(double t) override;
    double evaluateAccelerationAtTime(double t) override { return 0.; }
};
} // end namespace oofem
#endif // piecewiselinfunction_h
