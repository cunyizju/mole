// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dsssolver_h
#define dsssolver_h

#include "oofemcfg.h"
#include "solvers/sparselinsystemnm.h"
#include "solvers/convergedreason.h"
#include "math/sparsemtrx.h"

#define _IFT_DSSSolver_Name "dss"

namespace oofem {
class Domain;
class EngngModel;
class FloatMatrix;
class FloatArray;

/**
 * Implements the solution of linear system of equation in the form Ax=b using direct factorization method.
 * Can work with any sparse matrix implementation. However, the sparse matrix implementation have to support
 * its factorization (canBeFactorized method).
 */
class OOFEM_EXPORT DSSSolver : public SparseLinearSystemNM
{
public:
    /**
     * Constructor.
     * Creates new instance of DSS solver belonging to domain d and Engngmodel m.
     * @param d Domain which solver belongs to.
     * @param m Engineering model which solver belongs to.
     */
    DSSSolver(Domain *d, EngngModel *m);
    /// Destructor.
    virtual ~DSSSolver();

    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    const char *giveClassName() const override { return "DSSSolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_DSS; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return symmetric ? SMT_DSS_sym_LDL : SMT_DSS_unsym_LU; } ///@todo Check
};
} // end namespace oofem

#endif // dsssolver_h

