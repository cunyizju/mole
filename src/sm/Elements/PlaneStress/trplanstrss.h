// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef trplanstrss_h
#define trplanstrss_h

#include "sm/Elements/structural2delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "sm/ErrorEstimators/zzerrorestimator.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "sm/CrossSections/layeredcrosssection.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"
#include "mapping/mmashapefunctprojection.h"


#define _IFT_TrPlaneStress2d_Name "trplanestress2d"

namespace oofem {
class FEI2dTrLin;

/**
 * This class implements an triangular three-node plane-stress
 * elasticity finite element. Each node has 2 degrees of freedom.
 * Element has 3 nodes and 6 DoFs.
 * Tasks:
 * - calculating its B,D,N matrices and dV.
 */
class TrPlaneStress2d : public PlaneStressElement, public ZZNodalRecoveryModelInterface,
public NodalAveragingRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
public SpatialLocalizerInterface,
public ZZErrorEstimatorInterface,
public HuertaErrorEstimatorInterface,
public LayeredCrossSectionInterface
{
protected:
    static FEI2dTrLin interp;
    double area;

public:
    TrPlaneStress2d(int n, Domain * d);
    virtual ~TrPlaneStress2d() { }

    FEInterpolation *giveInterpolation() const override;
    double giveCharacteristicSize(GaussPoint *gp, FloatArray &normalToCrackPlane, ElementCharSizeMethod method) override;
    double giveParentElSize() const override { return 0.5; }
    Interface *giveInterface(InterfaceType) override;



    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_TrPlaneStress2d_Name; }
    const char *giveClassName() const override { return "TrPlaneStress2d"; }

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
    // Methods to implement LayeredCrossSectionInterface
    void computeStrainVectorInLayer(FloatArray &answer, const FloatArray &masterGpStrain, GaussPoint *masterGp, GaussPoint *slaveGp, TimeStep *tStep) override;

protected:

    virtual double giveArea();
    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }
};
} // end namespace oofem
#endif // trplanstrss_h
