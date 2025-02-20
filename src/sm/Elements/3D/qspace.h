// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qspace_h
#define qspace_h

#include "sm/Elements/structural3delement.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_QSpace_Name "qspace"

namespace oofem {
class FEI3dHexaQuad;

/**
 * This class implements an Quadratic 3d  20 - node element. Each node has 3 degrees of freedom.
 *
 * @author Ladislav Svoboda
 * @author Mikael Öhman
 */
class QSpace : public Structural3DElement, public SPRNodalRecoveryModelInterface, public ZZNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface, public SpatialLocalizerInterface
{
protected:
    static FEI3dHexaQuad interpolation;

    bool matRotation;

public:
    QSpace(int n, Domain * d);
    virtual ~QSpace() { }

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
    const char *giveInputRecordName() const override { return _IFT_QSpace_Name; }
    const char *giveClassName() const override { return "QSpace"; }

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
