// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef constantfunction_h
#define constantfunction_h

#include "func/function.h"

#define _IFT_ConstantFunction_Name "constantfunction"
#define _IFT_ConstantFunction_f "f(t)" ///@todo Rename this to just "f"

namespace oofem {
/**
 * Class implementing time function that is constant in time; @f$ f(t) = C @f$.
 */
class OOFEM_EXPORT ConstantFunction : public Function
{
private:
    /// Value of receiver.
    double value;

public:
    /**
     * Constructor. Creates constant load time function with given number, belonging to given domain.
     * @param i Load time function number.
     * @param d Domain to which new object will belongs.
     */
    ConstantFunction(int i, Domain * d) : Function(i, d) {
        value = 0;
    }
    /// Destructor.
    virtual ~ConstantFunction() { }

    /// @return Value of receiver.
    double giveValue() { return value; }

    void evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp=nullptr, double param=0.) override { answer = FloatArray{this->giveValue()}; }
    double evaluateAtTime(double t) override { return this->giveValue(); }
    double evaluateVelocityAtTime(double t) override { return 0.; }
    double evaluateAccelerationAtTime(double t) override { return 0.; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    const char *giveClassName() const override { return "ConstantFunction"; }
    const char *giveInputRecordName() const override { return _IFT_ConstantFunction_Name; }
    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};
} // end namespace oofem
#endif // constantfunction_h
