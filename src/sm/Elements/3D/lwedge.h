// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef lwedge_h
#define lwedge_h

#include "sm/Elements/structural3delement.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_LWedge_Name "lwedge"

namespace oofem {
class FEI3dWedgeLin;

/**
 * This class implements a Linear 3d  6 - node elasticity finite element.
 */
class LWedge : public Structural3DElement, public SPRNodalRecoveryModelInterface, public ZZNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface, public SpatialLocalizerInterface
{
protected:
    static FEI3dWedgeLin interpolation;

public:
    LWedge(int, Domain *);
    virtual ~LWedge() { }

    FEInterpolation *giveInterpolation() const override;

    void initializeFrom(InputRecord &ir) override;

    Interface *giveInterface(InterfaceType) override;
    int testElementExtension(ElementExtension ext) override { return ( ( ext == Element_SurfaceLoadSupport ) ? 1 : 0 ); }
    int giveNumberOfIPForMassMtrxIntegration() override { return 9; }

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_LWedge_Name; }
    const char *giveClassName() const override { return "LWedge"; }
};
} // end namespace oofem
#endif
