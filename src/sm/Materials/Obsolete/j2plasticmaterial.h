// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef j2plasticmaterial_h
#define j2plasticmaterial_h

#include "plasticmaterial.h"

///@name Input fields for J2plasticMaterial
//@{
#define _IFT_J2plasticMaterial_Name "j2mat"
#define _IFT_J2plasticMaterial_ry "ry"
#define _IFT_J2plasticMaterial_khm "khm"
#define _IFT_J2plasticMaterial_ihm "ihm"
//@}

namespace oofem {
class Domain;

/**
 * This class implements a isotropic  plastic linear material (J2 plasticity condition is used)
 * in a finite element problem. A material
 * is an attribute of a domain. It is usually also attribute of many elements.
 */
class J2plasticMaterial : public PlasticMaterial
{
protected:
    int kinematicHardeningFlag = 0, isotropicHardeningFlag = 0;
    double kinematicModuli = 0., isotropicModuli = 0.;
    //double E = 0., nu = 0.; // isotropic material constants
    double k = 0.;

public:
    J2plasticMaterial(int n, Domain * d);

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    const char *giveInputRecordName() const override { return _IFT_J2plasticMaterial_Name; }
    const char *giveClassName() const override { return "J2plasticMaterial"; }

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

protected:

    //
    // yield(YC-like functions) and loading(LC-like functions) criteria specific section
    //

    FloatArray *ComputeStressSpaceHardeningVars(GaussPoint *gp,
                                                FloatArray *strainSpaceHardeningVariables) const override;
    double computeYieldValueAt(GaussPoint *gp, FloatArray *stressVector,
                               FloatArray *stressSpaceHardeningVars) const override;
    void computeHardeningReducedModuli(FloatMatrix &answer, GaussPoint *gp,
                                       FloatArray *strainSpaceHardeningVariables,
                                       TimeStep *tStep) const override;
    FloatArray *ComputeStressGradient(GaussPoint *gp, FloatArray *stressVector,
                                      FloatArray *stressSpaceHardeningVars) const override;
    FloatArray *ComputeStressSpaceHardeningVarsReducedGradient(GaussPoint *gp,
                                                               FloatArray *stressVector,
                                                               FloatArray *stressSpaceHardeningVars) const override;
    int hasHardening() const override;
    void computeReducedGradientMatrix(FloatMatrix &answer, GaussPoint *gp,
                                      const FloatArray &stressVector,
                                      const FloatArray &stressSpaceHardeningVars) const override;
    void computeTrialStressIncrement(FloatArray &answer, GaussPoint *gp,
                                     const FloatArray &strainIncrement, TimeStep *tStep) const override;
    void compute3dElasticModuli(FloatMatrix &answer, GaussPoint *gp, TimeStep *tStep) const override;

    // auxiliary function
    double computeJ2InvariantAt(FloatArray *answer) const;
    int giveSizeOfFullHardeningVarsVector() const override;
    int giveSizeOfReducedHardeningVarsVector(GaussPoint *gp) const override;
    double giveIsotropicHardeningVar(FloatArray *stressSpaceHardeningVars) const;
    void giveStressBackVector(FloatArray &answer, const FloatArray &stressSpaceHardeningVars) const;
};
} // end namespace oofem
#endif // j2plasticmaterial_h
