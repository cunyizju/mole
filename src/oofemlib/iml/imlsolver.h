// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef imlsolver_h
#define imlsolver_h

#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrx.h"
#include "solvers/convergedreason.h"
#include "math/floatarray.h"
#include "precond.h"

#include <memory>

///@name Input fields for IMLSolver
//@{
#define _IFT_IMLSolver_Name "iml"
#define _IFT_IMLSolver_stype "stype"
#define _IFT_IMLSolver_lstol "lstol"
#define _IFT_IMLSolver_lsiter "lsiter"
#define _IFT_IMLSolver_lsprecond "lsprecond"
//@}

namespace oofem {
class Domain;
class EngngModel;
class FloatMatrix;

/**
 * Implements the solution of linear system of equation in the form @f$ A\cdot x=b @f$ using iterative solvers
 * from IML++ library. Can work with any sparse matrix implementation.
 */
class OOFEM_EXPORT IMLSolver : public SparseLinearSystemNM
{
private:
    /// Solver type.
    enum IMLSolverType { IML_ST_CG, IML_ST_GMRES };
    /// Preconditioner type.
    enum IMLPrecondType { IML_VoidPrec, IML_DiagPrec, IML_ILU_CompColPrec, IML_ILU_CompRowPrec, IML_ICPrec };

    /// Last mapped Lhs matrix
    SparseMtrx *lhs;
    /// Last mapped matrix version
    SparseMtrx :: SparseMtrxVersionType lhsVersion;
    /// Preconditioner.
    std::unique_ptr<Preconditioner> M;
    /// IML Solver type.
    IMLSolverType solverType;
    /// IML Preconditioner type.
    IMLPrecondType precondType;
    /// Precond. init flag.
    bool precondInit;
    // Preconditioner attribute string
    // InputRecord precondAttributes;

    /// Tolerance of residual.
    double tol;
    /// Max number of iterations.
    int maxite;

public:
    /// Constructor. Creates new instance of LDLTFactorization, with number i, belonging to domain d and Engngmodel m.
    IMLSolver(Domain * d, EngngModel * m);
    /// Destructor
    virtual ~IMLSolver() {}

    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "IMLSolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_IML; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return symmetric ? SMT_SymCompCol : SMT_CompCol; }
};
} // end namespace oofem
#endif // imlsolver_h
