// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "solvers/sparselinsystemnm.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/sparsemtrx.h"

namespace oofem {
SparseLinearSystemNM :: SparseLinearSystemNM(Domain *d, EngngModel *m) : NumericalMethod(d, m)
{ }

SparseLinearSystemNM :: ~SparseLinearSystemNM()
{ }

ConvergedReason SparseLinearSystemNM :: solve(SparseMtrx &A, FloatMatrix &B, FloatMatrix &X)
{
    ConvergedReason status = CR_UNKNOWN;
    int neq = A.giveNumberOfRows();
    int nrhs = B.giveNumberOfColumns();
    if ( A.giveNumberOfRows() != B.giveNumberOfRows() ) {
        OOFEM_ERROR("A and B matrix mismatch");
    }
    FloatArray bi(neq), xi(neq);
    if ( X.giveNumberOfRows() != neq || X.giveNumberOfColumns() != nrhs ) {
        X.resize(neq, nrhs);
    }
    for ( int i = 1; i <= nrhs; ++i ) {
        B.copyColumn(bi, i);
        X.copyColumn(xi, i);
        status = this->solve(A, bi, xi);
        if ( status != CR_CONVERGED ) {
            return status;
        }
        X.setColumn(xi, i);
    }
    return status;
}
} // end namespace oofem
