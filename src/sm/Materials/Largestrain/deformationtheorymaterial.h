// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef deformationtheorymaterial_h
#define deformationtheorymaterial_h

#include "sm/Materials/Structural/structuralmaterial.h"

namespace oofem {
/**
 * This class implements an abstract base material, which behaves
 * according to deformation theory.
 */
class DeformationTheoryMaterial : public StructuralMaterial
{
public:
    DeformationTheoryMaterial(int n, Domain * d) : StructuralMaterial(n, d) { }

    const char *giveClassName() const override { return "DeformationTheoryMaterial"; }
};
} // end namespace oofem
#endif // deformationtheorymaterial_h
