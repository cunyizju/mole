// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef fei3dwedgelin_h
#define fei3dwedgelin_h

#include "fei/feinterpol3d.h"

namespace oofem {
/**
 * Class representing implementation of linear wedge interpolation class.
 *
 * @author Milan Jirasek
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI3dWedgeLin : public FEInterpolation3d
{
public:
    FEI3dWedgeLin() : FEInterpolation3d(1) { }

    integrationDomain giveIntegrationDomain() const override { return _Wedge; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_wedge_1; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override
    {
        if (ib <= 2) return _Triangle;
        else return _Square;
    }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return this->giveBoundaryIntegrationDomain(isurf); }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    // Bulk
    static FloatArrayF<6> evalN(const FloatArrayF<3> &lcoords);
    static std::pair<double, FloatMatrixF<3,6>> evaldNdx(const FloatArrayF<3> &lcoords, const FEICellGeometry &cellgeo);
    static FloatMatrixF<3,6> evaldNdxi(const FloatArrayF<3> &lcoords);

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evaldNdxi(FloatMatrix & answer, const FloatArray & lcoords, const FEICellGeometry & cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void giveJacobianMatrixAt(FloatMatrix &jacobianMatrix, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveCharacteristicLength(const FEICellGeometry &cellgeo) const;

    // Edge
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNdx(FloatMatrix &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo)  const override;
    double edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    IntArray computeLocalEdgeMapping(int iedge) const override;

    // Surface
    void surfaceEvalN(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    //void surfaceEvaldNdx (FloatMatrix&answer, int iedge, const FloatArray& lcoords, const FEICellGeometry& cellgeo) override;
    void surfaceLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double surfaceGiveTransformationJacobian(int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    IntArray computeLocalSurfaceMapping(int iSurf) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
    std::unique_ptr<IntegrationRule> giveBoundaryIntegrationRule(int order, int boundary) const override;
    std::unique_ptr<IntegrationRule> giveSurfaceIntegrationRule(int order, int isurf) const
    { return giveBoundaryIntegrationRule(order, isurf); }

    int giveNumberOfNodes() const override { return 6; }

protected:
    double edgeComputeLength(IntArray &edgeNodes, const FEICellGeometry &cellgeo) const;
};
} // end namespace oofem
#endif
