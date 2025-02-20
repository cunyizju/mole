// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef spoolessolver_h
#define spoolessolver_h

#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrx.h"
#include "solvers/convergedreason.h"
#include "math/floatarray.h"
extern "C" {
#include <spooles/misc.h>
#include <spooles/FrontMtx.h>
#include <spooles/SymbFac.h>
};


///@name Input fields for SpoolesSolver
//@{
#define _IFT_SpoolesSolver_Name "spooles"
#define _IFT_SpoolesSolver_msglvl "msglvl"
#define _IFT_SpoolesSolver_msgfile "msgfile"
//@}

namespace oofem {
class Domain;
class EngngModel;
class FloatMatrix;

/**
 * Implements the solution of linear system of equation in the form @f$ A\cdot x = b @f$ using solvers
 * from SPOOLES library. Can work with only SPOOLES sparse matrix implementation.
 */
class OOFEM_EXPORT SpoolesSolver : public SparseLinearSystemNM
{
private:
    /// Last mapped LHS matrix
    SparseMtrx *Lhs;
    /// Last mapped matrix version
    SparseMtrx :: SparseMtrxVersionType lhsVersion;
    int msglvl;
    FILE *msgFile;
    int msgFileCloseFlag;

    FrontMtx *frontmtx;
    IV *oldToNewIV, *newToOldIV;
    ETree *frontETree;
    IVL *adjIVL, *symbfacIVL;
    SubMtxManager *mtxmanager;
    Graph *graph;

public:
    /**
     * Constructor.
     * @param d Domain which solver belongs to.
     * @param m Engineering model which solver belongs to.
     */
    SpoolesSolver(Domain * d, EngngModel * m);

    ///Destructor
    virtual ~SpoolesSolver();

    /**
     * Solves the given linear system by LDL^T factorization.
     */
    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    /// Initializes receiver from given record.
    void initializeFrom(InputRecord &ir) override;

    // identification
    const char *giveClassName() const override { return "SpoolesSolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_Spooles; }
    SparseMtrxType giveRecommendedMatrix(bool symmetric) const override { return SMT_SpoolesMtrx; }
};
} // end namespace oofem
#endif // spoolessolver_h
