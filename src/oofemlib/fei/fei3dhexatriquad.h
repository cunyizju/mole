// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei3dhexatriquad_h
#define fei3dhexatriquad_h

#include "fei/fei3dhexaquad.h"

namespace oofem {
/**
 * Class representing implementation of tri-quadratic hexahedra interpolation class.
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI3dHexaTriQuad : public FEI3dHexaQuad
{
public:
    FEI3dHexaTriQuad() : FEI3dHexaQuad() { }

    integrationDomain giveIntegrationDomain() const override { return _Cube; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_hexa_27; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Square; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Square; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    // Bulk
    static FloatArrayF<27> evalN(const FloatArrayF<3> &lcoords);
    static std::pair<double, FloatMatrixF<3,27>> evaldNdx(const FloatArrayF<3> &lcoords, const FEICellGeometry &cellgeo);
    static FloatMatrixF<3,27> evaldNdxi(const FloatArrayF<3> &lcoords);

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 27; }

    // Surface
    void surfaceEvalN(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double surfaceEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    //void surfaceEvaldNdx(FloatMatrix&answer, int isurf, const FloatArray& lcoords, const FEICellGeometry& cellgeo) override;
    IntArray computeLocalSurfaceMapping(int iSurf) const override;
    double evalNXIntegral(int iSurf, const FEICellGeometry &cellgeo) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
    std::unique_ptr<IntegrationRule> giveBoundaryIntegrationRule(int order, int boundary) const override;
};
} // end namespace oofem
#endif
