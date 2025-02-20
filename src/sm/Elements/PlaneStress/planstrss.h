// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef planstrss_h
#define planstrss_h

#include "sm/Elements/structural2delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_PlaneStress2d_Name "planestress2d"

namespace oofem {
class FEI2dQuadLin;

/// Comment or uncomment the following line to force full or reduced integration
#define PlaneStress2d_reducedShearIntegration
/**
 * This class implements an isoparametric four-node quadrilateral plane-
 * stress elasticity finite element. Each node has 2 degrees of freedom.
 */
class PlaneStress2d : public PlaneStressElement, public ZZNodalRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
public SpatialLocalizerInterface,
public HuertaErrorEstimatorInterface
{
protected:
    static FEI2dQuadLin interpolation;

public:
    PlaneStress2d(int n, Domain * d);
    virtual ~PlaneStress2d();

    double giveCharacteristicSize(GaussPoint *gp, FloatArray &normalToCrackPlane, ElementCharSizeMethod method) override;
    double giveParentElSize() const override { return 4.0; }

    Interface *giveInterface(InterfaceType it) override;
    FEInterpolation *giveInterpolation() const override;

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    void HuertaErrorEstimatorI_setupRefinedElementProblem(RefinedElement *refinedElement, int level, int nodeId,
                                                          IntArray &localNodeIdArray, IntArray &globalNodeIdArray,
                                                          HuertaErrorEstimatorInterface :: SetupMode sMode, TimeStep *tStep,
                                                          int &localNodeId, int &localElemId, int &localBcId,
                                                          IntArray &controlNode, IntArray &controlDof,
                                                          HuertaErrorEstimator :: AnalysisMode aMode) override;
    void HuertaErrorEstimatorI_computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;



    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_PlaneStress2d_Name; }
    const char *giveClassName() const override { return "PlaneStress2d"; }
    void initializeFrom(InputRecord &ir) override;

protected:

    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;

    int giveNumberOfIPForMassMtrxIntegration() override { return 4; } 
};
} // end namespace oofem
#endif // planstrss_h
