// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef q4axisymm_h
#define q4axisymm_h

#include "sm/Elements/structuralelement.h"
#include "sm/Elements/structural2delement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"

///@name Input fields for Q4Axisymm
//@{
#define _IFT_Q4Axisymm_Name "q4axisymm"
#define _IFT_Q4Axisymm_nipfish "nipfish"
//@}

namespace oofem {
class FEI2dQuadQuadAxi;

/**
 * This class implements an Quadratic isoparametric eight-node quadrilateral -
 * elasticity finite element for axisymmetric 3d continuum.
 * Each node has 2 degrees of freedom.
 */
class Q4Axisymm : public AxisymElement, public ZZNodalRecoveryModelInterface
{
protected:
    static FEI2dQuadQuadAxi interp;
    int numberOfFiAndShGaussPoints;

public:
    Q4Axisymm(int n, Domain * d);
    virtual ~Q4Axisymm();

    FEInterpolation *giveInterpolation() const override;

    // definition & identification
    Interface *giveInterface(InterfaceType) override;
    const char *giveInputRecordName() const override { return _IFT_Q4Axisymm_Name; }
    const char *giveClassName() const override { return "Q4axisymm"; }
    void initializeFrom(InputRecord &ir) override;

    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int lowerIndx = 1, int upperIndx = ALL_STRAINS) override;
};
} // end namespace oofem
#endif // q4axisymm_h
