// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ltrspace_h
#define ltrspace_h

#include "sm/Elements/structural3delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"
#include "sm/ErrorEstimators/zzerrorestimator.h"
#include "mapping/mmashapefunctprojection.h"

#define _IFT_LTRSpace_Name "ltrspace"

namespace oofem {
class FEI3dTetLin;

/**
 * This class implements a linear tetrahedral four-node finite element for stress analysis.
 * Each node has 3 degrees of freedom.
 */
class LTRSpace : public Structural3DElement, public ZZNodalRecoveryModelInterface,
public NodalAveragingRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
public SpatialLocalizerInterface,
public ZZErrorEstimatorInterface,
public HuertaErrorEstimatorInterface
{
protected:
    static FEI3dTetLin interpolation;

public:
    LTRSpace(int n, Domain * d);
    virtual ~LTRSpace() { }

    FEInterpolation *giveInterpolation() const override;

    void computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep) override;
    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }
    Interface *giveInterface(InterfaceType it) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_LTRSpace_Name; }
    const char *giveClassName() const override { return "LTRSpace"; }



    double giveRelativeSelfComputationalCost() override { return 2.15; }

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                    InternalStateType type, TimeStep *tStep) override;

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    // HuertaErrorEstimatorInterface
    void HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                          IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                          HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                          int &localNodeId, int &localElemId, int &localBcId,
                                                          IntArray &controlNode, IntArray &controlDof,
                                                          HuertaErrorEstimator :: AnalysisMode aMode) override;
    void HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
};
} // end namespace oofem
#endif // ltrspace_h
