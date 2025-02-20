// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mklpardisosolver_h
#define mklpardisosolver_h

#include "solvers/sparselinsystemnm.h"
#include "solvers/convergedreason.h"

#define _IFT_MKLPardisoSolver_Name "mklpardiso"

namespace oofem {

/**
 * Implements the solution of linear system of equation in the form @f$ A\cdot x=b @f$ using solvers
 * from MKL Pardiso. It will not work with Pardiso 5.0.
 * Only works with the CSC (compressed sparse column) sparse matrix implementation.
 * 
 * @author Mikael Öhman
 */
class OOFEM_EXPORT MKLPardisoSolver : public SparseLinearSystemNM
{
public:
    /**
     * Constructor.
     * @param d Domain which solver belongs to.
     * @param m Engineering model which solver belongs to.
     */
    MKLPardisoSolver(Domain * d, EngngModel * m);
    virtual ~MKLPardisoSolver();

    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    const char *giveClassName() const override { return "MKLPardisoSolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_MKLPardiso; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return SMT_CompCol; }
};
} // end namespace oofem
#endif // mklpardisosolver_h
