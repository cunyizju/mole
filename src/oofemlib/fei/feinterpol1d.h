// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef feinterpol1d_h
#define feinterpol1d_h

#include "fei/feinterpol.h"
#include <stdexcept>

namespace oofem {
/**
 * Class representing a general abstraction for finite element interpolation class.
 */
class OOFEM_EXPORT FEInterpolation1d : public FEInterpolation
{
public:
    FEInterpolation1d(int o) : FEInterpolation(o) { }
    int giveNsd() const override { return 1; }

    //FloatArray giveParametricCenter() const override { return {0.}; }

    IntArray boundaryGiveNodes(int boundary) const override;
    void boundaryEvalN(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double boundaryEvalNormal(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const  override;
    double boundaryGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void boundaryLocal2Global(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    /**@name Surface interpolation services */
    //@{
    void boundarySurfaceEvalN(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    void boundarySurfaceEvaldNdx(FloatMatrix &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    double boundarySurfaceEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    void boundarySurfaceLocal2global(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    double boundarySurfaceGiveTransformationJacobian(int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    IntArray boundarySurfaceGiveNodes(int boundary) const override
    { throw std::runtime_error("Functions not supported for this interpolator."); }
    //@}

    /**@name Edge interpolation services */
    void boundaryEdgeEvalN(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    double boundaryEdgeEvalNormal(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    double boundaryEdgeGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    void boundaryEdgeLocal2Global(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override
    { OOFEM_ERROR("Functions not supported for this interpolator."); }
    //@}

    /**
     * Computes the exact length.
     * @param cellgeo Cell geometry for the element.
     * @return Length of geometry.
     */
    virtual double giveLength(const FEICellGeometry &cellgeo) const
    {
        OOFEM_ERROR("Not implemented in subclass.");
        return 0;
    }

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
    std::unique_ptr<IntegrationRule> giveBoundaryIntegrationRule(int order, int boundary) const override;
    std::unique_ptr<IntegrationRule> giveBoundaryEdgeIntegrationRule(int order, int boundary) const override;
};
} // end namespace oofem
#endif // feinterpol1d_h
