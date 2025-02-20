// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fei/feinterpol1d.h"
#include "math/floatarray.h"
#include "math/gaussintegrationrule.h"
#include <stdexcept>

namespace oofem {


void FEInterpolation1d :: boundaryEvalN(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    answer.resize(1);
    answer.at(1) = 1.;
}

IntArray FEInterpolation1d :: boundaryGiveNodes(int boundary) const
{
    throw std::runtime_error("Not implemented");
}

double FEInterpolation1d :: boundaryEvalNormal(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("Not implemented");
    return 1.;
}

double FEInterpolation1d :: boundaryGiveTransformationJacobian(int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    return 1.;
}

void FEInterpolation1d :: boundaryLocal2Global(FloatArray &answer, int boundary, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const 
{
    answer = cellgeo.giveVertexCoordinates(boundary);
}

std::unique_ptr<IntegrationRule> FEInterpolation1d :: giveIntegrationRule(int order) const
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    int points = iRule->getRequiredNumberOfIntegrationPoints(_Line, order + this->order);
    iRule->SetUpPointsOnLine(points, _Unknown);
    return std::move(iRule);
}

std::unique_ptr<IntegrationRule> FEInterpolation1d :: giveBoundaryIntegrationRule(int order, int boundary)  const
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    iRule->SetUpPoint(_Unknown);
    return std::move(iRule);
}

std::unique_ptr<IntegrationRule> FEInterpolation1d :: giveBoundaryEdgeIntegrationRule(int order, int boundary) const
{
    return this->giveIntegrationRule(order);
}
} // end namespace oofem
