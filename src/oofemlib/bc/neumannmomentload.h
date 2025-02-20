// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef NEUMANNMOMENTLOAD_H
#define NEUMANNMOMENTLOAD_H

#include "bc/boundaryload.h"
#include "input/element.h"

#define _IFT_NeumannMomentLoad_Name "momentload"
#define _IFT_NeumannMomentLoad_Gradient "gradient"
#define _IFT_NeumannMomentLoad_Constant "constant"
#define _IFT_NeumannMomentLoad_CenterSet "cset"

/**
 * This class contains a Neumann type boundary condition given as
 * @f[ t=p+g\cdot[x-\bar{x}]\otimes n @f]
 * where @f$ p @f$ is a prescribed constant (eg pressure), @f$ g @f$ is the gradient (pressure gradient), @f$ x @f$ is the coordinate, @f$ \bar{x} @f$ is the
 * centre of the structure and $n$ is the outward pointing normal.
 */
namespace oofem {
class OOFEM_EXPORT NeumannMomentLoad : public SurfaceLoad
{
private:
    /// Center of structure
    FloatArray xbar;
    /// Set containing elements used to calculate xbar
    int cset;
    /// Array containing elements elements in set cset
    IntArray celements;
    /// Gradient
    FloatArray g;
    /// Constant
    double p;

    /// Compute centre of mass for set cset
    void computeXbar();

    /// Compute normal at center of element
    void computeNormal(FloatArray &answer, Element *e, int side);
public:
    NeumannMomentLoad(int i, Domain * d) : SurfaceLoad(i, d) { }
    virtual ~NeumannMomentLoad() {}

    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;
    int giveApproxOrder() override { return 0; }

    FormulationType giveFormulationType() override { return FT_Global; }

    void computeValueAtBoundary(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode, Element *e, int boundary);
    /**
     * Sets a new load vector.
     * @param newValue New load.
     */
    void updateLoad(const FloatArray &newValue) { componentArray = newValue; }

    void initializeFrom(InputRecord &ir) override;
    bcGeomType giveBCGeoType() const override { return SurfaceLoadBGT; }

    const char *giveClassName() const override { return "NeumannMomentLoad"; }
    const char *giveInputRecordName() const override { return _IFT_NeumannMomentLoad_Name; }

private:
    void computeNArray(FloatArray &answer, const FloatArray &coords) const override { answer.clear(); }
};
} // end namespace oofem


#endif // NEUMANNMOMENTLOAD_H
