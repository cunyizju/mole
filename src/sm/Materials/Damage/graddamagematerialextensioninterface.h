// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef gradientdamagematerialextensioninterface_h
#define gradientdamagematerialextensioninterface_h

#include "utility/interface.h"
#include "material/matresponsemode.h"
#include "math/floatarray.h"

///@name graddpdmaterialextensioninterface
//@{
#define _IFT_GradientDamageMaterialExtensionInterface_l "l"
//@}

namespace oofem {
class GaussPoint;
class TimeStep;
class FloatArray;
class FloatMatrix;




/**
 * Material interface for gradient material models.
 */
class GradientDamageMaterialExtensionInterface : public Interface
{
protected:
    Domain *dom = nullptr;

    /**
     * Initial(user defined) characteristic length of the nonlocal model
     */
    double internalLength = 0.;



public:
    /**
     * Constructor. Creates material with given number, belonging to given domain.
     * @param d Domain to which new material will belong.
     */
    GradientDamageMaterialExtensionInterface(Domain *d);
    /// Destructor.
    virtual ~GradientDamageMaterialExtensionInterface() { }
    /// Left upper block
    virtual void giveGradientDamageStiffnessMatrix_uu(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) = 0;
    /// Left lower block
    virtual void giveGradientDamageStiffnessMatrix_du(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) = 0;
    /// Right upper block
    virtual void giveGradientDamageStiffnessMatrix_ud(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) = 0;
    /// Right lower block
    virtual void giveGradientDamageStiffnessMatrix_dd_NN(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    virtual void giveGradientDamageStiffnessMatrix_dd_BB(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) = 0;
    virtual void giveGradientDamageStiffnessMatrix_dd_BN(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    /// gradient - based giveRealStressVector
    virtual void giveRealStressVectorGradientDamage(FloatArray &answer1, double &answer2, GaussPoint *gp, const FloatArray &totalStrain, double nonlocalDamageDrivningVariable, TimeStep *tStep) { OOFEM_ERROR("not implemented") }
    virtual void giveFirstPKStressVectorGradientDamage(FloatArray &answer1, double &answer2, GaussPoint *gp, const FloatArray &totalStrain, double nonlocalDamageDrivningVariable, TimeStep *tStep) { OOFEM_ERROR("not implemented") }
    virtual void giveCauchyStressVectorGradientDamage(FloatArray &answer1, double &answer2, GaussPoint *gp, const FloatArray &totalStrain, double nonlocalDamageDrivningVariable, TimeStep *tStep) { OOFEM_ERROR("not implemented") }

    virtual void computeInternalForcesRegularizationTerm(double &answer, GaussPoint *gp, TimeStep *tStep)
    { answer = 0.; }
    virtual void computeStiffnessRegularizationTerm(double &answer, GaussPoint *gp, TimeStep *tStep)
    { answer = 0.; }


    virtual void computeLocalDamageDrivingVariable(double &answer, GaussPoint *gp, TimeStep *tStep) = 0;
    virtual void giveNonlocalInternalForces_N_factor(double &answer, double nlddv, GaussPoint *gp, TimeStep *tStep) = 0;
    virtual void giveNonlocalInternalForces_B_factor(FloatArray &answer, const FloatArray &nlddv, GaussPoint *gp, TimeStep *tStep) = 0;

    virtual void initializeFrom(InputRecord &ir);
};

class GradientDamageMaterialStatusExtensionInterface : public Interface
{
public:
    GradientDamageMaterialStatusExtensionInterface();
    /// Destructor.
    virtual ~GradientDamageMaterialStatusExtensionInterface() { }

protected:
    double nonlocalDamageDrivingVariable = 0.;
    double localDamageDrivingVariable = 0.;
    double tempNonlocalDamageDrivingVariable = 0.;
    double tempLocalDamageDrivingVariable = 0.;

    FloatArray nonlocalDamageDrivingVariableGrad;
    FloatArray tempNonlocalDamageDrivingVariableGrad;

public:
    virtual double giveLocalDamageDrivingVariable() { return localDamageDrivingVariable; }
    virtual double giveNonlocalDamageDrivingVariable() { return nonlocalDamageDrivingVariable; }
    virtual double giveTempLocalDamageDrivingVariable() { return tempLocalDamageDrivingVariable; }
    virtual double giveTempNonlocalDamageDrivingVariable() { return tempNonlocalDamageDrivingVariable; }
    const FloatArray &giveTempNonlocalDamageDrivingVariableGrad() const { return tempNonlocalDamageDrivingVariableGrad; }



    virtual void setLocalDamageDrivingVariable(double localDamageDrivingVariable) { this->localDamageDrivingVariable = localDamageDrivingVariable; }
    virtual void setNonlocalDamageDrivingVariable(double nonlocalDamageDrivingVariable) { this->nonlocalDamageDrivingVariable = nonlocalDamageDrivingVariable; }
    virtual void setTempLocalDamageDrivingVariable(double localDamageDrivingVariable) { this->tempLocalDamageDrivingVariable = localDamageDrivingVariable; }
    virtual void setTempNonlocalDamageDrivingVariable(double nonlocalDamageDrivingVariable) { this->tempNonlocalDamageDrivingVariable = nonlocalDamageDrivingVariable; }
    virtual void letTempNonlocalDamageDrivingVariableGradBe(const FloatArray &nonlocalDamageDrivingVariableGrad) { this->tempNonlocalDamageDrivingVariableGrad = nonlocalDamageDrivingVariableGrad; }

    virtual void initTempStatus();
    virtual void updateYourself(TimeStep *tStep);
};
}
#endif
