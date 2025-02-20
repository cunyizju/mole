// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef feinurbs_h
#define feinurbs_h

#include "feibspline.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "math/mathfem.h"

///@name Input fields for NURBSInterpolation
//@{
#define _IFT_NURBSInterpolation_weights "weights"
//@}

namespace oofem {
/**
 * Interpolation class for NURBS.
 */
class OOFEM_EXPORT NURBSInterpolation : public BSplineInterpolation
{
protected:
    FloatArray weights;

public:
    NURBSInterpolation(int nsd) : BSplineInterpolation(nsd) { }

    void initializeFrom(InputRecord &ir) override;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override {
        OOFEM_ERROR("Not yet implemented.");
        return 0;
    }
    void giveJacobianMatrixAt(FloatMatrix &jacobianMatrix, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    const char *giveClassName() const { return "NURBSInterpolation"; }
};
} // end namespace oofem
#endif // feinurbs_h
