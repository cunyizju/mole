// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qplanestressgraddamage_h
#define qplanestressgraddamage_h

#include "../sm/Elements/PlaneStress/qplanstrss.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_QPlaneStressGradDamage_Name "qplanestressgraddamage"

namespace oofem {
class FEI2dQuadLin;

class QPlaneStressGradDamage : public QPlaneStress2d, public GradientDamageElement
{
protected:
      static IntArray locationArray_u;
      static IntArray locationArray_d;
      static FEI2dQuadLin interpolation_lin;

public:
    QPlaneStressGradDamage(int n, Domain * d);
    virtual ~QPlaneStressGradDamage() { }

    void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_QPlaneStressGradDamage_Name; }
    const char *giveClassName() const override { return "QPlaneStressGradDamage"; }
    MaterialMode giveMaterialMode() override { return _PlaneStress; }
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
    void giveLocationArray_u(IntArray &answer) override;
    void giveLocationArray_d(IntArray &answer) override;
    void postInitialize() override;

};
} // end namespace oofem
#endif // qplanestressgrad_h
