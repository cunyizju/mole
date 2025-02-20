// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef axisymm3d_h
#define axisymm3d_h

#include "sm/Elements/structural2delement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"

///@name Input fields for Axisymm3d
//@{
#define _IFT_Axisymm3d_Name "axisymm3d"
//#define _IFT_Axisymm3d_nipfish "nipfish"
//@}

namespace oofem {
class FEI2dTrLinAxi;

/**
 * This class implements an triangular three-node finite element for axisymmetric continuum.
 * Each node has 2 degrees of freedom.
 *
 * Tasks:
 * - calculating its B,D,N matrices and dV.
 */
class Axisymm3d : public AxisymElement, public ZZNodalRecoveryModelInterface,
public NodalAveragingRecoveryModelInterface, public SPRNodalRecoveryModelInterface,
public SpatialLocalizerInterface
{
protected:
    static FEI2dTrLinAxi interpolation;

    double area;

public:
    Axisymm3d(int n, Domain * d);
    virtual ~Axisymm3d();

    virtual double giveArea();
    Interface *giveInterface(InterfaceType it) override;
    FEInterpolation *giveInterpolation() const override;



    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                    InternalStateType type, TimeStep *tStep) override;

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    const char *giveClassName() const override { return "Axisymm3d"; }
    const char *giveInputRecordName() const override { return _IFT_Axisymm3d_Name; }
    void initializeFrom(InputRecord &ir) override;
};
} // end namespace oofem
#endif // axisymm3d_h
