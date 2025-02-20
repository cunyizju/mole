// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef sparsegeneigenvalsystemnm_h
#define sparsegeneigenvalsystemnm_h

#include "input/nummet.h"
#include "solvers/convergedreason.h"

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
class OOFEM_EXPORT SparseGeneralEigenValueSystemNM : public NumericalMethod
{
public:
    /// Constructor.
    SparseGeneralEigenValueSystemNM(Domain * d, EngngModel * m) : NumericalMethod(d, m) { }
    /// Destructor.
    virtual ~SparseGeneralEigenValueSystemNM() { }

    virtual const char *giveClassName() const = 0;
    std :: string errorInfo(const char *func) { return std :: string(this->giveClassName()) + func; }

    /**
     * Solves the given sparse generalized eigen value system of equations @f$ Av = x^2 Bv @f$.
     * @param A coefficient matrix.
     * @param B coefficient matrix.
     * @param v eigen vector(s).
     * @param x eigen value(s).
     * @param rtol tolerance
     * @param nroot number of required eigenvalues.
     * @return NM_Status value.
     */
    virtual ConvergedReason solve(SparseMtrx &A, SparseMtrx &B, FloatArray &x, FloatMatrix &v, double rtol, int nroot) = 0;
};
} // end namespace oofem
#endif // sparsegeneigenvalsystemnm_h
