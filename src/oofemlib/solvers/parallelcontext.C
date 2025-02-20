// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "solvers/parallelcontext.h"
#include "engng/engngm.h"
#include "math/mathfem.h"
#include "input/unknownnumberingscheme.h"

namespace oofem {
//#define ParallelContext_debug_print

ParallelContext :: ParallelContext(EngngModel *e)
#ifdef __PARALLEL_MODE
    : n2g(), n2l()
#endif
{
    this->emodel = e;
}

ParallelContext :: ~ParallelContext()
{ }

void
ParallelContext :: init(int newDi)
{
    di = newDi;
    ///@todo Should we even do this here? The user of the requested ParallelContext will just set this manually instead.
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        ///@todo This shouldn't be hardcoded to just the default numbering schemes. In fact, this shouldn't even have "prescribed" and "free", just use the given numbering.
        n2g.init( emodel, di, EModelDefaultEquationNumbering() );
        n2l.init( emodel, di, EModelDefaultEquationNumbering() );

 #ifdef __VERBOSE_PARALLEL
        fprintf( stderr, "[%d] ParallelContext :: init - leq:%d, neq:%d, geq:%d\n", emodel->giveRank(), giveNumberOfLocalEqs(), giveNumberOfNaturalEqs(), giveNumberOfGlobalEqs() );
 #endif
    }

#endif
}


int
ParallelContext :: giveNumberOfLocalEqs()
{
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        return n2g.giveNumberOfLocalEqs();
    } else {
#endif
    return emodel->giveNumberOfDomainEquations( di, EModelDefaultEquationNumbering() );

#ifdef __PARALLEL_MODE
}
#endif
}


int
ParallelContext :: giveNumberOfGlobalEqs()
{
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        return n2g.giveNumberOfGlobalEqs();
    } else {
#endif
    return emodel->giveNumberOfDomainEquations( di, EModelDefaultEquationNumbering() );

#ifdef __PARALLEL_MODE
}
#endif
}

int
ParallelContext :: giveNumberOfNaturalEqs()
{
    return emodel->giveNumberOfDomainEquations( di, EModelDefaultEquationNumbering() );
}


bool
ParallelContext :: isLocal(DofManager *dman)
{
#ifdef __PARALLE_MODE
    if ( emodel->isParallel() ) {
        return this->giveN2GMap()->isLocal(dman); // Either map is fine.
    }
#else
    return true;

#endif
}


double
ParallelContext :: localNorm(const FloatArray &src)
{
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        double norm2 = 0.0, norm2_tot;
        int size = src.giveSize();
        Natural2LocalOrdering *n2l = this->giveN2Lmap();
        for ( int i = 0; i < size; i++ ) {
            if ( n2l->giveNewEq(i + 1) ) {
                norm2 += src(i) * src(i);
            }
        }
        MPI_Allreduce( & norm2, & norm2_tot, 1, MPI_DOUBLE, MPI_SUM, this->emodel->giveParallelComm() );
        return sqrt(norm2_tot);
    }
#endif
    return src.computeNorm();
}


double
ParallelContext :: localDotProduct(const FloatArray &a, const FloatArray &b)
{
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        double val = 0.0, val_tot = 0.0;
        int size = a.giveSize();
        Natural2LocalOrdering *n2l = this->giveN2Lmap();
        for ( int i = 0; i < size; i++ ) {
            if ( n2l->giveNewEq(i + 1) ) {
                val += a(i) * b(i);
            }
        }
        MPI_Allreduce( & val, & val_tot, 1, MPI_DOUBLE, MPI_SUM, this->emodel->giveParallelComm() );
        return val_tot;
    }
#endif
    return a.dotProduct(b);
}


double
ParallelContext :: accumulate(double local)
{
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        double global;
        MPI_Allreduce( & local, & global, 1, MPI_DOUBLE, MPI_SUM, this->emodel->giveParallelComm() );
        return global;
    }
#endif
    return local;
}



void
ParallelContext :: accumulate(const FloatArray &local, FloatArray &global)
{
#ifdef __PARALLEL_MODE
    if ( emodel->isParallel() ) {
        int size = local.giveSize();
        global.resize(size);
        ///@todo Why aren't we using the C++ MPI functions?
        MPI_Allreduce( local.givePointer(), global.givePointer(), size, MPI_DOUBLE, MPI_SUM, this->emodel->giveParallelComm() );
    } else {
#endif
    global = local;
#ifdef __PARALLEL_MODE
}
#endif
}
} // end namespace oofem
