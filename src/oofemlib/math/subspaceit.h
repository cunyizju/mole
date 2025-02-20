// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef subspaceit_h
#define subspaceit_h

#include "math/sparsegeneigenvalsystemnm.h"
#include "solvers/convergedreason.h"
#include "math/sparsemtrx.h"
#include "math/floatarray.h"

#define _IFT_SubspaceIteration_Name "subspaceit"

namespace oofem {
class Domain;
class EngngModel;

/**
 * This class implements the class NumericalMethod instance Subspace Iteration Eigen Value Problem Solver
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
 *      - - - INPUT  DATA - - -
 *
 *      A(NWK)  -  STIFFNESS MATRIX
 *      B(NWM)  -  MASS MARTRIX
 *      NN  -      SIZE OF PROBLEM
 *      NNM  -  NN+1
 *      NROOT  -  REQUIRED NUMBER OF
 *      RTOL  -  KRITERIUM KONVERGENCE VLASTNICH CISEL
 *      NC  -  POCET VEKTORU SIMULTANNI ITERACE, DOPORUCUJE SE VOLIT
 *      NC = MIN (2*NROOT , NROOT+8 )
 *      NITEM  -  MAXIMALNI POCET ITERACI (OBYC. 16)
 *
 *      - - - PRACOVNI POLE - - -
 *
 *      TT(NN),W(NN),D(NC),RTOLV(NC),BUP(NC),BLO(NC),BUPC(NC)
 *      AR(NC,NC)  -  PRACOVNI MATICE - PROJEKCE MATICE  A
 *      BR(NC,NC)  -  PROJEKCE MATICE  B
 *
 *      - - - VYSTUPNI DATA - - -
 *
 *      EIGV(NROOT)  -  VLASTNI CISLA
 *      R(NN,NROOT)  -  VLASTNI VEKTORY
 */
class OOFEM_EXPORT SubspaceIteration : public SparseGeneralEigenValueSystemNM
{
private:
    /// Max number of iterations
    int nitem;

public:
    SubspaceIteration(Domain * d, EngngModel * m);
    virtual ~SubspaceIteration() {}

    ConvergedReason solve(SparseMtrx &A, SparseMtrx &B, FloatArray &x, FloatMatrix &v, double rtol, int nroot) override;
    const char *giveClassName() const override { return "SubspaceIterationSolver"; }
};
} // end namespace oofem
#endif // subspaceit_h
