// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef usrdeftempfield_h
#define usrdeftempfield_h

#include "structtemperatureload.h"
#include "utility/parser.h"

///@name Input fields for UserDefinedTemperatureField
//@{
#define _IFT_UserDefinedTemperatureField_Name "usrdeftempfield"
#define _IFT_UserDefinedTemperatureField_size "size"
#define _IFT_UserDefinedTemperatureField_t1 "t1(txyz)"
#define _IFT_UserDefinedTemperatureField_t2 "t2(txyz)"
#define _IFT_UserDefinedTemperatureField_t3 "t3(txyz)"
//@}

namespace oofem {
/**
 * Class representing user defined temperature field. User input is function expression,
 * as a function of global x,y and z coordinates and time t.
 *
 * The load time function is not used here, the function provided is
 * supposed to be function of time and coordinates.
 *
 * Uses Parser class to parse given expression. Slow but useful.
 * Temperature load as body load is typically attribute of  domain and is
 * attribute of one or more elements.
 */
class UserDefinedTemperatureField : public StructuralTemperatureLoad
{
private:
    Parser myParser;
    int size;
    std :: string ftExpression [ 3 ];

public:
    /**
     * Constructor. Creates temperature load function with given number, belonging to given domain.
     * @param n Load time function number
     * @param d Domain to which new object will belongs.
     */
    UserDefinedTemperatureField(int n, Domain * d) : StructuralTemperatureLoad(n, d), myParser() { }
    /// Destructor
    virtual ~UserDefinedTemperatureField() { }

    /**
     * Computes components values of temperature field at given point (coordinates given in Global c.s.).
     * taking into account corresponding load time function value respecting load response mode.
     * @param answer Component values at given point and time.
     * @param tStep Time step representing time.
     * @param coords Global coordinates, which are used to evaluate components values.
     * @param mode Determines response mode.
     */
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    const char *giveInputRecordName() const override { return _IFT_UserDefinedTemperatureField_Name; }
    const char *giveClassName() const override { return "UserDefinedTemperatureField"; }

    void initializeFrom(InputRecord &ir) override;
};
} // end namespace oofem
#endif // usrdeftempfield_h
