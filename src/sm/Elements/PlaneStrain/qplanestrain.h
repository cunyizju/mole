// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qplanstrain_h
#define qplanstrain_h

#include "sm/Elements/structural2delement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"

#define _IFT_QPlaneStrain_Name "qplanestrain"

namespace oofem {
class FEI2dQuadQuad;

/**
 * This class implements an Quadratic isoparametric eight-node quadrilateral plane-
 * stress elasticity finite element. Each node has 2 degrees of freedom.
 */
class QPlaneStrain : public PlaneStrainElement, public ZZNodalRecoveryModelInterface
{
protected:
    static FEI2dQuadQuad interpolation;

public:
    QPlaneStrain(int N, Domain * d);
    virtual ~QPlaneStrain() { }

    FEInterpolation *giveInterpolation() const override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QPlaneStrain_Name; }
    const char *giveClassName() const override { return "QPlaneStrain"; }

    int testElementExtension(ElementExtension ext) override { return 0; } ///@todo //check this probably ok now when derived from PE-element

    Interface *giveInterface(InterfaceType it) override;



};
} // end namespace oofem
#endif // qplanstrain_h
