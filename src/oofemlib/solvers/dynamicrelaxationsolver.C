// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "solvers/dynamicrelaxationsolver.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"
#include "export/exportmodulemanager.h"
#include "engng/engngm.h"
#include "input/domain.h"
#include "dofman/dofmanager.h"
#include "input/element.h"
#include "input/unknownnumberingscheme.h"
#include "math/mathfem.h"
#include "input/assemblercallback.h"

namespace oofem {

#define ERROR_NORM_SMALL_NUM 1.e-6
#define MAX_REL_ERROR_BOUND 1.e20

REGISTER_SparseNonLinearSystemNM(DynamicRelaxationSolver)

DynamicRelaxationSolver :: DynamicRelaxationSolver(Domain *d, EngngModel *m) : NRSolver(d, m)
{
}


void
DynamicRelaxationSolver :: initializeFrom(InputRecord &ir)
{
    NRSolver :: initializeFrom(ir);
}


ConvergedReason
DynamicRelaxationSolver :: solve(SparseMtrx &k, FloatArray &R, FloatArray *R0,
                  FloatArray &X, FloatArray &dX, FloatArray &F,
                  const FloatArray &internalForcesEBENorm, double &l, referenceLoadInputModeType rlm,
                  int &nite, TimeStep *tStep)

{
    // residual, iteration increment of solution, total external force
    FloatArray rhs, ddX, RT, X_0, X_n, X_n1, M;

    double RRT;
    int neq = X.giveSize();
    ConvergedReason status = CR_UNKNOWN;
    bool converged, errorOutOfRangeFlag;
    ParallelContext *parallel_context = engngModel->giveParallelContext( this->domain->giveNumber() );
    if ( engngModel->giveProblemScale() == macroScale ) {
        OOFEM_LOG_INFO("DRSolver: Iteration");
        if ( rtolf.at(1) > 0.0 ) {
            OOFEM_LOG_INFO(" ForceError");
        }
        if ( rtold.at(1) > 0.0 ) {
            OOFEM_LOG_INFO(" DisplError");
        }
        OOFEM_LOG_INFO("\n----------------------------------------------------------------------------\n");
    }

    // compute total load R = R+R0
    l = 1.0;
    RT = R;
    if ( R0 ) {
        RT.add(* R0);
    }

    RRT = parallel_context->localNorm(RT);
    RRT *= RRT;

    ddX.resize(neq);
    ddX.zero();

    X_0 = X;
    X_n = X_0;
    X_n1 = X_0;

    // Compute the mass "matrix" (lumped, only storing the diagonal)
    M.resize(neq);
    M.zero();
    engngModel->assembleVector(M, tStep, LumpedMassVectorAssembler(), VM_Total, EModelDefaultEquationNumbering(), domain);

    double Le = -1.0;
    for ( auto &elem : domain->giveElements() ) {
        double size = elem->computeMeanSize();
        if ( Le < 0 || Le >= size ) {
            Le = size;
        }
    }

    for ( nite = 0; ; ++nite ) {
        // Compute the residual
        engngModel->updateComponent(tStep, InternalRhs, domain);
        rhs.beDifferenceOf(RT, F);

        converged = this->checkConvergence(RT, F, rhs, ddX, X, RRT, internalForcesEBENorm, nite, errorOutOfRangeFlag);
        if ( errorOutOfRangeFlag ) {
            status = CR_DIVERGED_TOL;
            dX.zero();
            X.zero();
            OOFEM_WARNING("Divergence reached after %d iterations", nite);
            break;
        } else if ( converged && ( nite >= minIterations ) ) {
            status = CR_CONVERGED;
            break;
        } else if ( nite >= nsmax ) {
            OOFEM_LOG_DEBUG("Maximum number of iterations reached\n");
            status = CR_DIVERGED_ITS;
            break;
        }

        double density = 1.;
        double lambda = 210e9;
        double mu = 210e9;
        double c = sqrt((lambda + 2*mu) / density);
        double dt = 0.25 * Le / c;
        double alpha = 0.1 / dt;
        printf("dt = %e\n", dt);
        for ( int j = 0; j < neq; ++j ) {
            //M * x'' + C*x' * dt = rhs * dt*dt
            X[j] = rhs[j] * dt * dt / M[j] - ( -2*X_n1[j] + X_n[j] ) - alpha * (X_n1[j] - X_n[j]) * dt;
        }
        X_n = X_n1;
        X_n1 = X;

        dX.beDifferenceOf(X, X_0);
        tStep->incrementStateCounter(); // update solution state counter
        tStep->incrementSubStepNumber();
    }

    return status;
}

} // end namespace oofem

