// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qwedge_h
#define qwedge_h

#include "sm/Elements/structural3delement.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

#define _IFT_QWedge_Name "qwedge"

namespace oofem {
class FEI3dWedgeQuad;

/**
 * This class implements an Quadratic 3d  15 - node structural finite element.
 * Each node has 3 degrees of freedom.
 *
 * @author Mikael Öhman (among others)
 */
class QWedge : public Structural3DElement, public SPRNodalRecoveryModelInterface, public ZZNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface, public SpatialLocalizerInterface
{
protected:
    static FEI3dWedgeQuad interpolation;

public:
    QWedge(int, Domain *);
    virtual ~QWedge() { }

    FEInterpolation *giveInterpolation() const override;

    void initializeFrom(InputRecord &ir) override;
    int giveNumberOfIPForMassMtrxIntegration() override { return 9; }

    Interface *giveInterface(InterfaceType) override;
    int testElementExtension(ElementExtension ext) override { return ( ( ext == Element_SurfaceLoadSupport ) ? 1 : 0 ); }

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QWedge_Name; }
    const char *giveClassName() const override { return "QWedge"; }
};
} // end namespace oofem
#endif
