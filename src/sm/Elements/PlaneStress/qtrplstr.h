// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qtrplstr_h
#define qtrplstr_h

#include "sm/Elements/structural2delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "mesher/spatiallocalizer.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"

#define _IFT_QTrPlaneStress2d_Name "qtrplstr"

namespace oofem {
class FEI2dTrQuad;

/**
 * This class implements a quadratic triangular 6-node plane-
 * stress elasticity finite element. Each node has 2 degrees of freedom.
 */
class QTrPlaneStress2d : public PlaneStressElement, public SpatialLocalizerInterface,
public SPRNodalRecoveryModelInterface
{
protected:
    static FEI2dTrQuad interpolation;

public:
    QTrPlaneStress2d(int n, Domain * d);
    virtual ~QTrPlaneStress2d() { }

    Interface *giveInterface(InterfaceType it) override;
    FEInterpolation *giveInterpolation() const override;
    double giveParentElSize() const override { return 0.5; }



    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QTrPlaneStress2d_Name; }
    const char *giveClassName() const override { return "QTrPlaneStress2d"; }
    void initializeFrom(InputRecord &ir) override;

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

protected:
    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }

};
} // end namespace oofem
#endif // qtrplstr_h
