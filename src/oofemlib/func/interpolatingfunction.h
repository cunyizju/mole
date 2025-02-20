// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef interpolatingfunction_h
#define interpolatingfunction_h

#include "func/function.h"

///@name Input fields for InterpolatingFuction
//@{
#define _IFT_InterpolatingFuction_Name "interpolatingfunction"
#define _IFT_InterpolatingFuction_filename "name"
#define _IFT_InterpolatingFuction_dim "dim"
//@}

namespace oofem {
/**
 * This class implements a function which reads
 * an externally generated field interpolates
 */
class OOFEM_EXPORT InterpolatingFuction : public Function
{
protected:
    FloatArray field;
    IntArray numberReal;
    int dimension;

public:
    /// Constructor
    InterpolatingFuction(int n, Domain * d);
    /// Destructor
    virtual ~InterpolatingFuction();

    void evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp=nullptr, double param=0.) override;
    double evaluateAtTime(double t) override;
    double evaluateVelocityAtTime(double t) override { return 0.; }
    double evaluateAccelerationAtTime(double t) override { return 0.; }

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "InterpolatingFuction"; }
    const char *giveInputRecordName() const override { return _IFT_InterpolatingFuction_Name; }
};
} // end namespace oofem

#endif // interpolatingfunction_h
