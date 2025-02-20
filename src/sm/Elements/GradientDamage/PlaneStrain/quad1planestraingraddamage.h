// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef quad1planestraingraddamage_h
#define quad1planestraingraddamage_h

#include "../sm/Elements/PlaneStrain/quad1planestrain.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_Quad1PlaneStrainGradDamage_Name "quad1planestraingraddamage"

namespace oofem {

class Quad1PlaneStrainGradDamage : public Quad1PlaneStrain, public GradientDamageElement
{
protected:
  static IntArray locationArray_u;
  static IntArray locationArray_d;

public:
    Quad1PlaneStrainGradDamage(int n, Domain * d);
    virtual ~Quad1PlaneStrainGradDamage() { }

    //void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_Quad1PlaneStrainGradDamage_Name; }
    const char *giveClassName() const override { return "Quad1PlaneStrainGradDamage"; }

    MaterialMode giveMaterialMode() override { return _PlaneStrain; }
    int computeNumberOfDofs() override { return 12; }

protected:
    void computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeNdMatrixAt(GaussPoint *gp, FloatArray &answer) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override { GradientDamageElement :: computeStiffnessMatrix(answer, rMode, tStep); }
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override { GradientDamageElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord); }

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
#endif // quad1planestraingrad_h
