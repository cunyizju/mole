// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "symmetrybarrier.h"
#include "math/intarray.h"
#include "math/floatarray.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_NonlocalBarrier(SymmetryBarrier)

SymmetryBarrier :: SymmetryBarrier(int n, Domain *aDomain) :
    NonlocalBarrier(n, aDomain), origin(), normals(), mask(), lcs(3, 3)
    // Constructor. Creates an element with number n, belonging to aDomain.
{ }


SymmetryBarrier :: ~SymmetryBarrier()
// Destructor.
{ }

void
SymmetryBarrier :: applyConstraint(const double cl, const FloatArray &c1, const FloatArray &c2, double &weight,
                                   bool &shieldFlag, const NonlocalMaterialExtensionInterface &nei)
{
    // compute node coordinates in barrrier lcs
    FloatArray mc2(3), help(3);
    int i, ii, jj, kk, mi, dim = c1.giveSize();
    double d;

    shieldFlag = false;


    for ( i = 1; i <= dim; i++ ) {
        help.at(i) = c2.at(i) - origin.at(i);
    }

    // first compute mirrors to active planes
    // loop over active planes, mirror source point and compute weights
    for ( mi = 1; mi <= 3; mi++ ) {
        if ( mask.at(mi) ) {
            // plane active
            // mirror source
            for ( d = 0.0, i = 1; i <= dim; i++ ) {
                d += help.at(i) * lcs.at(mi, i);
            }

            for ( i = 1; i <= dim; i++ ) {
                mc2.at(i) = c2.at(i) - 2.0 *d *lcs.at(mi, i);
            }

            // compute weight of mirrored source
            weight += nei.computeWeightFunction(cl, c1, mc2);
        }
    }

    // compute mirrors to lines common to two active planes
    for ( mi = 0; mi < 3; mi++ ) {
        ii = mi + 1;
        jj = ( mi + 1 ) % 3 + 1;
        kk = ( mi + 2 ) % 3 + 1;

        if ( mask.at(ii) && mask.at(jj) ) {
            // compute mirror point
            for ( d = 0.0, i = 1; i <= dim; i++ ) {
                d += help.at(i) * lcs.at(kk, i);
            }

            d = sqrt(d);
            for ( i = 1; i <= dim; i++ ) {
                mc2.at(i) = c2.at(i) - 2.0 * ( c2.at(i) - ( origin.at(i) + d * lcs.at(kk, i) ) );
            }

            // compute weight of mirrored source
            weight += nei.computeWeightFunction(cl, c1, mc2);
        }
    }

    // finally compute mirror to origin if all three planes are active
    if ( mask.at(1) && mask.at(2) && mask.at(3) ) {
        // mirror source
        for ( i = 1; i <= dim; i++ ) {
            mc2.at(i) = c2.at(i) - 2.0 * ( c2.at(i) - origin.at(i) );
        }

        // compute weight of mirrored source
        weight += nei.computeWeightFunction(cl, c1, mc2);
    }
}

void
SymmetryBarrier :: initializeFrom(InputRecord &ir)
{
    FloatArray normals;

    IR_GIVE_FIELD(ir, origin, _IFT_SymmetryBarrier_origin);
    IR_GIVE_FIELD(ir, normals, _IFT_SymmetryBarrier_normals);

    lcs.resize(3, 3);
    int size = normals.giveSize();
    if ( !( ( size == 0 ) || ( size == 6 ) ) ) {
        OOFEM_WARNING("lcs in node %d is not properly defined, will be ignored", this->giveNumber() );
    }

    if ( size == 6 ) {
        double n1 = 0.0, n2 = 0.0;
        // compute transformation matrix
        for ( int j = 1; j <= 3; j++ ) {
            lcs.at(1, j) = normals.at(j);
            n1 += normals.at(j) * normals.at(j);
            lcs.at(2, j) = normals.at(j + 3);
            n2 += normals.at(j + 3) * normals.at(j + 3);
        }

        n1 = sqrt(n1);
        n2 = sqrt(n2);
        if ( ( n1 <= 1.e-6 ) || ( n2 <= 1.e-6 ) ) {
            OOFEM_ERROR("lcs input error");
        }

        for ( int j = 1; j <= 3; j++ ) { // normalize e1' e2'
            lcs.at(1, j) /= n1;
            lcs.at(2, j) /= n2;
        }

        // vector e3' computed from vector product of e1', e2'
        lcs.at(3, 1) = lcs.at(1, 2) * lcs.at(2, 3) - lcs.at(1, 3) * lcs.at(2, 2);
        lcs.at(3, 2) = lcs.at(1, 3) * lcs.at(2, 1) - lcs.at(1, 1) * lcs.at(2, 3);
        lcs.at(3, 3) = lcs.at(1, 1) * lcs.at(2, 2) - lcs.at(1, 2) * lcs.at(2, 1);
    }

    IR_GIVE_FIELD(ir, mask, _IFT_SymmetryBarrier_activemask);
    if ( mask.giveSize() != 3 ) {
        throw ValueInputException(ir, _IFT_SymmetryBarrier_activemask, "size must be 3");
    }
}
} // end namespace oofem
