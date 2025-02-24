// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef isolinearelasticmaterial_h
#define isolinearelasticmaterial_h

#include "Elasticity/linearelasticmaterial.h"
#include "utility/dictionary.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "math/floatmatrixf.h"
#include "math/floatarrayf.h"

#include "qcmaterialextensioninterface.h"

#include "Misc/mixedpressurematerialextensioninterface.h"

///@name Input fields for IsotropicLinearElasticMaterial
//@{
#define _IFT_IsotropicLinearElasticMaterial_Name "isole"
#define _IFT_IsotropicLinearElasticMaterial_e "e"
#define _IFT_IsotropicLinearElasticMaterial_n "n"
#define _IFT_IsotropicLinearElasticMaterial_talpha "talpha"
//@}

namespace oofem {
class GaussPoint;

/**
 * This class implements an isotropic linear elastic material in a finite element problem.
 * For large deformation analysis it becomes the St. Venant-Kirchoff hyperelasticity model.
 *
 * Tasks:
 * - Returning standard material stiffness matrix for 3d-case.
 *   according to current state determined by using data stored
 *   in Gausspoint.
 * - methods give2dPlaneStressMtrx, givePlaneStrainMtrx, give1dStressMtrx are
 *   introduced since form of this matrices is well known, and for
 *   faster response mainly in linear elastic problems.
 * - Returning a material property (method 'give'). Only for non-standard elements.
 * - Returning real stress state vector(tensor) at gauss point for 3d - case.
 */
class IsotropicLinearElasticMaterial : public LinearElasticMaterial, public QCMaterialExtensionInterface, public MixedPressureMaterialExtensionInterface
{
protected:
    /// Young's modulus.
    double E = 0;
    /// Poisson's ratio.
    double nu = 0;
    /// Shear modulus.
    double G = 0;
    /// Alpha
    double a = 0;

public:
    /**
     * Creates a new IsotropicLinearElasticMaterial class instance
     * with given number belonging to domain d.
     * @param n material model number in domain
     * @param d domain which receiver belongs to
     */
    IsotropicLinearElasticMaterial(int n, Domain *d);
    /**
     * Creates a new IsotropicLinearElasticMaterial class instance
     * with given number belonging to domain d.
     * @param n Material model number in domain.
     * @param d Domain which receiver belongs to.
     * @param E Young modulus.
     * @param nu Poisson ratio.
     */
    IsotropicLinearElasticMaterial(int n, Domain *d, double E, double nu);

    const char *giveClassName() const override { return "IsotropicLinearElasticMaterial"; }
    const char *giveInputRecordName() const override { return _IFT_IsotropicLinearElasticMaterial_Name; }

    /**
     * Initializes receiver according to object description stored in input record.
     * The E modulus (keyword "E"), Poisson ratio ("nu") and coefficient of thermal dilatation
     * alpha ("talpha") are read. The parent class instanciateFrom method is called.
     */
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    /// Initialized fixed size tangents. Called by ctor and initializeFrom.
    void initTangents();

    double give(int aProperty, GaussPoint *gp) const override;

    /// Returns Young's modulus.
    double giveYoungsModulus() const { return E; }

    /// Returns Poisson's ratio.
    double givePoissonsRatio() const { return nu; }

    /// Returns the shear elastic modulus @f$ G = \frac{E}{2(1+\nu)} @f$.
    double giveShearModulus() const override { return G; }

    /// Returns the bulk elastic modulus @f$ K = \frac{E}{3(1-2\nu)} @f$.
    double giveBulkModulus() const { return E / ( 3. * ( 1. - 2. * nu ) ); }

    FloatMatrixF<3,3> givePlaneStressStiffMtrx(MatResponseMode, GaussPoint * gp,
                                               TimeStep * tStep) const override;

    FloatMatrixF<4,4> givePlaneStrainStiffMtrx(MatResponseMode, GaussPoint * gp,
                                               TimeStep * tStep) const override;

    FloatMatrixF<1,1> give1dStressStiffMtrx(MatResponseMode, GaussPoint * gp,
                                            TimeStep * tStep) const override;

    /**
     * Computes bulk modulus from given Young's modulus and Poisson's ratio.
     * @param young Young's modulus (@f$ E @f$).
     * @param nu Poisson's ratio (@f$ \nu @f$).
     * @return Bulk modulus (@f$ K = E/(3*(1-2*nu) @f$).
     */
    static double computeBulkModulusFromYoungAndPoisson(double young, double nu)
    {
        return young / ( 3. * ( 1. - 2. * nu ) );
    }

    /**
     * Computes shear modulus from given Young's modulus and Poisson's ratio.
     * @param young Young's modulus (@f$ E @f$).
     * @param nu Poisson's ratio (@f$ \nu @f$).
     * @return Shear modulus (@f$ G = \frac{E}{2 (1+\nu)} @f$).
     */
    static double computeShearModulusFromYoungAndPoisson(double young, double nu)
    {
        return young / ( 2. * ( 1. + nu ) );
    }

    double giveQcElasticParamneter() override { return E; }
    double giveQcPlasticParamneter() override { return std :: numeric_limits< float > :: infinity(); }

    Interface *giveInterface(InterfaceType t) override {
        if ( t == QCMaterialExtensionInterfaceType ) {
            return static_cast< QCMaterialExtensionInterface * >(this);
        } else if ( t == MixedPressureMaterialExtensionInterfaceType ) {
            return static_cast< MixedPressureMaterialExtensionInterface * >( this );
        } else {
            return nullptr;
        }
    }


    /**
    ** support for mixed u-p formulation
    **
    */

    void giveDeviatoric3dMaterialStiffnessMatrix(FloatMatrix & answer,
                                                 MatResponseMode,
                                                 GaussPoint * gp,
                                                 TimeStep * tStep) override;


    void giveDeviatoricPlaneStrainStiffMtrx(FloatMatrix & answer,
                                            MatResponseMode, GaussPoint * gp,
                                            TimeStep * tStep) override;


    void giveInverseOfBulkModulus(double &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) override { answer  = 3. * ( 1. - 2. * nu ) / E; }

    void giveRealStressVector_3d(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep) override;
    void giveRealStressVector_PlaneStrain(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep) override;

};
} // end namespace oofem
#endif // isolinearelasticmaterial_h
