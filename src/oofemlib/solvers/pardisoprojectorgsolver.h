// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef pardisoprojectorgsolver_h
#define pardisoprojectorgsolver_h

#include "solvers/sparselinsystemnm.h"
#include "solvers/convergedreason.h"

/* PARDISO prototype. */
extern "C" void pardisoinit(void *, int *,   int *, int *, double *, int *);
extern "C" void pardiso(void *, int *,   int *, int *,    int *, int *,
                        double *, int *,    int *, int *,   int *, int *,
                        int *, double *, double *, int *, double *);
extern "C" void pardiso_chkmatrix(int *, int *, double *, int *, int *, int *);
extern "C" void pardiso_chkvec(int *, int *, double *, int *);
extern "C" void pardiso_printstats(int *, int *, double *, int *, int *, int *,
                                   double *, int *);

#define _IFT_PardisoProjectOrgSolver_Name "pardiso"

namespace oofem {
/**
 * Implements the solution of linear system of equation in the form @f$ A\cdot x=b @f$ using solvers
 * from Pardiso-project.org. Adapted from mklpardisosolver implementation by M. Ohman
 * Only works with the CSC (compressed sparse column) sparse matrix implementation.
 *
 */
class OOFEM_EXPORT PardisoProjectOrgSolver : public SparseLinearSystemNM
{
public:
    /**
     * Constructor.
     * @param d Domain which solver belongs to.
     * @param m Engineering model which solver belongs to.
     */
    PardisoProjectOrgSolver(Domain *d, EngngModel *m);

    virtual ~PardisoProjectOrgSolver();

    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    const char *giveClassName() const override { return "PardisoProjectOrgSolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_PardisoProjectOrg; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return SMT_CompCol; }
};
} // end namespace oofem
#endif // pardisoprojectorgsolver_h
