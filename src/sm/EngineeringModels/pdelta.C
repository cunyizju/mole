// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/EngineeringModels/pdelta.h"
#include "sm/Elements/structuralelement.h"
#include "sm/Elements/structuralelementevaluator.h"
#include "input/nummet.h"
#include "solvers/timestep.h"
#include "input/element.h"
#include "dofman/dof.h"
#include "math/sparsemtrx.h"
#include "utility/verbose.h"
#include "engng/classfactory.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "engng/classfactory.h"
#include "input/unknownnumberingscheme.h"

#ifdef __PARALLEL_MODE
 #include "parallel/problemcomm.h"
 #include "parallel/communicator.h"
#endif

#include <typeinfo>

namespace oofem {
REGISTER_EngngModel(Pdelta);

Pdelta :: Pdelta(int i, EngngModel *_master) : LinearStatic(i, _master)
{
    ndomains = 1;
    initFlag = 1;
    solverType = ST_Direct;
    lumpedInitialStressMatrix = true;
}

void
Pdelta :: initializeFrom(InputRecord &ir)
{
    LinearStatic :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, rtolv, _IFT_Pdelta_rtolv);
    this->maxiter = 50;
    IR_GIVE_OPTIONAL_FIELD(ir, maxiter, _IFT_Pdelta_maxiter);
    IR_GIVE_OPTIONAL_FIELD(ir, lumpedInitialStressMatrix, _IFT_Pdelta_lumpedInitialStressMatrix);
}

void Pdelta :: solveYourselfAt(TimeStep *tStep)
{
 
    FloatArray rhs, previousDisplacementVector;
    //
    // first step  assemble stiffness Matrix
    //
    stiffnessMatrix = classFactory.createSparseMtrx(sparseMtrxType);
    if ( !stiffnessMatrix ) {
        OOFEM_ERROR("sparse matrix creation failed");
    }
    initialStressMatrix = classFactory.createSparseMtrx(sparseMtrxType);
    if ( !initialStressMatrix ) {
        OOFEM_ERROR("sparse matrix creation failed");
    }

    stiffnessMatrix->buildInternalStructure( this, 1, this->giveEquationNumbering() );
    initialStressMatrix->buildInternalStructure( this, 1, this->giveEquationNumbering() );

#ifdef VERBOSE
        OOFEM_LOG_DEBUG("Assembling stiffness matrix\n");
#endif

    this->assemble( *stiffnessMatrix, tStep, TangentAssembler(TangentStiffness),
                    this->giveEquationNumbering(), this->giveDomain(1) );

#ifdef VERBOSE
    OOFEM_LOG_DEBUG("Assembling load\n");
#endif

    //
    // allocate space for displacementVector
    //
    displacementVector.resize( this->giveNumberOfDomainEquations( 1, this->giveEquationNumbering() ) ); // km?? replace EModelDefaultEquationNumbering() with this->giveEquationNumbering(). Use pointer?
    displacementVector.zero();

    //
    // assembling the load vector
    //
    loadVector.resize( this->giveNumberOfDomainEquations( 1, this->giveEquationNumbering() ) );
    loadVector.zero();
    this->assembleVector( loadVector, tStep, ExternalForceAssembler(), VM_Total,
                         this->giveEquationNumbering(), this->giveDomain(1) );

    //
    // internal forces (from Dirichlet b.c's, or thermal expansion, etc.)
    //
    FloatArray internalForces( this->giveNumberOfDomainEquations( 1, this->giveEquationNumbering() ) );
    internalForces.zero();
    this->assembleVector( internalForces, tStep, InternalForceAssembler(), VM_Total,
                         this->giveEquationNumbering(), this->giveDomain(1) );

    loadVector.subtract(internalForces);

    this->updateSharedDofManagers(loadVector, this->giveEquationNumbering(), ReactionExchangeTag);

    //
    // set-up numerical model
    //
    this->giveNumericalMethod( this->giveMetaStep( tStep->giveMetaStepNumber() ) );

    //
    // call numerical model to solve arose problem
    //
#ifdef VERBOSE
    OOFEM_LOG_INFO("\n\nSolving ...\n");
#endif
    int iter = 0;
    double error;
    FloatArray feq(displacementVector.giveSize());

    do {
        previousDisplacementVector = displacementVector;
        rhs = loadVector;
        if (iter) {
            feq.zero();
            if (this->lumpedInitialStressMatrix) {
                this->assembleVector(feq, tStep, MatrixProductAssembler(LumpedInitialStressMatrixAssembler()),
                                     VM_Total, EModelDefaultEquationNumbering(), this->giveDomain(1) );
            } else {
                this->assembleVector(feq, tStep, MatrixProductAssembler(InitialStressMatrixAssembler()),
                                     VM_Total, EModelDefaultEquationNumbering(), this->giveDomain(1) );

            }
            rhs.subtract(feq);
            //this->assembleVector(rhs, tStep, EquivalentLateralLoadAssembler(), VM_Total,
            //                     this->giveEquationNumbering(), this->giveDomain(1) );
        }
        ConvergedReason s = nMethod->solve(*stiffnessMatrix, rhs, displacementVector);
    
        if ( s != CR_CONVERGED ) {
            OOFEM_ERROR("No success in solving system.");
        }
        tStep->convergedReason = s;
        tStep->incrementStateCounter();            // update solution state counter
        previousDisplacementVector.subtract(displacementVector);
        error = previousDisplacementVector.computeNorm()/displacementVector.computeNorm();
        iter++;
    } while ((error > rtolv) && (iter <= this->maxiter));
    OOFEM_LOG_INFO("Done in %d iteration(s) (err = %le)\n", iter-1, error);
}




} // end namespace oofem
