// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef constantpressureload_h
#define constantpressureload_h

#include "bc/boundaryload.h"

///@name Input fields for ConstantPressureLoad
//@{
#define _IFT_ConstantPressureLoad_LoadOffset "loadoffset"
#define _IFT_ConstantPressureLoad_Name "constantpressureload"
//@}

namespace oofem {
/**
 * This class implements a boundary load (force, moment,...) that acts
 * directly on a boundary of some finite element (on side, face, ..).
 * A boundary load is usually attribute of one or more elements.
 *
 * The boundary load describes its geometry and values (it is assumed, that user will specify
 * all necessary dofs) on element boundary using isoparametric approximation.
 * Elements can request the order of approximation (for setting up the appropriate
 * integration rule order) and the array of values (for each dof) at specific integration point
 * on the boundary.
 *
 * Elements must take care, on which boundary the load acts on (side number, ...).
 *
 * For some elements it may be better to obtain "vertex values" of boundary load to
 * compute load vector directly using exact formulae.
 *
 * This class is not restricted to structural problems. For example, in thermal
 * analysis, a boundary load load would be a  heat source.
 */
class ConstantPressureLoad : public SurfaceLoad
{
public:
    ConstantPressureLoad(int i, Domain * d);

    // Overloaded methods:
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;
    void computeValues(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, const IntArray &dofids, ValueModeType mode) override;
    int giveApproxOrder() override { return 0; }

    /**
     * Sets a new load vector.
     * @param newValue New load.
     */
    void updateLoad(const FloatArray &newValue) { componentArray = newValue; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    bcGeomType giveBCGeoType() const override { return SurfaceLoadBGT; }

    const char *giveClassName() const override { return "ConstantPressureLoad"; }
    const char *giveInputRecordName() const override { return _IFT_ConstantPressureLoad_Name; }
    double giveLoadOffset() { return this->loadOffset; }
private:
    void computeNArray(FloatArray &answer, const FloatArray &coords) const override { answer.clear(); }
    double loadOffset;  // xi-coord offset of load. xi=-1 -> bottom, xi=0 -> midsurface (default), xi=1 -> top surface
};
} // end namespace oofem
#endif
