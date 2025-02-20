// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fei/feinterpol3d.h"
#include "math/floatarray.h"
#include "math/gaussintegrationrule.h"

namespace oofem {
double FEInterpolation3d :: giveVolume(const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("Not implemented in subclass.");
    return 0;
}

IntArray FEInterpolation3d :: boundaryEdgeGiveNodes(int boundary) const
{
    return this->computeLocalEdgeMapping(boundary);
}

void FEInterpolation3d :: boundaryEdgeEvalN(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    this->edgeEvalN(answer, boundary, lcoords, cellgeo);
}

double FEInterpolation3d :: boundaryEdgeGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    return this->edgeGiveTransformationJacobian(boundary, lcoords, cellgeo);
}

void FEInterpolation3d :: boundaryEdgeLocal2Global(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    this->edgeLocal2global(answer, boundary, lcoords, cellgeo);
}

IntArray FEInterpolation3d :: boundaryGiveNodes(int boundary) const
{
    return this->computeLocalSurfaceMapping(boundary);
}

void FEInterpolation3d :: boundaryEvalN(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    this->surfaceEvalN(answer, boundary, lcoords, cellgeo);
}

double FEInterpolation3d :: boundaryEvalNormal(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    return this->surfaceEvalNormal(answer, boundary, lcoords, cellgeo);
}

double FEInterpolation3d :: boundaryGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    return this->surfaceGiveTransformationJacobian(boundary, lcoords, cellgeo);
}

void FEInterpolation3d :: boundaryLocal2Global(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    return this->surfaceLocal2global(answer, boundary, lcoords, cellgeo);
}

IntArray FEInterpolation3d :: computeEdgeMapping(const IntArray &elemNodes, int iedge) const
{
    const auto &ln = this->computeLocalEdgeMapping(iedge);
    int size = ln.giveSize();
    IntArray edgeNodes(size);
    for ( int i = 1; i <= size; i++ ) {
        edgeNodes.at(i) = elemNodes.at( ln.at(i) );
    }
    return edgeNodes;
}

IntArray FEInterpolation3d :: computeSurfaceMapping(const IntArray &elemNodes, int isurf) const
{
    const auto &ln = this->computeLocalSurfaceMapping(isurf);
    int size = ln.giveSize();
    IntArray surfNodes(size);
    for ( int i = 1; i <= size; i++ ) {
        surfNodes.at(i) = elemNodes.at( ln.at(i) );
    }
    return surfNodes;
}

std::unique_ptr<IntegrationRule> FEInterpolation3d :: giveBoundaryEdgeIntegrationRule(int order, int boundary) const
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    int points = iRule->getRequiredNumberOfIntegrationPoints(_Line, order + this->order);
    iRule->SetUpPointsOnLine(points, _Unknown);
    return iRule;
}

void FEInterpolation3d :: edgeEvaldNdxi(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("Not implemented");
}

void FEInterpolation3d :: surfaceEvaldNdx(FloatMatrix &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("Not implemented");
}

double FEInterpolation3d :: surfaceEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("Not implemented");
    return -1.0;
}

double FEInterpolation3d :: edgeEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("Not implemented");
    return -1.0;
}

IntArray FEInterpolation3d::boundarySurfaceGiveNodes(int boundary) const
{
    return this->computeLocalSurfaceMapping(boundary);
}
  
} // end namespace oofem
