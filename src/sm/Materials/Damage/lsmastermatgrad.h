// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef lsmastermatgrad_h
#define lsmastermatgrad_h

#include "Largestrain/lsmastermat.h"
#include "../sm/Materials/Structural/structuralmaterial.h"
#include "../sm/Materials/Structural/structuralms.h"
#include "Materials/Elasticity/linearelasticmaterial.h"
#include "utility/dictionary.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "Damage/graddamagematerialextensioninterface.h"

namespace oofem {
class GaussPoint;
class Domain;


#define _IFT_LargeStrainMasterMaterialGrad_Name "lsmastermatgrad"


/**
 * This class implements an gradient version of LargeStrainMasterMaterial
 */
class LargeStrainMasterMaterialGrad : public LargeStrainMasterMaterial, GradientDamageMaterialExtensionInterface
{
public:
    LargeStrainMasterMaterialGrad(int n, Domain *d);

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

    void giveGradientDamageStiffnessMatrix_uu(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveGradientDamageStiffnessMatrix_du(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveGradientDamageStiffnessMatrix_ud(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;
    void giveGradientDamageStiffnessMatrix_dd_BB(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override;

    void computeLocalDamageDrivingVariable(double &answer, GaussPoint *gp, TimeStep *tStep) override;
    void giveNonlocalInternalForces_N_factor(double &answer, double nlddv, GaussPoint *gp, TimeStep *tStep) override;
    void giveNonlocalInternalForces_B_factor(FloatArray &answer, const FloatArray &nlddv, GaussPoint *gp, TimeStep *tStep) override;

    void give3dKappaMatrix(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void give3dGprime(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    void giveInternalLength(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    const char *giveClassName() const override { return "LargeStrainMasterMaterialGrad"; }

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

    virtual void giveFirstPKStressVectorGrad(FloatArray &answer1, double &answer2, GaussPoint *gp, const FloatArray &totalStrain, double nonlocalDamageDrivningVariable, TimeStep *tStep);
};
} // end namespace oofem
#endif // misesmat_h
