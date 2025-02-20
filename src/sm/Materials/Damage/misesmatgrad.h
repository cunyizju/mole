// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef MisesMatGrad_h

#include "Materials/Plasticity/misesmat.h"
#include "Damage/graddamagematerialextensioninterface.h"
#include "input/cltypes.h"

///@name Input fields for MisesMatGrad
//@{
#define _IFT_MisesMatGrad_Name "misesmatgrad"
#define _IFT_MisesMatGrad_l "l"
#define _IFT_MisesMatGrad_m "m"
//@}

namespace oofem {
/**
 * Gradient Mises maaterial status.
 */
class MisesMatGradStatus : public MisesMatStatus, GradientDamageMaterialStatusExtensionInterface
{
protected:
    double localCumPlastStrainForAverage = 0.;

public:
    MisesMatGradStatus(GaussPoint *g);

    void printOutputAt(FILE *file, TimeStep *tStep) const override;

    // definition
    const char *giveClassName() const override { return "MisesMatGradStatus"; }

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;

    double giveNonlocalCumulatedStrain() const { return nonlocalDamageDrivingVariable; }
    void setNonlocalCumulatedStrain(double nonlocalCumulatedStrain) { this->nonlocalDamageDrivingVariable = nonlocalCumulatedStrain; }
};


/**
 * Gradient Mises material.
 */
class MisesMatGrad : public MisesMat, GradientDamageMaterialExtensionInterface
{
protected:
    double L = 0.;
    double mParam = 0.;

public:
    MisesMatGrad(int n, Domain *d);

    // definition
    const char *giveInputRecordName() const override { return _IFT_MisesMatGrad_Name; }
    const char *giveClassName() const override { return "MisesMatGrad"; }

    void initializeFrom(InputRecord &ir) override;
    bool hasMaterialModeCapability(MaterialMode mode) const override;

    Interface *giveInterface(InterfaceType t) override {
        if ( t == GradientDamageMaterialExtensionInterfaceType ) {
            return static_cast< GradientDamageMaterialExtensionInterface * >( this );
        } else {
            return nullptr;
        }
    }

    void giveStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;

    FloatMatrixF<1,1> give1dStressStiffMtrx(MatResponseMode, GaussPoint * gp, TimeStep * tStep) const override;
    FloatMatrixF<4,4> givePlaneStrainStiffMtrx(MatResponseMode, GaussPoint * gp, TimeStep * tStep) const override;
    FloatMatrixF<6,6> give3dMaterialStiffnessMatrix(MatResponseMode, GaussPoint * gp, TimeStep * tStep) const override;

    void giveGradientDamageStiffnessMatrix_uu(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveGradientDamageStiffnessMatrix_du(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveGradientDamageStiffnessMatrix_ud(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveGradientDamageStiffnessMatrix_dd_BB(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveRealStressVectorGradientDamage(FloatArray &answer1, double &answer2, GaussPoint *gp, const FloatArray &totalStrain, double nonlocalCumulatedStrain, TimeStep *tStep) override;


    void give1dKappaMatrix(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void givePlaneStrainKappaMatrix(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void give3dKappaMatrix(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);

    void give1dGprime(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void givePlaneStrainGprime(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void give3dGprime(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);

    void giveInternalLength(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);


    void computeLocalDamageDrivingVariable(double &answer, GaussPoint *gp, TimeStep *tStep) override;
    void giveNonlocalInternalForces_N_factor(double &answer, double nlddv, GaussPoint *gp, TimeStep *tStep) override;
    void giveNonlocalInternalForces_B_factor(FloatArray &answer, const FloatArray &nlddv, GaussPoint *gp, TimeStep *tStep) override;

    double computeCumPlastStrain(GaussPoint *gp, TimeStep *tStep) const override;
    void performPlasticityReturn(GaussPoint *gp, const FloatArray &totalStrain);
    //    LinearElasticMaterial *giveLinearElasticMaterial() { return linearElasticMaterial; }

protected:
    MaterialStatus *CreateStatus(GaussPoint *gp) const override { return new MisesMatGradStatus(gp); }
};
} // end namespace oofem
#define MisesMatGrad_h
#endif
