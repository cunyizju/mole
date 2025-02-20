// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef rankinepm_h
#define rankinepm_h

#include "mplasticmaterial.h"

///@name Input fields for RankinePlasticMaterial
//@{
#define _IFT_RankinePlasticMaterial_Name "rankine"
#define _IFT_RankinePlasticMaterial_ry "ry"
//@}

namespace oofem {
class Domain;

/**
 * This class implements a isotropic  plastic linear material (J2 plasticity condition is used)
 * in a finite element problem.
 */
class RankinePlasticMaterial : public MPlasticMaterial
{
protected:
    /// Yield value.
    double k = 0.;

public:
    RankinePlasticMaterial(int n, Domain * d);

    void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_RankinePlasticMaterial_Name; }
    const char *giveClassName() const override { return "RankinePlasticMaterial"; }

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

protected:

    //
    // yield(YC-like functions) and loading(LC-like functions) criteria specific section
    //

    double computeYieldValueAt(GaussPoint *gp, int isurf, const FloatArray &stressVector,
                               const FloatArray &stressSpaceHardeningVars) const override;

    void computeHardeningReducedModuli(FloatMatrix &answer,
                                       GaussPoint *gp,
                                       const FloatArray &strainSpaceHardeningVariables,
                                       TimeStep *tStep) const override;
    void computeStressGradientVector(FloatArray &answer, functType ftype, int isurf, GaussPoint *gp, const FloatArray &stressVector,
                                     const FloatArray &stressSpaceHardeningVars) const override;
    void computeStressSpaceHardeningVarsReducedGradient(FloatArray &answer, functType ftype, int isurf, GaussPoint *gp,
                                                        const FloatArray &stressVector,
                                                        const FloatArray &stressSpaceHardeningVars) const override;
    int hasHardening() const override { return 0; }
    void computeReducedGradientMatrix(FloatMatrix &answer, int isurf,
                                      GaussPoint *gp,
                                      const FloatArray &stressVector,
                                      const FloatArray &stressSpaceHardeningVars) const override;

    void computeStressSpaceHardeningVars(FloatArray &answer, GaussPoint *gp,
                                         const FloatArray &strainSpaceHardeningVariables) const override;
};
} // end namespace oofem
#endif // rankinepm_h
