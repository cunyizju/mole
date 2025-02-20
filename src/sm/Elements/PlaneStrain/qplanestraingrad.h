// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qplanestraingrad_h
#define qplanestraingrad_h

#include "sm/Elements/PlaneStrain/qplanestrain.h"
#include "sm/Elements/graddpelement.h"

#define _IFT_QPlaneStrainGrad_Name "qplanestraingrad"

namespace oofem {
class FEI2dQuadLin;

class QPlaneStrainGrad : public QPlaneStrain, public GradDpElement
{
protected:
    static FEI2dQuadLin interpolation_lin;

public:
    QPlaneStrainGrad(int n, Domain * d);
    virtual ~QPlaneStrainGrad() { }

    void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_QPlaneStrainGrad_Name; }
    const char *giveClassName() const override { return "QPlaneStrainGrad"; }

    MaterialMode giveMaterialMode() override { return _PlaneStrain; }
    int computeNumberOfDofs() override { return 20; }

protected:
     void computeBkappaMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeNkappaMatrixAt(GaussPoint *gp, FloatArray &answer) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override { GradDpElement :: computeStiffnessMatrix(answer, rMode, tStep); }
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override { GradDpElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord); }

    void computeGaussPoints() override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    StructuralElement *giveStructuralElement() override { return this; }
    NLStructuralElement *giveNLStructuralElement() override { return this; }
};
} // end namespace oofem
#endif // qplanestraingrad_h
