// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef calculatorfunction_h
#define calculatorfunction_h

#include "func/function.h"

///@name Input fields for CalculatorFunction
//@{
#define _IFT_CalculatorFunction_Name "usrdefltf"
///@todo These aren't limited to just functions of "t" anymore; rename these.
#define _IFT_CalculatorFunction_f "f(t)"
#define _IFT_CalculatorFunction_dfdt "dfdt(t)"
#define _IFT_CalculatorFunction_d2fdt2 "d2fdt2(t)"
//@}

namespace oofem {
/**
 * Class representing user defined load time function. User input is function expression.
 * Uses Parser class to parse given expression. Slow but useful.
 * Load time function typically belongs to domain and is
 * attribute of one or more loads. Generally load time function is real function of time (@f$ y=f(t) @f$).
 */
class OOFEM_EXPORT CalculatorFunction : public Function
{
private:
    /// Expression for the function value.
    std :: string fExpression;
    /// Expression for first time derivative.
    std :: string dfdtExpression;
    /// Expression for second time derivative.
    std :: string d2fdt2Expression;

public:
    /**
     * Constructor. Creates load time function with given number, belonging to given domain.
     * @param n Load time function number.
     * @param d Domain to which new object will belongs..
     */
    CalculatorFunction(int n, Domain * d);
    /// Destructor.
    virtual ~CalculatorFunction() { }

    /**
     * Reads the fields
     * - f(t) (required)
     * - dfdt(t) (optional)
     * - d2fdt2(t) (optional)
     */
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &ir) override;

    void evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp=nullptr, double param=0.) override;
    double evaluateAtTime(double t) override;
    double evaluateVelocityAtTime(double t) override;
    double evaluateAccelerationAtTime(double t) override;

    const char *giveClassName() const override { return "CalculatorFunction"; }
    const char *giveInputRecordName() const override { return _IFT_CalculatorFunction_Name; }
};
} // end namespace oofem
#endif // calculatorfunction_h
