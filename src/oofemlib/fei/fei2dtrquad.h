// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dtrquad_h
#define fei2dtrquad_h

#include "fei/feinterpol2d.h"

namespace oofem {
/**
 * Second order triangular interpolation in 2D (6 nodes).
 */
class OOFEM_EXPORT FEI2dTrQuad : public FEInterpolation2d
{
public:
    FEI2dTrQuad(int ind1, int ind2) : FEInterpolation2d(2, ind1, ind2) { }

    integrationDomain giveIntegrationDomain() const override { return _Triangle; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_triangle_2; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Line; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Triangle; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    // Bulk
    static FloatArrayF<6> evalN(const FloatArrayF<2> &lcoords);
    static FloatMatrixF<2,6> evaldNdxi(const FloatArrayF<2> &lcoords);
    std::pair<double,FloatMatrixF<2,6>> evaldNdx(const FloatArrayF<2> &lcoords, const FEICellGeometry &cellgeo) const;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evald2Ndx2(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveArea(const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 6; }

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

    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

protected:
    double edgeComputeLength(IntArray &edgeNodes, const FEICellGeometry &cellgeo);
};
} // end namespace oofem
#endif // fei2dtrquad_h
