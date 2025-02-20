// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qtrplanestrain_h
#define qtrplanestrain_h

#include "sm/Elements/structural2delement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "mesher/spatiallocalizer.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"

#define _IFT_QTrPlaneStrain_Name "qtrplanestrain"

namespace oofem {
class FEI2dTrQuad;

/**
 * This class implements an triangular three-node  plane-
 * stress elasticity finite element. Each node has 2 degrees of freedom.
 */
class QTrPlaneStrain : public PlaneStrainElement, public SpatialLocalizerInterface,
public SPRNodalRecoveryModelInterface, public ZZNodalRecoveryModelInterface
{
protected:
    static FEI2dTrQuad interpolation;

public:
    QTrPlaneStrain(int n, Domain * d);
    virtual ~QTrPlaneStrain() { }

    FEInterpolation *giveInterpolation() const override;

    Interface *giveInterface(InterfaceType it) override;
    double giveParentElSize() const override { return 0.5; }

    int testElementExtension(ElementExtension ext) override { return ( ( ext == Element_EdgeLoadSupport ) ? 0 : 0 ); }



    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QTrPlaneStrain_Name; }
    const char *giveClassName() const override { return "QTrPlaneStrain"; }
    
    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;


protected:
    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }
};
} // end namespace oofem
#endif // qtrplanestrain_h
