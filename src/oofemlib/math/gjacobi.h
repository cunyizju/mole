// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef gjacobi_h
#define gjacobi_h

#include "input/nummet.h"
#include "math/floatarray.h"
#include "solvers/convergedreason.h"

namespace oofem {
class Domain;
class EngngModel;

/**
 * This class implements the Generalized Jacobi Eigenvalue Problem Solver.
 * Perform solution of eigenvalue problem in the form
 * @f[
 *  K\cdot x = \omega^2 M\cdot x
 * @f]
 */
class OOFEM_EXPORT GJacobi : public NumericalMethod
{
private:
    int nsmax;
    double rtol;

public:
    GJacobi(Domain * d, EngngModel * m);
    virtual ~GJacobi();

    /**
     * Solves the given sparse generalized eigenvalue system of equations @f$ K\cdot x = w^2 M\cdot x @f$.
     * @param K Coefficient matrix.
     * @param M Coefficient matrix.
     * @param x Eigenvector(s).
     * @param w Eigenvalue(s).
     * @return Status.
     */
    ConvergedReason solve(FloatMatrix &K, FloatMatrix &M, FloatArray &w, FloatMatrix &x);

    const char *giveClassName() const { return "GeneralizedJacobiSolver"; }
    std :: string errorInfo(const char *func) { return std :: string(this->giveClassName()) + func; }
};
} // end namespace oofem
#endif // gjacobi_h
