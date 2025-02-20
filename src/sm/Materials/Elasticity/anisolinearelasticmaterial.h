// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef anisolinearelasticmaterial_h
#define anisolinearelasticmaterial_h

#include "Elasticity/linearelasticmaterial.h"
#include "utility/dictionary.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "material/matconst.h"
#include "input/element.h"

///@name Input fields for AnisotropicLinearElasticMaterial
//@{
#define _IFT_AnisotropicLinearElasticMaterial_Name "anisole"
/// Stiffness coefficients arranged by rows from the diagonal to the right (21 values)
#define _IFT_AnisotropicLinearElasticMaterial_stiff "stiff"
/// Thermal expansion, 6 components in strain-Voigt order,
#define _IFT_AnisotropicLinearElasticMaterial_talpha "talpha"
//@}

namespace oofem {
class GaussPoint;

/**
 * This class implements a general anisotropic linear elastic material in a finite
 * element problem.
 */
class AnisotropicLinearElasticMaterial : public LinearElasticMaterial
{
public:
    AnisotropicLinearElasticMaterial(int n, Domain *d) : LinearElasticMaterial(n, d) {}

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    const char *giveInputRecordName() const override { return _IFT_AnisotropicLinearElasticMaterial_Name; }
    const char *giveClassName() const override { return "AnisotropicLinearElasticMaterial"; }
};
} // end namespace oofem
#endif // anisolinearelasticmaterial_h
