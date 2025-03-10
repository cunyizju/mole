// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/3D/lspacebb.h"
#include "fei/fei3dhexalin.h"
#include "math/gausspoint.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(LSpaceBB);

LSpaceBB :: LSpaceBB(int n, Domain *aDomain) : LSpace(n, aDomain)
{ }

void
LSpaceBB :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
// Returns the [6x24] strain-displacement matrix {B} of the receiver, eva-
// luated at gp.
// B matrix  -  6 rows : epsilon-X, epsilon-Y, epsilon-Z, gamma-YZ, gamma-ZX, gamma-XY  :
{
    FloatMatrix dnx, dnx0;
    FloatArray coord(3);

    answer.resize(6, 24);
    answer.zero();
    coord.zero();


    LSpace :: interpolation.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    LSpace :: interpolation.evaldNdx( dnx0, coord, FEIElementGeometryWrapper(this) );

    // deviatoric part fully integrated, volumetric part in one point
    // here we follow BBar approach
    //
    // construct BB = B(gp) + Pv [B(0)-B(gp)]
    // where Pv is volumetric projection mtrx
    // B(gp) is original geometrical matrix evalueated at gp
    // B(0)  is geometrical matrix evalueated at centroid
    //
    // assemble Pv [B(0)-B(gp)]
    for ( int i = 1; i <= 8; i++ ) {
        answer.at(1, 3 * i - 2) = answer.at(2, 3 * i - 2) = answer.at(3, 3 * i - 2) = ( dnx0.at(i, 1) - dnx.at(i, 1) ) / 3.0;
        answer.at(1, 3 * i - 1) = answer.at(2, 3 * i - 1) = answer.at(3, 3 * i - 1) = ( dnx0.at(i, 2) - dnx.at(i, 2) ) / 3.0;
        answer.at(1, 3 * i - 0) = answer.at(2, 3 * i - 0) = answer.at(3, 3 * i - 0) = ( dnx0.at(i, 3) - dnx.at(i, 3) ) / 3.0;
    }

    // add B(gp)
    for ( int i = 1; i <= 8; i++ ) {
        answer.at(1, 3 * i - 2) += dnx.at(i, 1);
        answer.at(2, 3 * i - 1) += dnx.at(i, 2);
        answer.at(3, 3 * i - 0) += dnx.at(i, 3);
    }

    for ( int i = 1; i <= 8; i++ ) {
        answer.at(4, 3 * i - 1) += dnx.at(i, 3);
        answer.at(4, 3 * i - 0) += dnx.at(i, 2);

        answer.at(5, 3 * i - 2) += dnx.at(i, 3);
        answer.at(5, 3 * i - 0) += dnx.at(i, 1);

        answer.at(6, 3 * i - 2) += dnx.at(i, 2);
        answer.at(6, 3 * i - 1) += dnx.at(i, 1);
    }
}
} // end namespace oofem
