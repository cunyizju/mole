// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef sparselinsystemnm_h
#define sparselinsystemnm_h

#include "input/nummet.h"
#include "solvers/convergedreason.h"
#include "solvers/linsystsolvertype.h"
#include "math/sparsemtrxtype.h"

namespace oofem {
class EngngModel;
class SparseMtrx;
class FloatArray;

/**
 * This base class is an abstraction for all numerical methods solving sparse
 * linear system of equations. The purpose of this class is to declare
 * the general interface to all numerical methods solving this kind of
 * problem. This interface allows to use any suitable
 * instance of the Numerical method class to the solve problem,
 * and leave the  whole engineering model code,
 * including mapping, unchanged, because all instances of this class
 * provide the common interface.
 */
class OOFEM_EXPORT SparseLinearSystemNM : public NumericalMethod
{
public:
    /// Constructor.
    SparseLinearSystemNM(Domain * d, EngngModel * m);
    /// Destructor.
    virtual ~SparseLinearSystemNM();

    virtual const char *giveClassName() const = 0;
    std :: string errorInfo(const char *func) { return std :: string(this->giveClassName()) + func; }

    /**
     * @return LinSystSolverType value, corresponding to receiver.
     */
    virtual LinSystSolverType giveLinSystSolverType() const = 0;

    /**
     * Solves the given sparse linear system of equations @f$ A\cdot x=b @f$.
     * @param A Coefficient matrix.
     * @param b Right hand side.
     * @param x Solution array.
     * @return Status of the solver.
     */
    virtual ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) = 0;

    /**
     * Solves the given sparse linear system of equations @f$ A\cdot X=B @f$.
     * Default implementation calls solve multiple times.
     * @param A Coefficient matrix.
     * @param B Right hand side.
     * @param X Solution matrix.
     * @return Status of the solver.
     */
    virtual ConvergedReason solve(SparseMtrx &A, FloatMatrix &B, FloatMatrix &X);
    /**
     * Returns the recommended sparse matrix type for this solver.
     */
    virtual SparseMtrxType giveRecommendedMatrix(bool symmetric) const = 0;
};
} // end namespace oofem
#endif // sparselinsystemnm_h
