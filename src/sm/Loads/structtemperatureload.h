// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structtemperatureload_h
#define structtemperatureload_h

#include "bc/load.h"

#define _IFT_StructuralTemperatureLoad_Name "structtemperatureload"

namespace oofem {
class Element;
class TimeStep;

/**
 * This class implements temperature (constant) load over the element
 * component array contains one or two numbers;
 * componentArray->at(1) contains increment of temperature in mid-surface
 * componentArray->at(2) contains increment of gradient of temperature
 * over the thickness of element (optional)
 */
class StructuralTemperatureLoad : public Load
{
public:
    StructuralTemperatureLoad(int n, Domain * d) : Load(n, d) { }

    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    bcValType giveBCValType() const override { return TemperatureBVT; }
    bcGeomType giveBCGeoType() const override { return BodyLoadBGT; }
    const char *giveInputRecordName() const override { return _IFT_StructuralTemperatureLoad_Name; }
    const char *giveClassName() const override { return "StructuralTemperatureLoad"; }
};
} // end namespace oofem
#endif // structtemperatureload_h
