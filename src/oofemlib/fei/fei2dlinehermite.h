// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dlinehermite_h
#define fei2dlinehermite_h

#include "fei/feinterpol2d.h"

namespace oofem {
/**
 * Class representing a 2d line with Hermitian interpolation.
 * The order used is cubic, quadratic, cubic, quadratic.
 * The functions that need geometric information, a linear interpolation is assumed (for geometry). This means functions such as evaldNdx.
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI2dLineHermite : public FEInterpolation2d
{
public:
    FEI2dLineHermite(int ind1, int ind2) : FEInterpolation2d(1, ind1, ind2) { }

    integrationDomain giveIntegrationDomain() const override { return _Line; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }

    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Point; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _UnknownIntegrationDomain; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _UnknownIntegrationDomain; }


    double giveArea(const FEICellGeometry &cellgeo) const override { return 0.0; }
    double giveLength(const FEICellGeometry &cellgeo) const;

    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &gcoords, const FEICellGeometry &cellgeo) const override;

    // "Bulk"
    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    // Edge (same as bulk for this type, so they are all ignored) (perhaps do it the other way around?).
    IntArray computeLocalEdgeMapping(int iedge) const override { return {}; }
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override { }
    double edgeEvalNormal(FloatArray &normal, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNds(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvald2Nds2(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const ;

    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override { }

    int giveNumberOfNodes() const override { return 2; }

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
};
} // end namespace oofem
#endif // fei2dlinehermite_h
