// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fetisolver_h
#define fetisolver_h

#include "sm/FETISolver/feticommunicator.h"
#include "solvers/sparselinsystemnm.h"
#include "solvers/convergedreason.h"
#include "math/sparsemtrx.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "parallel/processcomm.h"

///@name Input fields for FETISolver
//@{
#define _IFT_FETISolver_Name "feti"
#define _IFT_FETISolver_maxiter "maxiter"
#define _IFT_FETISolver_maxerr "maxerr"
#define _IFT_FETISolver_limit "limit"
#define _IFT_FETISolver_energynormflag "energynormflag"
//@}

namespace oofem {
class Domain;
class EngngModel;


 #define FETISOLVER_MAX_RBM 6
/// computer zero
 #define FETISOLVER_ZERONUM 1.e-40

/**
 * This class implements the class NumericalMethod instance FETI
 * linear algebraic equation parallel solver.
 */
class FETISolver : public SparseLinearSystemNM
{
private:
    int nse;
    /// Max number of iterations.
    int ni;
    /// Max allowed error.
    double err;
    /// Linear dep./indep. trigger.
    double limit;
    /// Rigid body motions.
    FloatMatrix rbm;
    /// Rigid body motions of all partitions. On master only.
    FloatMatrix l;
    /// Indices of singular equations.
    IntArray se;
    /// Addresses of initial partition contribution to rbm matrix.
    IntArray rbmAddr;
    /// Primary unknowns.
    FloatArray w;
    //
    FloatArray qq, q, dd, g, d, p, pp, gamma, localGammas;
    //
    IntArray nsem;
    ///
    ProcessCommunicatorBuff pcbuff;
    ProcessCommunicator processCommunicator;
    /// Common Communicator buffer.
    CommunicatorBuff *commBuff;
    FETICommunicator *masterCommunicator;
    /// List of local nodes (at master) participating in communication (list of boundary dof managers).
    IntArray masterCommMap;
    /// Flag indicating computation of energy norm.
    int energyNorm_comput_flag;
public:
    FETISolver(Domain * d, EngngModel * m);
    virtual ~FETISolver();

    /**
     * Solves the given linear system by LDL^T factorization.
     * Implementation rely on factorization support provided by mapped sparse matrix.
     * It calls Lhs->factorized()->backSubstitutionWith(*solutionArray). Sets solved flag to 1 if o.k.
     * @param A Coefficient matrix
     * @param b Right hand side
     * @param x Solution array
     * @return ConvergedReason value
     */
    ConvergedReason solve(SparseMtrx &A, FloatArray &b, FloatArray &x) override;

    int estimateMaxPackSize(IntArray &, DataStream &, int &);
    /// Sets up the communication maps
    void setUpCommunicationMaps();

    void initializeFrom(InputRecord &ir) override;

    // identification
    const char *giveClassName() const override { return "FETISolver"; }
    LinSystSolverType giveLinSystSolverType() const override { return ST_Feti; }

    void projection(FloatArray &v, FloatMatrix &l, FloatMatrix &l1);

    int packRBM(ProcessCommunicator &processComm);
    int masterUnpackRBM(ProcessCommunicator &processComm);
    int packQQProducts(ProcessCommunicator &processComm);
    int masterUnpackQQProduct(ProcessCommunicator &processComm);
    int packSolution(ProcessCommunicator &processComm);
    int unpackSolution(ProcessCommunicator &processComm);
    int packResiduals(ProcessCommunicator &processComm);
    int unpackResiduals(ProcessCommunicator &processComm);
    int packDirectionVector(ProcessCommunicator &processComm);
    int unpackDirectionVector(ProcessCommunicator &processComm);
    int packPPVector(ProcessCommunicator &processComm);
    int unpackPPVector(ProcessCommunicator &processComm);
    int packGammas(ProcessCommunicator &processComm);
    int unpackGammas(ProcessCommunicator &processComm);

    int masterMapRBM();
    int masterMapQQProduct();
    int masterMapSolution();
    int masterMapResiduals();
    int masterMapDirectionVector();
    int masterMapPPVector();
    int masterMapGammas();

    enum { FETISolverZeroTag, NumberOfRBMMsg, RBMMessage, QQMessage, SolutionMessage, ResidualMessage, DirectionVectorMessage, PPVectorMessage, GammasMessage, FETISolverIterationContinue, FETISolverIterationBreak };
    virtual SparseMtrxType giveRecommendedMatrix(bool symmetric) const { return symmetric ? SMT_Skyline : SMT_SkylineU; }
};
} // end namespace oofem

#endif // fetisolver_h
