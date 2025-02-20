// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef foreigntempfield_h
#define foreigntempfield_h

#include <memory> // std::shared_ptr
#include "structtemperatureload.h"

///@name Input fields for ForeignTemperatureFieldLoad
//@{
#define _IFT_ForeignTemperatureFieldLoad_Name "foreigntempfieldload"
//@}

namespace oofem {

class Field;
/**
 * Class representing foreign temperature field, which asks a field object to return
 * temperature at given point. 
 *
 * The load time function is not used here, the field has to be updated by the user
 * if it is non-constant in time.
 *
 * The field (stored in foreignField) cannot be set in the input file (there
 * is only one mesh defined), it is typically assigned using Python bindings.
 *
 */
class ForeignTemperatureFieldLoad : public StructuralTemperatureLoad
{
public:
    // make public so that it can be simply set from python
    std::shared_ptr<Field> foreignField;
    /**
     * Constructor. Creates temperature load function with given number, belonging to given domain.
     * @param n Load time function number
     * @param d Domain to which new object will belongs.
     */
    ForeignTemperatureFieldLoad(int n, Domain * d) : StructuralTemperatureLoad(n, d) { }
    /// Destructor
    virtual ~ForeignTemperatureFieldLoad() { }

    /**
     * Computes components values of temperature field at given point (coordinates given in Global c.s.).
     * taking into account corresponding load time function value respecting load response mode.
     * @param answer Component values at given point and time.
     * @param tStep Time step representing time.
     * @param coords Global coordinates, which are used to evaluate components values.
     * @param mode Determines response mode.
     */
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    const char *giveInputRecordName() const override { return _IFT_ForeignTemperatureFieldLoad_Name; }
    const char *giveClassName() const override { return "ForeignTemperatureFieldLoad"; }

    void initializeFrom(InputRecord &ir) override;
};
} // end namespace oofem
#endif // foreigntempfield_h
