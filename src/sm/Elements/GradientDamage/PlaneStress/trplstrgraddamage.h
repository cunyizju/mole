// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef trplstrgraddamage_h
#define trplstrgraddamage_h

#include "../sm/Elements/PlaneStress/trplanstrss.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_TrPlaneStressGradDamage_Name "trplanestressgraddamage"

namespace oofem {
class FEI2dTrLin;

class TrPlaneStressGradDamage : public TrPlaneStress2d, public GradientDamageElement
{
protected:
    static FEI2dTrLin interpolation_lin;
    static IntArray locationArray_u;
  static IntArray locationArray_d;

public:
    TrPlaneStressGradDamage(int n, Domain * d);
    virtual ~TrPlaneStressGradDamage() { }

    void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_TrPlaneStressGradDamage_Name; }
    const char *giveClassName() const override { return "TrPlaneStressGradDamage"; }

    MaterialMode giveMaterialMode() override { return _PlaneStress; }
    int computeNumberOfDofs() override { return 9; }

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
    void postInitialize() override;
    void giveLocationArray_u(IntArray &answer) override;
    void giveLocationArray_d(IntArray &answer) override;
};
} // end namespace oofem
#endif // trplstrgrad_h
