// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef simplevitrificationmaterial_h
#define simplevitrificationmaterial_h

#include "sm/Materials/Structural/structuralmaterial.h"
#include "sm/Materials/Structural/structuralms.h"
#include "math/floatarray.h"

///@name Input fields for SimpleVitrificationMaterial
//@{
#define _IFT_SimpleVitrificationMaterial_Name "simplevitrificationmaterial"
#define _IFT_SimpleVitrificationMaterial_vitrificationTime "vitrificationtime" ///< Describes the time where the material switches response.
#define _IFT_SimpleVitrificationMaterial_E "e"
#define _IFT_SimpleVitrificationMaterial_nu "nu"
#define _IFT_SimpleVitrificationMaterial_G "g"
#define _IFT_SimpleVitrificationMaterial_alpha "alpha"
#define _IFT_SimpleVitrificationMaterial_E_r "e_r"
#define _IFT_SimpleVitrificationMaterial_nu_r "nu_r"
#define _IFT_SimpleVitrificationMaterial_G_r "g_r"
#define _IFT_SimpleVitrificationMaterial_alpha_r "alpha_r"
//@}

namespace oofem {
/**
 * Model describing the vitrification process of a glass like material.
 * The model is characterized by a linear anisotropic response followed by switch in tangent stiffness as the material vitrifies.
 *
 * @author Tomasz Garstka
 * @author Mikael Öhman
 */
class SimpleVitrificationMaterial : public StructuralMaterial
{
private:
    /// Vitrification time (when equal or larger than this time the material changes response).
    double vitrTime = 0.;
    /// Material parameters for the glassy part of the model (after vitrification).
    FloatArray E, nu, G, alpha;
    /// Material parameters for the rubbery part of the model (before vitrification).
    FloatArray E_r, nu_r, G_r, alpha_r;

public:
    /// Constructor.
    SimpleVitrificationMaterial(int n, Domain * d) : StructuralMaterial(n, d) { }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    int checkConsistency() override;

    FloatMatrixF<6,6> give3dMaterialStiffnessMatrix(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const override;

    FloatArrayF<6> giveRealStressVector_3d(const FloatArrayF<6> &strain, GaussPoint *gp, TimeStep *tStep) const override;

    FloatMatrixF<6,6> computeTangent(bool vitr) const;
                                 
    FloatArrayF<6> giveThermalDilatationVector(GaussPoint *gp, TimeStep *tStep) const override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

    const char *giveClassName() const override { return "SimpleVitrificationMaterial"; }
    const char *giveInputRecordName() const override { return _IFT_SimpleVitrificationMaterial_Name; }
};
} // end namespace oofem
#endif // simplevitrificationmaterial_h
