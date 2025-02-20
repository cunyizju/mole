// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef localgaussianrandomfunction_h
#define localgaussianrandomfunction_h

#include "func/function.h"

///@name Input fields for LocalGaussianRandomFunction
//@{
#define _IFT_LocalGaussianRandomFunction_Name "localgaussrandomfunction"
#define _IFT_LocalGaussianRandomFunction_mean "mean"
#define _IFT_LocalGaussianRandomFunction_variance "variance"
#define _IFT_LocalGaussianRandomFunction_seed "seed"
//@}

namespace oofem {
/**
 * This class implements a local (no spatial correlation) random function using Gaussian distribution.
 * @author Peter Grassl
 */
class OOFEM_EXPORT LocalGaussianRandomFunction : public Function
{
protected:
    /// Integer which is the input of the pseudo-random number generator.
    long randomInteger;
    /// Gauss distribution parameters.
    double mean, variance;

public:
    /// Constructor.
    LocalGaussianRandomFunction(int n, Domain * d);
    /// Destructor
    virtual ~LocalGaussianRandomFunction();

    void evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp=nullptr, double param=0.) override;
    double evaluateAtTime(double t) override;
    double evaluateVelocityAtTime(double t) override;
    double evaluateAccelerationAtTime(double t) override;

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "LocalGaussianRandomFunction"; }
    const char *giveInputRecordName() const override { return _IFT_LocalGaussianRandomFunction_Name; }

protected:
    /**
     * Computes pseudo-random numbers.
     * @param idum Pointer to start integer (must be negative).
     * @return Random number between 0 and 1.
     */
    double ran1(long *idum);

    /**
     * Computes the inverse of the Gaussian CDF
     * @param cdf Input probability.
     * @param a Mean.
     * @param b Standard deviation.
     * @return Inverse.
     */
    double normalCdfInverse(double cdf, double a, double b);

    /**
     * Computes the inverse of the normal distribution.
     * @param p Input probability.
     * @return Inverse.
     */
    double normal01CdfInverse(double p);

    double dpolyValue(int n, double a[], double x);
};
} // end namespace oofem
#endif
