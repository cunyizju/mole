// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ldltfact_h
#define ldltfact_h

#include "solvers/sparselinsystemnm.h"
#include "solvers/convergedreason.h"
#include "math/sparsemtrx.h"
#include "math/floatarray.h"

#define _IFT_LDLTFactorization_Name "direct"

namespace oofem {
class Domain;
class EngngModel;
class FloatMatrix;

/**
 * Implements the solution of linear system of equation in the form Ax=b using direct factorization method.
 * Can work with any sparse matrix implementation. However, the sparse matrix implementation have to support
 * its factorization (canBeFactorized method).
 */
class OOFEM_EXPORT LDLTFactorization : public SparseLinearSystemNM
{
public:
    /// Constructor - creates new instance of LDLTFactorization, with number i, belonging to domain d and Engngmodel m.
    LDLTFactorization(Domain * d, EngngModel * m);
    /// Destructor
    virtual ~LDLTFactorization();

    /**
     * Solves the given linear system by LDL^T factorization.
     * Implementation rely on factorization support provided by mapped sparse matrix.
     * It calls Lhs->factorized()->backSubstitutionWith(*solutionArray). Sets solved flag to 1 if o.k.
     * @param A coefficient matrix
     * @param b right hand side
     * @param x solution array
     * @return NM_Status value
     */
    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    const char *giveClassName() const override { return "LDLTFactorization"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_Direct; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return symmetric ? SMT_Skyline : SMT_SkylineU; }
};
} // end namespace oofem
#endif // ldltfact_h
