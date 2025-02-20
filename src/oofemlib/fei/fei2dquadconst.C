// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fei/fei2dquadconst.h"
#include "math/mathfem.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/gaussintegrationrule.h"
#include <stdexcept>

namespace oofem {
void
FEI2dQuadConst :: evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    answer.resize(1);
    answer.at(1) = 1.;
}

double
FEI2dQuadConst :: evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    answer.resize(1, 2);
    answer.at(1, 1) = 0.;
    answer.at(1, 2) = 0.;
    return 0.;
}

void
FEI2dQuadConst :: local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    answer.resize(2);

    answer.at(1) = 0.25 * ( cellgeo.giveVertexCoordinates(1).at(xind) +
                            cellgeo.giveVertexCoordinates(2).at(xind) +
                            cellgeo.giveVertexCoordinates(3).at(xind) +
                            cellgeo.giveVertexCoordinates(4).at(xind) );
    answer.at(2) = 0.25 * ( cellgeo.giveVertexCoordinates(1).at(yind) +
                            cellgeo.giveVertexCoordinates(2).at(yind) +
                            cellgeo.giveVertexCoordinates(3).at(yind) +
                            cellgeo.giveVertexCoordinates(4).at(yind) );
}


bool FEI2dQuadConst :: inside(const FloatArray &lcoords) const
{
	const double point_tol = 1.0e-3;
    bool inside = true;
    for ( int i = 1; i <= 2; i++ ) {
        if ( lcoords.at(i) < ( -1. - point_tol ) ) {
            inside = false;
        } else if ( lcoords.at(i) > ( 1. + point_tol ) ) {
            inside = false;
        }
    }

    return inside;
}


void
FEI2dQuadConst :: edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    answer = FloatArray{1.};
}


double FEI2dQuadConst :: edgeEvalNormal(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("not implemented");
    return 0.;
}

void
FEI2dQuadConst :: edgeEvaldNds(FloatArray &answer, int iedge,
                               const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("not implemented");
}

void
FEI2dQuadConst :: edgeLocal2global(FloatArray &answer, int iedge,
                                   const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    OOFEM_ERROR("not implemented");
}

IntArray
FEI2dQuadConst :: computeLocalEdgeMapping(int iedge) const
{
    if ( iedge == 1 ) { // edge between nodes 1 2
        return {1, 2};
    } else if ( iedge == 2 ) { // edge between nodes 2 3
        return {2, 3};
    } else if ( iedge == 3 ) { // edge between nodes 2 3
        return {3, 1};
    } else {
        throw std::range_error("invalid egde number");
        return {};
    }
}

double
FEI2dQuadConst :: edgeComputeLength(const IntArray &edgeNodes, const FEICellGeometry &cellgeo) const
{
    int nodeA = edgeNodes.at(1);
    int nodeB = edgeNodes.at(2);

    double dx = cellgeo.giveVertexCoordinates(nodeB).at(xind) - cellgeo.giveVertexCoordinates(nodeA).at(xind);
    double dy = cellgeo.giveVertexCoordinates(nodeB).at(yind) - cellgeo.giveVertexCoordinates(nodeA).at(yind);
    return sqrt(dx * dx + dy * dy);
}

std::unique_ptr<IntegrationRule>
FEI2dQuadConst :: giveIntegrationRule(int order) const
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    int points = iRule->getRequiredNumberOfIntegrationPoints(_Square, order + 0);
    iRule->SetUpPointsOnSquare(points, _Unknown);
    return std::move(iRule);
}
} // end namespace oofem
