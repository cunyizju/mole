// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qwedgegraddamage_h
#define qwedgegraddamage_h

#include "../sm/Elements/3D/qwedge.h"
#include "../sm/Elements/GradientDamage/graddamageelement.h"

#define _IFT_QWedgeGradDamage_Name "qwedgegraddamage"

namespace oofem {
class FEI3dWedgeLin;

/**
 * @author M. Horak
 */
class QWedgeGradDamage : public QWedge, public GradientDamageElement
{
protected:
    static FEI3dWedgeLin interpolation_lin;

public:
    QWedgeGradDamage(int, Domain *);
    virtual ~QWedgeGradDamage() { }

    void initializeFrom(InputRecord &ir) override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    void giveDofManDofIDMask_u(IntArray &answer) const override;
    void giveDofManDofIDMask_d(IntArray &answer) const override;
    
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QWedgeGradDamage_Name; }
    const char *giveClassName() const override { return "QWedgeGradDamage"; }
    int computeNumberOfDofs() override { return 51; }
    MaterialMode giveMaterialMode() override { return _3dMat; }

protected:
    void computeGaussPoints() override;
    void computeNdMatrixAt(GaussPoint *gp, FloatArray &answer) override;
    void computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    StructuralElement *giveStructuralElement() override { return this; }
    NLStructuralElement *giveNLStructuralElement() override { return this; }

    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override { GradientDamageElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord); }
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override { GradientDamageElement :: computeStiffnessMatrix(answer, rMode, tStep); }
    void giveLocationArray_u(IntArray &answer) override { }
    void giveLocationArray_d(IntArray &answer) override { }

};
}
#endif // end namespace oofem
