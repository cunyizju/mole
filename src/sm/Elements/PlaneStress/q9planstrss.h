// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef Q9PLANSTRSS_H_
#define Q9PLANSTRSS_H_

#include "sm/Elements/structural2delement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"

#define _IFT_Q9PlaneStress2d_Name "q9planestress2d"

namespace oofem {
class FEI2dQuadBiQuad;

/**
 * 9-node plane stress element.
 *
 * @date May 22, 2013
 * @author Erik Svenning
 */
class Q9PlaneStress2d : public PlaneStressElement, public ZZNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface
{
protected:
    static FEI2dQuadBiQuad interpolation;

public:
    Q9PlaneStress2d(int n, Domain * d);
    virtual ~Q9PlaneStress2d() { }

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_Q9PlaneStress2d_Name; }
    const char *giveClassName() const override { return "Q9PlaneStress2d"; }
    FEInterpolation *giveInterpolation() const override;

    Interface *giveInterface(InterfaceType it) override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                    InternalStateType type, TimeStep *tStep) override;

};
} // end namespace oofem
#endif // qplanstrss_h
