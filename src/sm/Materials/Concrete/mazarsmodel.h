// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mazarsmodel_h
#define mazarsmodel_h

#include "sm/Materials/Elasticity/linearelasticmaterial.h"
#include "sm/Materials/Concrete/idm1.h"
#include "sm/Materials/Structural/structuralms.h"

///@name Input fields for MazarsMaterial
//@{
#define _IFT_MazarsMaterial_Name "mazarsmodel"
#define _IFT_MazarsMaterial_version "version"
#define _IFT_MazarsMaterial_e0 "e0"
#define _IFT_MazarsMaterial_ac "ac"
#define _IFT_MazarsMaterial_bc "bc"
#define _IFT_MazarsMaterial_beta "beta"
#define _IFT_MazarsMaterial_at "at"
#define _IFT_MazarsMaterial_bt "bt"
#define _IFT_MazarsMaterial_ef "ef"
#define _IFT_MazarsMaterial_r "r"
#define _IFT_MazarsMaterial_hreft "hreft"
#define _IFT_MazarsMaterial_hrefc "hrefc"
//@}

namespace oofem {
/**
 * This class implements associated Material Status to MazarsMaterial.
 */
class MazarsMaterialStatus : public IsotropicDamageMaterial1Status
{
protected:
    /// Characteristic element length for compression, fixed as square from element size (for 2d).
    double lec = 0.;

public:
    /// Constructor.
    MazarsMaterialStatus(GaussPoint * g);

    /// Returns characteristic length stored in receiver.
    double giveLec() { return lec; }
    /// Sets characteristic length to given value.
    void setLec(double ls) { lec = ls; }

    const char *giveClassName() const override { return "MazarsMaterialStatus"; }

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};


/**
 * This class implements a Mazars damage model form concrete.
 * It introduces two damage parameters omega_t and omega_c that
 * are computed from the same equivalent strain using two different damage functions
 * g_t and g_c. The g_t is identified from the uniaxial tension tests, while
 * g_c from compressive test. The damage parameter for general stress states
 * omega is obtained as a linear combination of omega_t and omega_c.
 */
class MazarsMaterial : public IsotropicDamageMaterial1
{
protected:
    /// Elastic parameters.
    double E = 0., nu = 0.;
    /// Model parameters related to the shape of uniaxial stress-strain diagrams.
    double At = 0., Bt = 0., Ac = 0., Bc = 0.;
    /// Reference elem-length for objectivity.
    double hReft = 0., hRefc = 0.;
    /// Beta coefficient reducing the effect of shear; default val = 1.06.
    double beta = 0.;

    /// Model variants.
    enum mazarsModelVariant { maz_original, maz_modTension } modelVersion;

public:
    /// Constructor
    MazarsMaterial(int n, Domain * d);

    const char *giveInputRecordName() const override { return _IFT_MazarsMaterial_Name; }
    const char *giveClassName() const override { return "MazarsMaterial"; }

    void initializeFrom(InputRecord &ir) override;

    double computeEquivalentStrain(const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) const override;
    double computeDamageParam(double kappa, const FloatArray &strain, GaussPoint *gp) const override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override { return new MazarsMaterialStatus(gp); }

protected:
    /**
     *  Perfoms initialization, when damage first appear. The Le characteristic length is
     *  computed from the direction of largest positive principal strain and stored
     *  in corresponding status.
     *  @param kappa Scalar measure of strain level,
     *  @param totalStrainVector Current total strain vector,
     *  @param gp Integration point,
     */
    void initDamaged(double kappa, FloatArray &totalStrainVector, GaussPoint *gp) const override;
    /*
     * Computes elastic stiffness for normal stress components.
     * @param answer Result of size (3,3).
     * @param mode Determines the MatResponseMode.
     * @param gp Integration point.
     * @param tStep Time step.
     */
    /*
     * void giveNormalElasticStiffnessMatrix(FloatMatrix &answer,
     *                                    MatResponseMode rMode,
     *                                    GaussPoint *gp, TimeStep *tStep);
     */

    int giveNumberOfSpatialDimensions(GaussPoint *gp) const;
    void giveNormalBlockOfElasticCompliance(FloatMatrix &answer, GaussPoint *gp) const;
    double computeGt(double kappa, GaussPoint *gp) const;
    double computeGc(double kappa, GaussPoint *gp) const;
};
} // end namespace oofem
#endif // mazarsmodel_h
