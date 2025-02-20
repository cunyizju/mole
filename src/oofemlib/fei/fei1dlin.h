// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei1dlin_h
#define fei1dlin_h

#include "fei/feinterpol1d.h"

namespace oofem {
/**
 * Class representing a 1d linear isoparametric interpolation.
 */
class OOFEM_EXPORT FEI1dLin : public FEInterpolation1d
{
protected:
    int cindx = 0;

public:
    FEI1dLin(int coordIndx) : FEInterpolation1d(1), cindx(coordIndx) { }

    integrationDomain giveIntegrationDomain() const override { return _Line; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Point; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _UnknownIntegrationDomain; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _UnknownIntegrationDomain; }

    double giveLength(const FEICellGeometry &cellgeo) const override;

    static FloatArrayF<2> evalN(double ksi);
    std::pair<double, FloatMatrixF<1,2>> evaldNdx(const FEICellGeometry &cellgeo) const;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int  global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    int giveNumberOfNodes() const override { return 2; }

    IntArray boundaryEdgeGiveNodes(int boundary) const override;
    void boundaryEdgeEvalN(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double boundaryEdgeGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void boundaryEdgeLocal2Global(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
};
} // end namespace oofem
#endif // fei1dlin_h
