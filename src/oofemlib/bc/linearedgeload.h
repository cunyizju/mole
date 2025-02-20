// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef linearedgeload_h
#define linearedgeload_h

#include "bc/boundaryload.h"
#include "math/floatarray.h"

///@name Input fields for LinearEdgeLoad
//@{
#define _IFT_LinearEdgeLoad_Name "linearedgeload"
#define _IFT_LinearEdgeLoad_formulation "formulation"
#define _IFT_LinearEdgeLoad_startcoord "sc"
#define _IFT_LinearEdgeLoad_endcoord "ec"
//@}

namespace oofem {
class TimeStep;

/**
 * This class implements a linear boundary load (force, moment,...) that acts
 * on straight segment.
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
 * @note{This class is not restricted to structural problems. For example, in thermal analysis,
 * a boundary load load would be a  heat source.}
 */
class OOFEM_EXPORT LinearEdgeLoad : public EdgeLoad
{
protected:
    /// Coordinates of start and end point
    FloatArray startCoords, endCoords;
    FormulationType formulation;

public:
    LinearEdgeLoad(int i, Domain * d) : EdgeLoad(i, d) { }
    virtual ~LinearEdgeLoad() { }

    int giveApproxOrder() override { return 1; }
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    bcGeomType giveBCGeoType() const override { return EdgeLoadBGT; }
    FormulationType giveFormulationType() override { return formulation; }

    const char *giveClassName() const override { return "LinearEdgeLoad"; }
    const char *giveInputRecordName() const override { return _IFT_LinearEdgeLoad_Name; }

protected:
    void computeNArray(FloatArray &answer, const FloatArray &coords) const override;
};
} // end namespace oofem
#endif // linearedgeload_h
