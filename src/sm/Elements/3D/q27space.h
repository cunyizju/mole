// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef q27space_h
#define q27space_h

#include "sm/Elements/structural3delement.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"

#define _IFT_Q27Space_Name "q27space"

namespace oofem {
class FEI3dHexaTriQuad;

/**
 * A 27 node tri-quadratic element for structural analysis.
 * @author Mikael Öhman
 */
class Q27Space : public Structural3DElement, public SPRNodalRecoveryModelInterface, public ZZNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface
{
protected:
    static FEI3dHexaTriQuad interpolation;

public:
    Q27Space(int n, Domain * d);
    virtual ~Q27Space() { }

    FEInterpolation *giveInterpolation() const override;

    void initializeFrom(InputRecord &ir) override;
    Interface *giveInterface(InterfaceType) override;
    int testElementExtension(ElementExtension ext) override { return ( ( ext == Element_SurfaceLoadSupport ) ? 1 : 0 ); }

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_Q27Space_Name; }
    const char *giveClassName() const override { return "Q27Space"; }
protected:
    int giveNumberOfIPForMassMtrxIntegration() override { return 27; }

    /**
     * @name Surface load support
     */
    //@{
    int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int, GaussPoint *gp) override;
    //@}
};
} // end namespace oofem
#endif
