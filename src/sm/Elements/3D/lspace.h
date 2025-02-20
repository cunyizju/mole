// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef lspace_h
#define lspace_h

#include "sm/Elements/structural3delement.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_LSpace_Name "lspace"
#define _IFT_LSpace_reducedShearIntegration "reducedshearint"

namespace oofem {
class FEI3dHexaLin;

/**
 * This class implements a Linear 3d 8-node finite element for stress analysis.
 * Each node has 3 degrees of freedom.
 *
 * One single additional attribute is needed for Gauss integration purpose :
 * 'jacobianMatrix'. This 3x3 matrix contains polynomials.
 * TASKS :
 * - Calculating its Gauss points.
 * - Calculating its B,D,N matrices and dV.
 */
class LSpace  : public Structural3DElement, public ZZNodalRecoveryModelInterface,
    public SPRNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface,
    public SpatialLocalizerInterface,
    public HuertaErrorEstimatorInterface
{
protected:
    static FEI3dHexaLin interpolation;
    bool reducedShearIntegration;
public:
    LSpace(int n, Domain *d);
    virtual ~LSpace() { }
    FEInterpolation *giveInterpolation() const override;

    Interface *giveInterface(InterfaceType it) override;
    int testElementExtension(ElementExtension ext) override
    { return ( ( ( ext == Element_EdgeLoadSupport ) || ( ext == Element_SurfaceLoadSupport ) ) ? 1 : 0 ); }


    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                    InternalStateType type, TimeStep *tStep) override;

    void HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                          IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                          HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                          int &localNodeId, int &localElemId, int &localBcId,
                                                          IntArray &controlNode, IntArray &controlDof,
                                                          HuertaErrorEstimator :: AnalysisMode aMode) override;
    void HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_LSpace_Name; }
    const char *giveClassName() const override { return "LSpace"; }
    void initializeFrom(InputRecord &ir) override;

protected:
    int giveNumberOfIPForMassMtrxIntegration() override { return 8; }
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui) override;
    /**
     * @name Surface load support
     */
    //@{
    int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int iSurf, GaussPoint *gp) override;
    //@}
};
} // end namespace oofem
#endif // lspace_h
