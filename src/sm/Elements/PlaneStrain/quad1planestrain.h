// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef quad1planestrain_h
#define quad1planestrain_h

#include "sm/Elements/structural2delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_Quad1PlaneStrain_Name "quad1planestrain"

namespace oofem {
class FEI2dQuadLin;

/// Comment or uncomment the following line to force full or reduced integration
///@todo Removed for now.
//#define Quad1PlaneStrain_reducedShearIntegration
//#define Quad1PlaneStrain_reducedVolumetricIntegration
/**
 * This class implements an isoparametric four-node quadrilateral plane-
 * stress structural finite element. Each node has 2 degrees of freedom.
 */
class Quad1PlaneStrain : public PlaneStrainElement, public ZZNodalRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
    public SpatialLocalizerInterface,
    public HuertaErrorEstimatorInterface
{
protected:
    static FEI2dQuadLin interp;

public:
    Quad1PlaneStrain(int n, Domain *d);
    virtual ~Quad1PlaneStrain();

    FEInterpolation *giveInterpolation() const override;
    Interface *giveInterface(InterfaceType it) override;

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
    const char *giveInputRecordName() const override { return _IFT_Quad1PlaneStrain_Name; }
    const char *giveClassName() const override { return "Quad1PlaneStrain"; }
    void initializeFrom(InputRecord &ir) override;

protected:
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;

    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }
};
} // end namespace oofem
#endif // quad1planestrain_h
