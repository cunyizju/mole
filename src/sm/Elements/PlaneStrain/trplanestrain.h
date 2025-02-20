// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef trplanstrain_h
#define trplanstrain_h

#include "sm/Elements/structural2delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "sm/ErrorEstimators/zzerrorestimator.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"
#include "mapping/mmashapefunctprojection.h"

#define _IFT_TrPlaneStrain_Name "trplanestrain"

namespace oofem {
class FEI2dTrLin;

/**
 * This class implements an triangular three-node  plane-
 * strain elasticity finite element. Each node has 2 degrees of freedom.
 */
class TrPlaneStrain : public PlaneStrainElement, public ZZNodalRecoveryModelInterface,
public NodalAveragingRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
public SpatialLocalizerInterface,
public ZZErrorEstimatorInterface,
public HuertaErrorEstimatorInterface
{
protected:
    static FEI2dTrLin interp;
    double area;

public:
    TrPlaneStrain(int n, Domain * d);
    virtual ~TrPlaneStrain() { }

    FEInterpolation *giveInterpolation() const override;
    Interface *giveInterface(InterfaceType it) override;

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



    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_TrPlaneStrain_Name; }
    const char *giveClassName() const override { return "TrPlaneStrain"; }
    void initializeFrom(InputRecord &ir) override;

protected:
    int giveNumberOfIPForMassMtrxIntegration() override { return 1; }
};
} // end namespace oofem
#endif // trplanstrain_h
