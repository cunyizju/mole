// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei3dtetlin_h
#define fei3dtetlin_h

#include "fei/feinterpol3d.h"

namespace oofem {
/**
 * Class representing implementation of linear tetrahedra interpolation class.
 */
class OOFEM_EXPORT FEI3dTetLin : public FEInterpolation3d
{
public:
    FEI3dTetLin() : FEInterpolation3d(1) { }

    integrationDomain giveIntegrationDomain() const override { return _Tetrahedra; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_tetra_1; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Triangle; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Triangle; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    // Bulk
    static FloatArrayF<4> evalN(const FloatArrayF<3> &lcoords);
    static std::pair<double, FloatMatrixF<3,4>> evaldNdx(const FEICellGeometry &cellgeo);

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int  global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 4; }
    void giveCellDofMans(IntArray& nodes, IntArray& internalDofMans, Element* elem) const override {nodes={1,2,3,4}; internalDofMans={};}

    // Edge
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNdx(FloatMatrix &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    IntArray computeLocalEdgeMapping(int iedge) const override;

    // Surface
    void surfaceEvalN(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void surfaceEvaldNdx(FloatMatrix &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double surfaceEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void surfaceLocal2global(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double surfaceGiveTransformationJacobian(int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    IntArray computeLocalSurfaceMapping(int iedge) const override;
    double evalNXIntegral(int iEdge, const FEICellGeometry &cellgeo) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
    std::unique_ptr<IntegrationRule> giveBoundaryIntegrationRule(int order, int boundary) const override;

protected:
    double edgeComputeLength(const IntArray &edgeNodes, const FEICellGeometry &cellgeo) const;
};
} // end namespace oofem
#endif // fei3dtetlin_h
