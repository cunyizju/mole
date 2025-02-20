// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef petscsolver_h
#define petscsolver_h

#include "solvers/sparselinsystemnm.h"
#include "solvers/petscsparsemtrx.h"
#include "solvers/convergedreason.h"

#define _IFT_PetscSolver_Name "petsc"

namespace oofem {
class Domain;
class EngngModel;
class FloatMatrix;

/**
 * Implements the solution of linear system of equation in the form @f$A\cdot x=b@f$ using solvers
 * from PETSc library. Only works with the PETSc sparse matrix implementation.
 */
class OOFEM_EXPORT PetscSolver : public SparseLinearSystemNM
{
public:
    /**
     * Constructor.
     * @param d Domain which solver belongs to.
     * @param m Engineering model which solver belongs to.
     */
    PetscSolver(Domain * d, EngngModel * m);

    virtual ~PetscSolver();

    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    /**
     * Solves the given linear system.
     * @param A Coefficient matrix.
     * @param b Right hand side (PETSC Vec(tor)).
     * @param x Solution array(PETSC Vec(tor)).
     * @return ConvergedReason value.
     */
    ConvergedReason petsc_solve(PetscSparseMtrx &A, Vec b, Vec x);

    const char *giveClassName() const override { return "PetscSolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_Petsc; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return SMT_PetscMtrx; }
};
} // end namespace oofem
#endif // petscsolver_h
