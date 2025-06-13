// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dsssolver.h"
#include "engng/classfactory.h"
#include "dssmatrix.h"
#include "utility/timer.h"

namespace oofem {

REGISTER_SparseLinSolver(DSSSolver, ST_DSS);

DSSSolver :: DSSSolver(Domain *d, EngngModel *m) :
    SparseLinearSystemNM(d, m) { }

DSSSolver :: ~DSSSolver() { }

ConvergedReason
DSSSolver :: solve(SparseMtrx &A, FloatArray &b, FloatArray &x)
{
 #ifdef TIME_REPORT
    Timer timer;
    timer.startTimer();
 #endif


    DSSMatrix *_mtrx = dynamic_cast< DSSMatrix * >(&A);
    if ( _mtrx ) {
        _mtrx->factorized();
        _mtrx->solve(b, x);
    } else {
        OOFEM_ERROR("incompatible sparse mtrx format");
    }

 #ifdef TIME_REPORT
    timer.stopTimer();
    OOFEM_LOG_INFO( "DSSSolver info: user time consumed by solution: %.2fs\n", timer.getUtime() );
 #endif

    return CR_CONVERGED;
}
} // end namespace oofem

