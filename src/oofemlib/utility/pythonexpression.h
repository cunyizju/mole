// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef pythonexpression_h
#define pythonexpression_h

#include "func/function.h"

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

///@name Input fields for PythonExpression
//@{
#define _IFT_PythonExpression_Name "pythonexpression"
#define _IFT_PythonExpression_f "f" ///< Expression with return variable named "ret"
#define _IFT_PythonExpression_dfdt "dfdt" ///< Velocity with return variable named "ret"
#define _IFT_PythonExpression_d2fdt2 "d2fdt2" ///< Acceleration with return variable named "ret"
//@}


namespace oofem {
/**
 * Class representing user defined functions as Python expressions
 */
class OOFEM_EXPORT PythonExpression : public Function
{
private:
    /// Expression for the function value.
    std :: string fExpression;
    /// Expression for first time derivative.
    std :: string dfdtExpression;
    /// Expression for second time derivative.
    std :: string d2fdt2Expression;

    PyObject *f;
    PyObject *dfdt;
    PyObject *d2fdt2;

    PyObject *main_dict;

    /// Helper function to convert the std::map to a Python dictionary.
    PyObject *getDict(const std :: map< std :: string, FunctionArgument > &valDict);
    /// Helper function to run given function for given value dictionary.
    void getArray(FloatArray &answer, PyObject *func, const std :: map< std :: string, FunctionArgument > &valDict);
    /// Helper function to run given function for given time
    double getScalar(PyObject *func, double time);

public:
    /**
     * Constructor. Creates load time function with given number, belonging to given domain.
     * @param n Load time function number.
     * @param d Domain to which new object will belongs..
     */
    PythonExpression(int n, Domain * d);
    /// Destructor.
    virtual ~PythonExpression();

    /**
     * Reads the fields
     * - f(t) (required)
     * - dfdt(t) (optional)
     * - d2fdt2(t) (optional)
     */
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &ir) override;

    void evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp=nullptr, double param=0.) override;
    void evaluateVelocity(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict);
    void evaluateAcceleration(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict);

    double evaluateAtTime(double t) override;
    double evaluateVelocityAtTime(double t) override;
    double evaluateAccelerationAtTime(double t) override;

    const char *giveClassName() const override { return "PythonExpression"; }
    const char *giveInputRecordName() const override { return _IFT_PythonExpression_Name; }
};
} // end namespace oofem
#endif // pythonexpression_h
