// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qtrplanestraingraddamage_h
#define qtrplanestraingraddamage_h

#include "../sm/Elements/PlaneStrain/qtrplanestrain.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

namespace oofem {
class FEI2dTrLin;

class QTrPlaneStrainGradDamage : public QTrPlaneStrain, public GradientDamageElement
{
protected:
    static FEI2dTrLin interpolation_lin;

public:
    QTrPlaneStrainGradDamage(int n, Domain * d);
    virtual ~QTrPlaneStrainGradDamage() { }

    void initializeFrom(InputRecord &ir) override;

    //const char *giveInputRecordName() const override { return _IFT_QtrPlaneStrainGradDamage_Name; }
    const char *giveClassName() const override { return "QTrPlaneStrainGrad"; }

protected:
    void computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeNdMatrixAt(GaussPoint *gp, FloatMatrix &answer);
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override { GradientDamageElement :: computeStiffnessMatrix(answer, rMode, tStep); }
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override { GradientDamageElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord); }

    int computeNumberOfDofs() override { return 15; }
    void computeGaussPoints() override;
    void giveDofManDofIDMask(int inode, IntArray &) const override;
    void giveDofManDofIDMask_u(IntArray &answer) const override;
    void giveDofManDofIDMask_d(IntArray &answer) const override;
    
    StructuralElement *giveStructuralElement() override { return this; }
    NLStructuralElement *giveNLStructuralElement() override { return this; }
};
} // end namespace oofem
#endif // qtrplanestraingrad_h
