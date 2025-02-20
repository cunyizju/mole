// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef tf1_h
#define tf1_h

#include "structtemperatureload.h"

#define _IFT_TF1_Name "tf1"

namespace oofem {
/**
 * Class representing user defined temperature field.
 * No user input. The expression is hard - coded in the class body
 * as a function of global x,y and z coordinates and time t.
 *
 * The load time function is not used here, the function provided is
 * supposed to be function of time and coordinates.
 */
class TF1 : public StructuralTemperatureLoad
{
public:
    /**
     * Constructor. Creates temperature load function with given number, belonging to given domain.
     * @param n Load time function number.
     * @param d Domain to which new object will belongs.
     */
    TF1(int n, Domain * d) : StructuralTemperatureLoad(n, d) { }
    /// Destructor
    virtual ~TF1()  { }

    void initializeFrom(InputRecord &ir) override;

    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    const char *giveInputRecordName() const override { return _IFT_TF1_Name; }
    const char *giveClassName() const override { return "TF1"; }
};
} // end namespace oofem
#endif // tf1_h
