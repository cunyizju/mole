// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dquadconst_h
#define fei2dquadconst_h

#include "fei/feinterpol2d.h"

namespace oofem {
/**
 * Class representing a 2d quadrilateral with constant interpolation.
 */
class OOFEM_EXPORT FEI2dQuadConst : public FEInterpolation2d
{
public:
    FEI2dQuadConst(int ind1, int ind2) : FEInterpolation2d(0, ind1, ind2) { }

    integrationDomain giveIntegrationDomain() const override { return _Square; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_quad_1; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Line; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Square; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    // Bulk
    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    bool inside(const FloatArray &lcoords) const override;

    // Edge
    IntArray computeLocalEdgeMapping(int iedge) const override;
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeEvalNormal(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNds(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;

    int giveNumberOfNodes() const override { return 4; }
    void giveCellDofMans(IntArray& nodes, IntArray &internalDofMans, Element* elem) const override {nodes={}; internalDofMans={1};}


protected:
    /**
     * Computes the length of a given edge.
     * @param edgeNodes Node indices of edge.
     * @param cellgeo Cell geometry.
     */
    double edgeComputeLength(const IntArray &edgeNodes, const FEICellGeometry &cellgeo) const;
};
} // end namespace oofem
#endif // fei2dquadlin_h
