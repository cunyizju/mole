// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef deadweight_h
#define deadweight_h

#include "bc/bodyload.h"
#include "bc/bcgeomtype.h"
#include "input/valuemodetype.h"

#define _IFT_DeadWeight_Name "deadweight"

namespace oofem {
/**
 * This class implements a gravity-like load, or internal source (heat etc.) for transport problems.
 * The inherited attribute 'componentArray' contains the components of an
 * loading prescribed per unit volume.
 *
 * Its task is to return the body force @f$ \rho a @f$
 */
class OOFEM_EXPORT DeadWeight : public BodyLoad
{
public:
    /// Constructor
    DeadWeight(int i, Domain * d) : BodyLoad(i, d) { }
    /**
     * Computes components values of deadweight field at given point (coordinates given in Global c.s.).
     * taking into account corresponding load time function value while respecting load response mode.
     * @param answer Component values at given point and time.
     * @param tStep Time step.
     * @param coords Global coordinates, which are used to evaluate components values.
     * @param mode Determines response mode-
     */
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    bcValType giveBCValType() const override { return ForceLoadBVT; }
    bcGeomType giveBCGeoType() const override { return BodyLoadBGT; }

    void setDeadWeighComponents(FloatArray newComponents);

    const char *giveClassName() const override { return "DeadWeight"; }
    const char *giveInputRecordName() const override { return _IFT_DeadWeight_Name; }
};
} // end namespace oofem
#endif // deadweight_h
