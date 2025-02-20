// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qspacegraddamage_h
#define qspacegraddamage_h

#include "../sm/Elements/3D/qspace.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_QSpaceGradDamage_Name "qspacegraddamage"

namespace oofem {
class FEI3dHexaLin;

/**
 * Quadratic 3d  20 - node element with quadratic approximation of displacements and linear approximation of gradient damage driving variable
 *
 * @author Martin Horak
 */
class QSpaceGradDamage : public QSpace, public GradientDamageElement
{
protected:
    static FEI3dHexaLin interpolation_lin;

public:
    QSpaceGradDamage(int n, Domain * d);
    virtual ~QSpaceGradDamage() { }

    void initializeFrom(InputRecord &ir) override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    void giveDofManDofIDMask_u(IntArray &answer) const override;
    void giveDofManDofIDMask_d(IntArray &answer) const override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QSpaceGradDamage_Name; }
    const char *giveClassName() const override { return "QSpaceGradDamage"; }
    int computeNumberOfDofs() override { return 68; }
    MaterialMode giveMaterialMode() override { return _3dMat; }

protected:
    void computeGaussPoints() override;
    void computeNdMatrixAt(GaussPoint *gp, FloatArray &answer) override;
    void computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    StructuralElement *giveStructuralElement() override { return this; }
    NLStructuralElement *giveNLStructuralElement() override { return this; }
    void giveLocationArray_u(IntArray &answer) override { }
    void giveLocationArray_d(IntArray &answer) override { }
};
}
#endif // end namespace oofem
