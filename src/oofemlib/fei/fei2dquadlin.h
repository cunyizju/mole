// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dquadlin_h
#define fei2dquadlin_h

#include "fei/feinterpol2d.h"

namespace oofem {
/**
 * Class representing a 2d isoparametric linear interpolation based on natural coordinates
 * for quadrilateral elements.
 */
class OOFEM_EXPORT FEI2dQuadLin : public FEInterpolation2d
{
public:
    FEI2dQuadLin(int ind1, int ind2) : FEInterpolation2d(1, ind1, ind2) { }

    integrationDomain giveIntegrationDomain() const override { return _Square; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_quad_1; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Line; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Square; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    double giveArea(const FEICellGeometry &cellgeo) const override;

    // Bulk
    static FloatArrayF<4> evalN(const FloatArrayF<2> &lcoords) ;
    static FloatMatrixF<2,4> evaldNdxi(const FloatArrayF<2> &lcoords);
    std::pair<double, FloatMatrixF<2,4>> evaldNdx(const FloatArrayF<2> &lcoords, const FEICellGeometry &cellgeo) const;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 4; }
    void giveCellDofMans(IntArray& nodes, IntArray& internalDofMans, Element* elem) const override {nodes={1,2,3,4};internalDofMans={};}


    bool inside(const FloatArray &lcoords) const override;

    // Edge
    IntArray computeLocalEdgeMapping(int iedge) const override;
    int giveNumberOfEdges() const override { return 4; }
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeEvalNormal(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNds(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evalNXIntegral(int iEdge, const FEICellGeometry &cellgeo) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;

protected:
    double edgeComputeLength(const IntArray &edgeNodes, const FEICellGeometry &cellgeo) const;
};

/**
 * Class representing a 2d isoparametric linear interpolation based on natural coordinates
 * for quadrilateral elements in axisymmetric setting.
 */
class OOFEM_EXPORT FEI2dQuadLinAxi : public FEI2dQuadLin
{
public:
    FEI2dQuadLinAxi(int ind1, int ind2) : FEI2dQuadLin(ind1, ind2) { }

    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double boundaryEdgeGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double boundaryGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
};

} // end namespace oofem
#endif // fei2dquadlin_h
