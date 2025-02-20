// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structeigenstrainload_h
#define structeigenstrainload_h

#include "bc/load.h"

#define _IFT_StructuralEigenstrainLoad_Name "structeigenstrainload"

namespace oofem {
class Element;
class TimeStep;

/**
 * This class implements prescribed eigenstrain (stress-free strain). It reads six
 * strain components (xx, yy, zz, yz, zx, xy) in the global coordinate system. 2D, 1D?
 *
 * @author Vit Smilauer
 */
class StructuralEigenstrainLoad : public Load
{
public:
    StructuralEigenstrainLoad(int i, Domain * d) : Load(i, d) { }

    /**
     * Computes components values of eigenstrain field at given point (coordinates given in Global c.s.).
     * taking into account corresponding load time function value while respecting load response mode.
     * @param answer Component values at given point and time.
     * @param tStep Time step representing time.
     * @param coords Integration point global coordinates, which are used to evaluate components values.
     * @param mode Determines response mode.
     */
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    const char *giveInputRecordName() const override { return _IFT_StructuralEigenstrainLoad_Name; }
    const char *giveClassName() const override { return "StructuralEigenstrainLoad"; }
    bcValType giveBCValType() const override { return EigenstrainBVT; }
    bcGeomType giveBCGeoType() const override { return BodyLoadBGT; }
};
} // end namespace oofem
#endif // structeigenstrainload_h
