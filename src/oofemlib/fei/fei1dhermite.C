// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fei/fei1dhermite.h"
#include "math/mathfem.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/floatmatrixf.h"
#include "math/floatarrayf.h"

namespace oofem {

void
FEI1dHermite :: evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    double ksi = lcoords.at(1);
    double l = this->giveLength(cellgeo);

    answer.resize(4);
    answer.zero();

    answer.at(1) = 0.25 * ( 1.0 - ksi ) * ( 1.0 - ksi ) * ( 2.0 + ksi );
    answer.at(2) =  0.125 * l * ( 1.0 - ksi ) * ( 1.0 - ksi ) * ( 1.0 + ksi );
    answer.at(3) = 0.25 * ( 1.0 + ksi ) * ( 1.0 + ksi ) * ( 2.0 - ksi );
    answer.at(4) = -0.125 * l * ( 1.0 + ksi ) * ( 1.0 + ksi ) * ( 1.0 - ksi );
}

std::pair<double, FloatMatrixF<1,4>>
FEI1dHermite :: evaldNdx(double ksi, const FEICellGeometry &cellgeo) const
{
    double l = this->giveLength(cellgeo);
    double l_inv = 1.0 / l;
    FloatMatrixF<1,4> ans = {
        1.5 * ( -1.0 + ksi * ksi ) * l_inv,
        0.25 * ( ksi - 1.0 ) * ( 1.0 + 3.0 * ksi ),
        -1.5 * ( -1.0 + ksi * ksi ) * l_inv,
        0.25 * ( ksi - 1.0 ) * ( 1.0 + 3.0 * ksi )
    };
    return {0.5 * l, ans};
}

double
FEI1dHermite :: evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    double l = this->giveLength(cellgeo);
    double l_inv = 1.0 / l;
    double ksi = lcoords.at(1);

    answer.resize(1, 4);
    answer.zero();

    answer.at(1, 1) =  1.5 * ( -1.0 + ksi * ksi ) * l_inv;
    answer.at(1, 2) =  0.25 * ( ksi - 1.0 ) * ( 1.0 + 3.0 * ksi );
    answer.at(1, 3) = -1.5 * ( -1.0 + ksi * ksi ) * l_inv;
    answer.at(1, 4) =  0.25 * ( ksi - 1.0 ) * ( 1.0 + 3.0 * ksi );

    return 0.5 * l;
}

FloatMatrixF<1,4>
FEI1dHermite :: evald2Ndx2(double ksi, const FEICellGeometry &cellgeo) const
{
    double l_inv = 1.0 / this->giveLength(cellgeo);

    return {
        l_inv * 6.0 * ksi * l_inv,
        l_inv * ( 3.0 * ksi - 1.0 ),
        -l_inv * 6.0 * ksi * l_inv,
        l_inv * ( 3.0 * ksi + 1.0 )
    };
}

void
FEI1dHermite :: evald2Ndx2(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    //answer = evald2Ndx2(lcoords[0]);
    double l_inv = 1.0 / this->giveLength(cellgeo);
    double ksi = lcoords.at(1);
    answer.resize(1, 4);
    answer.zero();

    answer.at(1, 1) =  l_inv * 6.0 * ksi * l_inv;
    answer.at(1, 2) =  l_inv * ( 3.0 * ksi - 1.0 );
    answer.at(1, 3) = -l_inv * 6.0 * ksi * l_inv;
    answer.at(1, 4) =  l_inv * ( 3.0 * ksi + 1.0 );
}

void
FEI1dHermite :: local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    FloatArray n;
    this->evalN(n, lcoords, cellgeo);

    answer.resize(1);
    answer.at(1) = n.at(1) * cellgeo.giveVertexCoordinates(1).at(cindx) +
                   n.at(2) * cellgeo.giveVertexCoordinates(2).at(cindx) +
                   n.at(3) * cellgeo.giveVertexCoordinates(3).at(cindx);
}

int
FEI1dHermite :: global2local(FloatArray &answer, const FloatArray &coords, const FEICellGeometry &cellgeo) const
{
    double x1 = cellgeo.giveVertexCoordinates(1).at(cindx);
    double x2 = cellgeo.giveVertexCoordinates(2).at(cindx);

    double ksi = ( 2.0 * coords.at(1) - ( x1 + x2 ) ) / ( x2 - x1 );
    answer.resize(1);
    answer.at(1) = clamp(ksi, -1., 1.);
    return fabs(ksi) <= 1.0;
}

double
FEI1dHermite :: giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const
{
    // This isn't really relevant, interpolation of geometry will be just linear
    double l = this->giveLength(cellgeo);
    return 0.5 * l;
}

double
FEI1dHermite :: giveLength(const FEICellGeometry &cellgeo) const
{
    return fabs( cellgeo.giveVertexCoordinates(2).at(cindx) - cellgeo.giveVertexCoordinates(1).at(cindx) );
}
} // end namespace oofem
