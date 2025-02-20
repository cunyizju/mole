// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qtruss1dgraddamage_h
#define qtruss1dgraddamage_h

#include "../sm/Elements/Bars/qtruss1d.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_QTruss1dGradDamage_Name "qtruss1dgraddamage"

namespace oofem {
class FEI1dLin;

/**
 * This class implements a three-node gradient truss bar element for one-dimensional
 * analysis.
 */
class QTruss1dGradDamage : public QTruss1d, public GradientDamageElement
{
protected:
    static FEI1dLin interpolation_lin;

public:
    QTruss1dGradDamage(int n, Domain * d);
    virtual ~QTruss1dGradDamage() { }

    const char *giveInputRecordName() const override { return _IFT_QTruss1dGradDamage_Name; }
    const char *giveClassName() const override { return "QTruss1dGradDamage"; }

    MaterialMode giveMaterialMode() override { return _1dMat; }
    void initializeFrom(InputRecord &ir) override;
    int computeNumberOfDofs() override { return 5; }

protected:
    void computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeNdMatrixAt(GaussPoint *gp, FloatArray &answer) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;
    void computeField(ValueModeType mode, TimeStep *tStep, const FloatArray &lcoords, FloatArray &answer) override;
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override;
    void computeGaussPoints() override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    void giveDofManDofIDMask_u(IntArray &answer) const override;
    void giveDofManDofIDMask_d(IntArray &answer) const override;
    
    StructuralElement *giveStructuralElement() override { return this; }
    NLStructuralElement *giveNLStructuralElement() override { return this; }
    void giveLocationArray_u(IntArray &answer) override { }
    void giveLocationArray_d(IntArray &answer) override { }
};
} // end namespace oofem
#endif // truss1d_h
