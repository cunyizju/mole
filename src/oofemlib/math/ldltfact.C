// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "math/ldltfact.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_SparseLinSolver(LDLTFactorization, ST_Direct)

LDLTFactorization :: LDLTFactorization(Domain *d, EngngModel *m) :
    SparseLinearSystemNM(d, m)
{
}

LDLTFactorization :: ~LDLTFactorization()
{
}

ConvergedReason
LDLTFactorization :: solve(SparseMtrx &A, FloatArray &b, FloatArray &x)
{
    // check whether Lhs supports factorization
    if ( !A.canBeFactorized() ) {
        OOFEM_ERROR("Lhs not support factorization");
    }

    x = b;

    // solving
    A.factorized()->backSubstitutionWith(x);

    return CR_CONVERGED;
}
} // end namespace oofem
