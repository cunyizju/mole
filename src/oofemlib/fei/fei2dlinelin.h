// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dlinelin_h
#define fei2dlinelin_h

#include "fei/feinterpol2d.h"

namespace oofem {
/**
 * Class representing a 2d line with linear interpolation.
 * @todo{Some more routines to be implemented here}
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI2dLineLin : public FEInterpolation2d
{
public:
    FEI2dLineLin(int ind1, int ind2) : FEInterpolation2d(1, ind1, ind2) { }

    integrationDomain giveIntegrationDomain() const override { return _Line; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Point; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _UnknownIntegrationDomain; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    double giveArea(const FEICellGeometry &cellgeo) const override { return 0.0; }

    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &gcoords, const FEICellGeometry &cellgeo) const override;

    int giveNumberOfEdges() const override { return 1; }

    // "Bulk"
    static FloatArrayF<2> evalN(double xi);

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    // Edge (same as bulk for this type, so they are all ignored) (perhaps do it the other way around?).
    IntArray boundaryEdgeGiveNodes(int boundary) const override;
    IntArray computeLocalEdgeMapping(int iedge) const override;
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeEvalNormal(FloatArray &normal, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNds(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override { }
    double evalNXIntegral(int iEdge, const FEICellGeometry &cellgeo) const override;

    int giveNumberOfNodes() const override { return 2; }

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;

protected:
    double edgeComputeLength(const IntArray &edgeNodes, const FEICellGeometry &cellgeo) const;
};
} // end namespace oofem
#endif // fei2dlinelin_h
