// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qtrplanestrainp1_h
#define qtrplanestrainp1_h

#include "../sm/Elements/PlaneStrain/qtrplanestrain.h"
#include "../sm/Elements/MixedPressure/basemixedpressureelement.h"

#define _IFT_QTrPlaneStrainP1_Name "qtrplanestrainp1"


namespace oofem {
class FEI2dTrLin;

class QTrPlaneStrainP1 : public QTrPlaneStrain, public BaseMixedPressureElement
{
protected:
    static FEI2dTrLin interpolation_lin;

public:
    QTrPlaneStrainP1(int n, Domain *d);
    virtual ~QTrPlaneStrainP1() { }

protected:
    void computePressureNMatrixAt(GaussPoint *, FloatArray &) override;
    void computeVolumetricBmatrixAt(GaussPoint *gp, FloatArray &Bvol, NLStructuralElement *element) override;
    NLStructuralElement *giveElement() override { return this; }
public:
    const char *giveClassName() const override { return "QTrPlaneStrainP1"; }
    const char *giveInputRecordName() const override { return _IFT_QTrPlaneStrainP1_Name; }


    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    void giveDofManDofIDMask_u(IntArray &answer) override;
    void giveDofManDofIDMask_p(IntArray &answer) override;


    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode mode, TimeStep *tStep) override { BaseMixedPressureElement :: computeStiffnessMatrix(answer, mode, tStep); }
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord) override { BaseMixedPressureElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord); }


    int giveNumberOfPressureDofs() override { return 3; }
    int giveNumberOfDisplacementDofs() override { return 12; }
    int giveNumberOfDofs() override { return 15; }
    void postInitialize() override;
};
} // end namespace oofem
#endif // qtrplanestrainp1_h
