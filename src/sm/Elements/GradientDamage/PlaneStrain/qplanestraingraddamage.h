// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qplanestraingraddamage_h
#define qplanestraingraddamage_h

#include "../sm/Elements/PlaneStrain/qplanestrain.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_QPlaneStrainGradDamage_Name "qplanestraingraddamage"

namespace oofem {
class FEI2dQuadLin;

class QPlaneStrainGradDamage : public QPlaneStrain, public GradientDamageElement
{
protected:
    static FEI2dQuadLin interpolation_lin;

public:
    QPlaneStrainGradDamage(int n, Domain * d);
    virtual ~QPlaneStrainGradDamage() { }

    void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_QPlaneStrainGradDamage_Name; }
    const char *giveClassName() const override { return "QPlaneStrainGradDamage"; }

    MaterialMode giveMaterialMode() override { return _PlaneStrain; }
    int computeNumberOfDofs() override { return 20; }

protected:
    void computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeNdMatrixAt(GaussPoint *gp, FloatArray &answer) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override { GradientDamageElement :: computeStiffnessMatrix(answer, rMode, tStep); }
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override { GradientDamageElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord); }

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
#endif // qplanestraingrad_h
