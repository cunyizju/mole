// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intelline2_h
#define intelline2_h

#include "sm/Elements/Interfaces/intelline1.h"

#define _IFT_IntElLine2_Name "intelline2"
#define _IFT_IntElLine2_LinearTraction "linear"

namespace oofem {
class FEI2dLineQuad;

/**
 * This class implements a two dimensional interface element and is simply an extension 
 * of IntElLine1 to a quadratic approximation.
 * @author Jim Brouzoulis
 * @author Borek Patzak
 */
class IntElLine2 : public IntElLine1
{
protected:
    static FEI2dLineQuad interp;
    static FEI2dLineLin interpLin;

public:
    IntElLine2(int n, Domain * d);
    virtual ~IntElLine2() { }
    FEInterpolation *giveInterpolation() const override;
    int computeNumberOfDofs() override { return 12; }
    void initializeFrom(InputRecord &ir) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_IntElLine2_Name; }
    const char *giveClassName() const override { return "IntElLine2"; }



protected:
    void computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeGaussPoints() override;

    Element_Geometry_Type giveGeometryType() const override { return EGT_quad_21_interface; }

    /// If linear interpolation should be used.
	bool linear;
};
} // end namespace oofem
#endif
