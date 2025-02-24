// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fei/fei2dquadbiquad.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/floatarrayf.h"
#include "math/floatmatrixf.h"
#include "math/gaussintegrationrule.h"

namespace oofem {

FloatArrayF<9>
FEI2dQuadBiQuad :: evalN(const FloatArrayF<2> &lcoords)
{
    double u = lcoords[0];
    double v = lcoords[1];

    std::array<double, 3> a = {0.5 * ( u - 1.0 ) * u, 1.0 - u * u, 0.5 * ( u + 1.0 ) * u};
    std::array<double, 3> b = {0.5 * ( v - 1.0 ) * v, 1.0 - v * v, 0.5 * ( v + 1.0 ) * v};

    return {
        a [ 0 ] * b [ 0 ],
        a [ 2 ] * b [ 0 ],
        a [ 2 ] * b [ 2 ],
        a [ 0 ] * b [ 2 ],
        a [ 1 ] * b [ 0 ],
        a [ 2 ] * b [ 1 ],
        a [ 1 ] * b [ 2 ],
        a [ 0 ] * b [ 1 ],
        a [ 1 ] * b [ 1 ],
    };
}

void
FEI2dQuadBiQuad :: evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    double u, v;

    u = lcoords.at(1);
    v = lcoords.at(2);

    std::array<double, 3> a = {0.5 * ( u - 1.0 ) * u, 1.0 - u * u, 0.5 * ( u + 1.0 ) * u};
    std::array<double, 3> b = {0.5 * ( v - 1.0 ) * v, 1.0 - v * v, 0.5 * ( v + 1.0 ) * v};

    answer.resize(9);
    answer.at(1) = a [ 0 ] * b [ 0 ];
    answer.at(5) = a [ 1 ] * b [ 0 ];
    answer.at(2) = a [ 2 ] * b [ 0 ];

    answer.at(8) = a [ 0 ] * b [ 1 ];
    answer.at(9) = a [ 1 ] * b [ 1 ];
    answer.at(6) = a [ 2 ] * b [ 1 ];

    answer.at(4) = a [ 0 ] * b [ 2 ];
    answer.at(7) = a [ 1 ] * b [ 2 ];
    answer.at(3) = a [ 2 ] * b [ 2 ];
}


FloatMatrixF<2,9>
FEI2dQuadBiQuad :: evaldNdxi(const FloatArrayF<2> &lc) 
{
    double u = lc[0];
    double v = lc[1];

    std::array<double, 3> a = {0.5 * ( u - 1.0 ) * u, 1.0 - u * u, 0.5 * ( u + 1.0 ) * u};
    std::array<double, 3> b = {0.5 * ( v - 1.0 ) * v, 1.0 - v * v, 0.5 * ( v + 1.0 ) * v};

    std::array<double, 3> da = {u - 0.5, -2.0 * u, u + 0.5};
    std::array<double, 3> db = {v - 0.5, -2.0 * v, v + 0.5};

    return {
        da [ 0 ] * b [ 0 ],
        a [ 0 ] * db [ 0 ],
        da [ 2 ] * b [ 0 ],
        a [ 2 ] * db [ 0 ],
        da [ 2 ] * b [ 2 ],
        a [ 2 ] * db [ 2 ],
        da [ 0 ] * b [ 2 ],
        a [ 0 ] * db [ 2 ],
        da [ 1 ] * b [ 0 ],
        a [ 1 ] * db [ 0 ],
        da [ 2 ] * b [ 1 ],
        a [ 2 ] * db [ 1 ],
        da [ 1 ] * b [ 2 ],
        a [ 1 ] * db [ 2 ],
        da [ 0 ] * b [ 1 ],
        a [ 0 ] * db [ 1 ],
        da [ 1 ] * b [ 1 ],
        a [ 1 ] * db [ 1 ],
    };
}


void
FEI2dQuadBiQuad :: evaldNdxi(FloatMatrix &dN, const FloatArray &lc, const FEICellGeometry &cellgeo) const
{
    double u = lc.at(1);
    double v = lc.at(2);

    double a[] = {
        0.5 * ( u - 1.0 ) * u, 1.0 - u * u, 0.5 * ( u + 1.0 ) * u
    };
    double b[] = {
        0.5 * ( v - 1.0 ) * v, 1.0 - v * v, 0.5 * ( v + 1.0 ) * v
    };

    double da[] = {
        u - 0.5, -2.0 * u, u + 0.5
    };
    double db[] = {
        v - 0.5, -2.0 * v, v + 0.5
    };

    dN.resize(9, 2);

    dN.at(1, 1) = da [ 0 ] * b [ 0 ];
    dN.at(5, 1) = da [ 1 ] * b [ 0 ];
    dN.at(2, 1) = da [ 2 ] * b [ 0 ];
    dN.at(8, 1) = da [ 0 ] * b [ 1 ];
    dN.at(9, 1) = da [ 1 ] * b [ 1 ];
    dN.at(6, 1) = da [ 2 ] * b [ 1 ];
    dN.at(4, 1) = da [ 0 ] * b [ 2 ];
    dN.at(7, 1) = da [ 1 ] * b [ 2 ];
    dN.at(3, 1) = da [ 2 ] * b [ 2 ];

    dN.at(1, 2) = a [ 0 ] * db [ 0 ];
    dN.at(5, 2) = a [ 1 ] * db [ 0 ];
    dN.at(2, 2) = a [ 2 ] * db [ 0 ];
    dN.at(8, 2) = a [ 0 ] * db [ 1 ];
    dN.at(9, 2) = a [ 1 ] * db [ 1 ];
    dN.at(6, 2) = a [ 2 ] * db [ 1 ];
    dN.at(4, 2) = a [ 0 ] * db [ 2 ];
    dN.at(7, 2) = a [ 1 ] * db [ 2 ];
    dN.at(3, 2) = a [ 2 ] * db [ 2 ];
}


std::pair<double, FloatMatrixF<2,9>>
FEI2dQuadBiQuad :: evaldNdx(const FloatArrayF<2> &lcoords, const FEICellGeometry &cellgeo) const
{
    auto dn = evaldNdxi(lcoords);
    FloatMatrixF<2,2> jacT;
    for ( std::size_t i = 1; i <= dn.cols(); i++ ) {
        const auto &c = cellgeo.giveVertexCoordinates(i);
        double x = c.at(xind);
        double y = c.at(yind);

        ///@todo check transpose
        jacT(0, 0) += dn.at(1, i) * x;
        jacT(0, 1) += dn.at(1, i) * y;
        jacT(1, 0) += dn.at(2, i) * x;
        jacT(1, 1) += dn.at(2, i) * y;
    }

    return {det(jacT), dot(inv(jacT), dn)};
}


std::unique_ptr<IntegrationRule> FEI2dQuadBiQuad :: giveIntegrationRule(int order) const
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    int points = iRule->getRequiredNumberOfIntegrationPoints(_Square, order + 6);
    iRule->SetUpPointsOnSquare(points, _Unknown);
    return std::move(iRule);
}
} // end namespace oofem
