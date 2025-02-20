// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fei/feinterpol.h"
#include "input/element.h"
#include "math/gaussintegrationrule.h"

namespace oofem {
int FEIElementGeometryWrapper :: giveNumberOfVertices() const { return elem->giveNumberOfNodes(); }

double
FEInterpolation :: giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    FloatMatrix jacobianMatrix;
    this->giveJacobianMatrixAt(jacobianMatrix, lcoords, cellgeo);
    return jacobianMatrix.giveDeterminant();
}


std::unique_ptr<IntegrationRule>
FEInterpolation:: giveIntegrationRule(int order) const
{
    integrationDomain id = this->giveIntegrationDomain();
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);

    int points = iRule->getRequiredNumberOfIntegrationPoints(id, order + this->order);
    iRule->SetUpPointsOnLine(points, _Unknown);
    return std::move(iRule);
}

std::unique_ptr<IntegrationRule>
FEInterpolation::giveBoundaryIntegrationRule(int order, int boundary) const 
{
    integrationDomain id = this->giveBoundaryIntegrationDomain(boundary);
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);

    int points = iRule->getRequiredNumberOfIntegrationPoints(id, order + this->order);
    iRule->setUpIntegrationPoints(id, points, _Unknown);
    return std::move(iRule);
}

std::unique_ptr<IntegrationRule>
FEInterpolation::giveBoundaryEdgeIntegrationRule(int order, int boundary) const 
{
    integrationDomain id = this->giveBoundaryEdgeIntegrationDomain(boundary);
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);

    int points = iRule->getRequiredNumberOfIntegrationPoints(id, order + this->order);
    iRule->setUpIntegrationPoints(id, points, _Unknown);
    return std::move(iRule);
}

std::unique_ptr<IntegrationRule>
FEInterpolation::giveBoundarySurfaceIntegrationRule(int order, int boundary) const
{
    integrationDomain id = this->giveBoundarySurfaceIntegrationDomain(boundary);
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);

    int points = iRule->getRequiredNumberOfIntegrationPoints(id, order + this->order);
    iRule->setUpIntegrationPoints(id, points, _Unknown);
    return std::move(iRule);
}  
  
} // end namespace oofem
