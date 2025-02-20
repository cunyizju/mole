// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei3dtetquad_h
#define fei3dtetquad_h

#include "fei/feinterpol3d.h"

namespace oofem {
/**
 * Class representing implementation of quadratic tetrahedra interpolation class.
 * @author Mikael Öhman
 * @todo This class is entirely unchecked.
 */
class OOFEM_EXPORT FEI3dTetQuad : public FEInterpolation3d
{
public:
    FEI3dTetQuad() : FEInterpolation3d(2) { }

    integrationDomain giveIntegrationDomain() const override { return _Tetrahedra; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_tetra_2; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Triangle; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Triangle; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    double giveVolume(const FEICellGeometry &cellgeo) const override;

    // Bulk
    static FloatArrayF<10> evalN(const FloatArrayF<3> &lcoords);
    static std::pair<double, FloatMatrixF<3,10>> evaldNdx(const FloatArrayF<3> &lcoords, const FEICellGeometry &cellgeo);
    static FloatMatrixF<3,10> evaldNdxi(const FloatArrayF<3> &lcoords);

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void giveJacobianMatrixAt(FloatMatrix &jacobianMatrix, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 10; }
    void giveCellDofMans(IntArray& nodes, IntArray& internalDofMans, Element* elem) const override {nodes={1,2,3,4,5,6,7,8,9,10}; internalDofMans={};}
    /**
     * Returns a characteristic length of the geometry, typically a diagonal or edge length.
     * @param cellgeo Underlying cell geometry.
     * @return Square root of area.
     */
    double giveCharacteristicLength(const FEICellGeometry &cellgeo) const;

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
    double computeVolume(const FEICellGeometry &cellgeo) const ;
};
} // end namespace oofem
#endif // fei3dtetquad_h
