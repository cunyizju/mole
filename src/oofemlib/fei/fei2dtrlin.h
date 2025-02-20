// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dtrlin_h
#define fei2dtrlin_h

#include "fei/feinterpol2d.h"

namespace oofem {
/**
 * Class representing a 2d triangular linear interpolation based on area coordinates.
 */
class OOFEM_EXPORT FEI2dTrLin : public FEInterpolation2d
{
public:
    FEI2dTrLin(int ind1, int ind2) : FEInterpolation2d(1, ind1, ind2) { }

    integrationDomain giveIntegrationDomain() const override { return _Triangle; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Line; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Triangle; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    Element_Geometry_Type giveGeometryType() const override { return EGT_triangle_1; }

    // Bulk
    static FloatArrayF<3> evalN(const FloatArrayF<2> &lcoords) ;
    std::pair<double,FloatMatrixF<2,3>> evaldNdx(const FEICellGeometry &cellgeo) const;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &gcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveArea(const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 3; }
 
    bool inside(const FloatArray &lcoords) const override;

    // Edge
    IntArray computeLocalEdgeMapping(int iedge) const override;
    int giveNumberOfEdges() const override { return 3; }
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeEvalNormal(FloatArray &normal, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNds(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evalNXIntegral(int iEdge, const FEICellGeometry &cellgeo) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;

protected:
    double edgeComputeLength(const IntArray &edgeNodes, const FEICellGeometry &cellgeo) const;
};

/**
 * Class representing a 2d isoparametric linear interpolation based on natural coordinates
 * for triangular elements in axisymmetric setting.
 */
class OOFEM_EXPORT FEI2dTrLinAxi : public FEI2dTrLin
{
public:
    FEI2dTrLinAxi(int ind1, int ind2) : FEI2dTrLin(ind1, ind2) { }

    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double boundaryEdgeGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double boundaryGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

};

} // end namespace oofem
#endif // fei2dtrlin_h
