// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef l4axisymm_h
#define l4axisymm_h

#include "sm/Elements/structural2delement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_L4Axisymm_Name "l4axisymm"

namespace oofem {
class FEI2dQuadLinAxi;

/**
 * This class implements an isoparametric four-node quadrilateral axisymmetric
 * finite element. Each node has 2 degrees of freedom.
 */
class L4Axisymm : public AxisymElement, public ZZNodalRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
public SpatialLocalizerInterface
{
protected:
    static FEI2dQuadLinAxi interpolation;
    int numberOfFiAndShGaussPoints;

public:
    L4Axisymm(int n, Domain * d);
    virtual ~L4Axisymm();

    FEInterpolation *giveInterpolation() const override;

    Interface *giveInterface(InterfaceType it) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_L4Axisymm_Name; }
    const char *giveClassName() const override { return "L4Axisymm"; }

    void initializeFrom(InputRecord &ir) override;

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;




protected:
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int lowerIndx = 1, int upperIndx = ALL_STRAINS) override;
};
} // end namespace oofem
#endif // l4axisymm_h
