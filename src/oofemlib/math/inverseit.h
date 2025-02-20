// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef inverseit_h
#define inverseit_h

#include "math/sparsegeneigenvalsystemnm.h"
#include "solvers/convergedreason.h"
#include "math/floatarray.h"

#define _IFT_InverseIteration_Name "inverseit"

namespace oofem {
class Domain;
class EngngModel;

/**
 * This class implements the class NumericalMethod instance Generalized Jacobi
 * Eigen Value Problem Solver
 *
 * DESCRIPTION :
 * Perform solution of eigen value problem in the form
 * K y = (omega)^2 M y
 *
 * TASKS :
 *
 * - solving problem
 *   solveYourselfAt.
 * - returning results (eigen values and associated eigen vectors).
 *
 * Variable description  :
 *
 *       A(N,N)    = STIFFNESS MATRIX (ASSUMED POZITIVE DEFINITE)        *
 *       B(N,N)    = MASS MATRIX (ASSUMED POZITIVE DEFINITE)             *
 *       X(N,N)    = MATRIX STORING EIGENVECTORS ON SOLUTION EXIT        *
 *       EIGV(N)   = VECTOR STORING EIGENVALUES ON SOLUTION EXIT         *
 *       D(N)      = WORKING VECTOR                                      *
 *       N         = ORDER OF WORKING AREA MATRICES A AND B              *
 *       RTOL      = CONVERGENCE TOLERANCE (USUALLY SET TO 10.**-12)     *
 *       NSMAX     = MAXIMUM NUMBER OF SWEEPS ALLOVED                    *
 *                                 (USUALLY SET TO 15)                   *
 *
 * OUTPUT : (after call solveYourselfAt)
 *       A(N,N)    = DIAGONALIZED STIFFNESS MATRIX                       *
 *       B(N,N)    = DIAGONALIZED MASS MATRIX                            *
 *       X(N,N)    = EIGENVECTORS STORED COLUMNWISE                      *
 *       EIGV(N)   = EIGENVALUES                                         *
 *
 *
 */
class OOFEM_EXPORT InverseIteration : public SparseGeneralEigenValueSystemNM
{
private:
    /// Max number of iterations
    int nitem;

public:
    InverseIteration(Domain * d, EngngModel * m);
    virtual ~InverseIteration() {}

    ConvergedReason solve(SparseMtrx &A, SparseMtrx &B, FloatArray &x, FloatMatrix &v, double rtol, int nroot) override;
    const char *giveClassName() const override { return "InverseIteration"; }
};
} // end namespace oofem
#endif // inverseit_h
