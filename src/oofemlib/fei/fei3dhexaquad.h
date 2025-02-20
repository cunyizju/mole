// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei3dhexaquad_h
#define fei3dhexaquad_h

#include "fei/feinterpol3d.h"

namespace oofem {
/**
 * Class representing implementation of quadratic hexahedra interpolation class.
 *  ***  numbering like T3d  ***            ***  numbering like T3d  ***
 *  ***  numbering of nodes  ***            ***  numbering of surfs  ***
 *
 *              zeta
 *        1      ^  9         2
 *         +-----|--+--------+                     +-----------------+
 *        /|     |          /|                    /|                /|
 *       / |     |         / |                   / |               / |
 *    12+  |     o      10+  |                  /  |     1        /  |
 *     /   |     |       /   |                 /   |             /   |
 *  4 /  17+  11 |    3 /    +18              /    |       (3)  /    |
 *   +--------+--------+     |               +-----------------+     |
 *   |     |     |     |     |               |     |           |     |
 *   |     |     +-----|--o------> eta       | (6) |           |  4  |
 *   |     |    /   13 |     |               |     |           |     |
 *   |   5 +---/----+--|-----+ 6             |     +-----------|-----+
 * 20+    /   o        +19  /                |    /   5        |    /
 *   |   /   /         |   /                 |   /             |   /
 *   |16+   /          |  +14                |  /       (2)    |  /
 *   | /   /           | /                   | /               | /
 *   |/   L ksi        |/                    |/                |/
 *   +--------+--------+                     +-----------------+
 *  8         15        7
 *
 * @author Ladislav Svoboda
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI3dHexaQuad : public FEInterpolation3d
{
public:
    FEI3dHexaQuad() : FEInterpolation3d(2) { }

    integrationDomain giveIntegrationDomain() const override { return _Cube; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_hexa_2; }
    integrationDomain giveBoundaryIntegrationDomain(int ib) const override { return _Square; }
    integrationDomain giveBoundarySurfaceIntegrationDomain(int isurf) const override { return _Square; }
    integrationDomain giveBoundaryEdgeIntegrationDomain(int iedge) const override { return _Line; }

    double giveCharacteristicLength(const FEICellGeometry &cellgeo) const;

    // Bulk
    static FloatArrayF<20> evalN(const FloatArrayF<3> &lcoords);
    static std::pair<double, FloatMatrixF<3,20>> evaldNdx(const FloatArrayF<3> &lcoords, const FEICellGeometry &cellgeo);
    static FloatMatrixF<3,20> evaldNdxi(const FloatArrayF<3> &lcoords);

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 20; }

    // Edge
    void edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeEvaldNdx(FloatMatrix &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    IntArray computeLocalEdgeMapping(int iedge) const override;

    // Surface
    void surfaceEvalN(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double surfaceEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void surfaceEvaldNdx(FloatMatrix &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void surfaceLocal2global(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double surfaceGiveTransformationJacobian(int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    IntArray computeLocalSurfaceMapping(int iSurf) const override;
    double evalNXIntegral(int iEdge, const FEICellGeometry &cellgeo) const override;

    void giveJacobianMatrixAt(FloatMatrix &jacobianMatrix, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
    std::unique_ptr<IntegrationRule> giveBoundaryIntegrationRule(int order, int boundary) const override;
};
} // end namespace oofem
#endif
