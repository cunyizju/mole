// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

//   ************************************************************************************
//   *** CLASS ROTATING SMEARED CRACK MODEL WITH TRANSITION TO SCALAR DAMAGE ************
//   ************************************************************************************

#ifndef rcsd_h
#define rcsd_h

#include "Damage/rcm2.h"

///@name Input fields for RCSDMaterial
//@{
#define _IFT_RCSDMaterial_Name "rcsd"
#define _IFT_RCSDMaterial_sdtransitioncoeff "sdtransitioncoeff"
//@}

namespace oofem {
#define rcsd_Omega 300
#define pscm_SDTransitionCoeff 306
#define RCSD_DAMAGE_EPS 1.e-4

class GaussPoint;

/**
 * This class implements associated Material Status to RCSDMaterial
 */
class RCSDMaterialStatus : public RCM2MaterialStatus
{
public:
    enum rcsdMode { rcMode, sdMode };

protected:
    double maxEquivStrain = 0., tempMaxEquivStrain = 0.;
    double damageCoeff = 1., tempDamageCoeff = 1.;
    FloatMatrix Ds0;
    double damageStiffCoeff = 0., depsf = 0., depsp = 0.;
    rcsdMode mode = rcMode, tempMode = rcMode;

public:
    RCSDMaterialStatus(GaussPoint * g);

    void printOutputAt(FILE *file, TimeStep *tStep) const override;

    double giveTempMaxEquivStrain() const { return tempMaxEquivStrain; }
    void setTempMaxEquivStrain(double val) { tempMaxEquivStrain = val; }
    double giveDamageStiffCoeff() const { return damageStiffCoeff; }
    void setDamageStiffCoeff(double val) { damageStiffCoeff = val; }
    double giveTempDamageCoeff() const { return tempDamageCoeff; }
    void setTempDamageCoeff(double val) { tempDamageCoeff = val; }
    const FloatMatrix *giveDs0Matrix() { return & Ds0; }
    void setDs0Matrix(FloatMatrix &mtrx) { Ds0 = mtrx; }
    double giveDamageEpsfCoeff() const { return depsf; }
    void setDamageEpsfCoeff(double val) { depsf = val; }
    double giveDamageEpspCoeff() const { return depsp; }
    void setDamageEpspCoeff(double val) { depsp = val; }

    rcsdMode giveTempMode() const { return tempMode; }
    void setTempMode(rcsdMode mode) { tempMode = mode; }

    // query for non-tem variables (usefull for postprocessing)
    double giveMaxEquivStrain() const { return maxEquivStrain; }
    double giveDamageCoeff() const { return damageCoeff; }

    rcsdMode giveMode() const { return mode; }

    const char *giveClassName() const override { return "RCSDMaterialStatus"; }

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};


/**
 * This class implements a Rotating Crack Model with transition to scalar damage
 * for fracture in smeared fashion
 * ( only material stiffness modification is required, no changes in
 * mesh topology).
 * Model according to Milan Jirasek RC-SD model.
 */
class RCSDMaterial : public RCM2Material
{
protected:
    double SDTransitionCoeff = 0.;

public:
    RCSDMaterial(int n, Domain * d);
    virtual ~RCSDMaterial();

    const char *giveInputRecordName() const override { return _IFT_RCSDMaterial_Name; }
    const char *giveClassName() const override { return "RCSDMaterial"; }

    void initializeFrom(InputRecord &ir) override;

    double give(int aProperty, GaussPoint *gp) const override;

    void giveRealStressVector(FloatArray &answer, GaussPoint *gp,
                              const FloatArray &, TimeStep *tStep) override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override { return new RCSDMaterialStatus(gp); }

protected:
    double computeCurrEquivStrain(GaussPoint *, const FloatArray &, double, TimeStep *);
    // two functions used to initialize and updating temporary variables in
    // gp's status. These variables are used to control process, when
    // we try to find equilibrium state.

    void giveEffectiveMaterialStiffnessMatrix(FloatMatrix &answer,
                                              MatResponseMode rMode,
                                              GaussPoint *gp, TimeStep *tStep) override;

    double computeDamageCoeff(double, double, double, double);
    double giveCrackingModulus(MatResponseMode rMode, GaussPoint *gp,
                               double crackStrain, int i) override;
    //double giveShearRetentionFactor(GaussPoint* gp, double eps_cr, int i) override;
    double giveNormalCrackingStress(GaussPoint *gp, double eps_cr, int i) override;
    double giveMinCrackStrainsForFullyOpenCrack(GaussPoint *gp, int i) override;
    //void updateStatusForNewCrack( GaussPoint*, int, double) override;
    double computeStrength(GaussPoint *gp, double) override;
    int checkSizeLimit(GaussPoint *gp, double) override;
};
} // end namespace oofem
#endif // rcsd_h
