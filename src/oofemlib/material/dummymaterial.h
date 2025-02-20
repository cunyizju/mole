// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef emptymaterial_h
#define emptymaterial_h

#include "material/material.h"

#define _IFT_DummyMaterial_Name "dummymat"

namespace oofem {
class GaussPoint;
class Domain;
class InputRecord;

/**
 * Dummy material model, no functionality. Convenient for special-purpose elements not
 * requiring real material.
 */
class OOFEM_EXPORT DummyMaterial : public Material
{
public:
    DummyMaterial(int n, Domain * d) : Material(n, d) { }

    bool hasMaterialModeCapability(MaterialMode mode) const override { return false; }

    const char *giveClassName() const override { return "DummyMaterial"; }
    const char *giveInputRecordName() const override { return _IFT_DummyMaterial_Name; }
    void initializeFrom(InputRecord &ir) override { }
};
} // end namespace oofem
#endif // material_h
