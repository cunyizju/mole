// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef rotatingboundary_h
#define rotatingboundary_h

#include "bc/boundarycondition.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"

///@name Input fields for RotatingBoundary
//@{
#define _IFT_RotatingBoundary_Name "rotatingboundary"
#define _IFT_RotatingBoundary_axis "axis"
#define _IFT_RotatingBoundary_center "center"
#define _IFT_RotatingBoundary_frequency "frequency" ///< @todo Unused ( But it makes sense that you'd have this, can you check it Andreas? ) / Mikael
//@}

namespace oofem {
/**
 * Class implementing rotating boundary surface.
 * This boundary condition is usually attribute of one or more degrees of freedom (DOF).
 *
 * @author Andreas Feymark
 */
class RotatingBoundary : public BoundaryCondition
{
protected:
    /// Rotation matrix.
    FloatMatrix R;

    /// Axis and center of rotation.
    FloatArray axis, center;

public:
    /**
     * Constructor. Creates boundary condition with given number, belonging to given domain.
     * @param i Boundary condition number.
     * @param d Domain to which new object will belongs.
     */
    RotatingBoundary(int i, Domain * d) : BoundaryCondition(i, d) { }
    /// Destructor.
    virtual ~RotatingBoundary() { }

    double give(Dof *dof, ValueModeType mode, double time) override;

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    void scale(double s) override { }

    const char *giveInputRecordName() const override { return _IFT_RotatingBoundary_Name; }
    const char *giveClassName() const override { return "RotatingBoundary"; }
};
} // end namespace oofem
#endif // rotatingboundary_h
