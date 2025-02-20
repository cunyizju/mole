// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef constantedgeload_h
#define constantedgeload_h

#include "bc/boundaryload.h"

#define _IFT_ConstantEdgeLoad_Name "constantedgeload"

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
class OOFEM_EXPORT ConstantEdgeLoad : public EdgeLoad
{
public:
    ConstantEdgeLoad(int i, Domain * d) : EdgeLoad(i, d) { }

    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;
    int giveApproxOrder() override { return 0; }
    void initializeFrom(InputRecord &ir) override;
    bcGeomType giveBCGeoType() const override { return EdgeLoadBGT; }

    const char *giveClassName() const override { return "ConstantEdgeLoad"; }
    const char *giveInputRecordName() const override { return _IFT_ConstantEdgeLoad_Name; }

private:
    void computeNArray(FloatArray &answer, const FloatArray &coords) const override { answer.clear(); }
};
} // end namespace oofem
#endif // constantedgeload_h
