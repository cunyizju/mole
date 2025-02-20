// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef reinforcement_h
#define reinforcement_h

#include "bc/bodyload.h"
#include "bc/bcgeomtype.h"
#include "input/valuemodetype.h"

///@name Input fields for Reinforcement
//@{
#define _IFT_Reinforcement_Name "reinforcement"
#define _IFT_Reinforcement_porosity "porosity"
#define _IFT_Reinforcement_permeability "permeability"
#define _IFT_Reinforcement_shapeFactor "shapefactor"
//@}

namespace oofem {
/**
 * This class implements an influence of reinforcement into flow problems, especially concrete (binhamfluid).
 * It is modeled as a special body load acting on elements in area, where reinfocement is placed. The inherited attribute 'componentArray' contains the components of an
 * loading prescribed per unit volume.
 *
 * Its task is to return the body force @f$ \rho a @f$
 */
class OOFEM_EXPORT Reinforcement : public BodyLoad
{
protected:
    double porosity;
    double shapefactor;
    FloatArray permeability;

public:
    /// Constructor
    Reinforcement(int i, Domain * d) : BodyLoad(i, d) { }
    /**
     * Computes components values of deadweight field at given point (coordinates given in Global c.s.).
     * taking into account corresponding load time function value while respecting load response mode.
     * @param answer Component values at given point and time.
     * @param tStep Time step.
     * @param coords Global coordinates, which are used to evaluate components values.
     * @param mode Determines response mode-
     */
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override
    { computeComponentArrayAt(answer, tStep, mode); }

    bcValType giveBCValType() const override { return ReinforceBVT; }
    bcGeomType giveBCGeoType() const override { return BodyLoadBGT; }

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "Reinforcement"; }
    const char *giveInputRecordName() const override { return _IFT_Reinforcement_Name; }

    /// Accessor
    double givePorosity() { return porosity; }
    double giveshapefactor() { return shapefactor; }
    FloatArray *givePermeability() { return & permeability; }
};
} // end namespace oofem
#endif // deadwght_h
