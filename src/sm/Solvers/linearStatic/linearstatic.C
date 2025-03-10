// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "linearstatic.h"
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
REGISTER_EngngModel(LinearStatic);

LinearStatic :: LinearStatic(int i, EngngModel *_master) : StructuralEngngModel(i, _master), loadVector(), displacementVector()
{
    ndomains = 1;
    initFlag = 1;
    solverType = ST_Direct;
}


LinearStatic :: ~LinearStatic() {}


NumericalMethod *LinearStatic :: giveNumericalMethod(MetaStep *mStep)
{
    if ( !nMethod ) {
        if ( isParallel() ) {
            if ( ( solverType == ST_Petsc ) || ( solverType == ST_Feti ) ) {
                nMethod = classFactory.createSparseLinSolver(solverType, this->giveDomain(1), this);
            }
        } else {
            nMethod = classFactory.createSparseLinSolver(solverType, this->giveDomain(1), this);
        }
        if ( !nMethod ) {
            OOFEM_ERROR("linear solver creation failed for lstype %d", solverType);
        }
    }


    return nMethod.get();
}

void
LinearStatic :: initializeFrom(InputRecord &ir)
{
    StructuralEngngModel :: initializeFrom(ir);

    int val = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, val, _IFT_EngngModel_lstype);
    solverType = ( LinSystSolverType ) val;

    val = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, val, _IFT_EngngModel_smtype);
    sparseMtrxType = ( SparseMtrxType ) val;

#ifdef __PARALLEL_MODE
    if ( isParallel() ) {
        commBuff = new CommunicatorBuff( this->giveNumberOfProcesses() );
        communicator = new NodeCommunicator(this, commBuff, this->giveRank(),
                                            this->giveNumberOfProcesses());
    }

#endif
}


double LinearStatic :: giveUnknownComponent(ValueModeType mode, TimeStep *tStep, Domain *d, Dof *dof)
// returns unknown quantity like displacement, velocity of equation eq
// This function translates this request to numerical method language
{
    int eq = dof->__giveEquationNumber();
#ifdef DEBUG
    if ( eq == 0 ) {
        OOFEM_ERROR("invalid equation number");
    }
#endif

    if ( tStep != this->giveCurrentStep() ) {
        OOFEM_ERROR("unknown time step encountered");
        return 0.;
    }

    switch ( mode ) {
    case VM_Total:
    case VM_Incremental:
        if ( displacementVector.isNotEmpty() ) {
            return displacementVector.at(eq);
        } else {
            return 0.;
        }

    default:
        OOFEM_ERROR("Unknown is of undefined type for this problem");
    }

    return 0.;
}


TimeStep *LinearStatic :: giveNextStep()
{
    if ( !currentStep ) {
        // first step -> generate initial step
        //currentStep = std::make_unique<TimeStep>(*giveSolutionStepWhenIcApply());
        currentStep = std::make_unique<TimeStep>(giveNumberOfTimeStepWhenIcApply(), this, 1, 0., 1., 0);
    }
    previousStep = std :: move(currentStep);
    currentStep = std::make_unique<TimeStep>(*previousStep, 1.);

    return currentStep.get();
}


void LinearStatic :: solveYourself()
{
    if ( this->isParallel() ) {
#ifdef __VERBOSE_PARALLEL
        // force equation numbering before setting up comm maps
        int neq = this->giveNumberOfDomainEquations(1, this->giveEquationNumbering());
        OOFEM_LOG_INFO("[process rank %d] neq is %d\n", this->giveRank(), neq);
#endif

        this->initializeCommMaps();
    }

    StructuralEngngModel :: solveYourself();
}



void LinearStatic :: solveYourselfAt(TimeStep *tStep)
{
    //
    // creates system of governing eq's and solves them at given time step
    //
    // first assemble problem at current time step

    if ( initFlag ) {
#ifdef VERBOSE
        OOFEM_LOG_DEBUG("Assembling stiffness matrix\n");
#endif

        //
        // first step  assemble stiffness Matrix
        //
        stiffnessMatrix = classFactory.createSparseMtrx(sparseMtrxType);
        if ( !stiffnessMatrix ) {
            OOFEM_ERROR("sparse matrix creation failed");
        }

        stiffnessMatrix->buildInternalStructure( this, 1, this->giveEquationNumbering() );

        this->assemble( *stiffnessMatrix, tStep, TangentAssembler(TangentStiffness),
                       this->giveEquationNumbering(), this->giveDomain(1) );

        initFlag = 0;
    }

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
    OOFEM_LOG_INFO("\n\nSolving ...\n\n");
#endif
    ConvergedReason s = nMethod->solve(*stiffnessMatrix, loadVector, displacementVector);
    if ( s != CR_CONVERGED ) {
        OOFEM_ERROR("No success in solving system.");
    }
    tStep->numberOfIterations = 1;
    tStep->convergedReason = s;
    tStep->incrementStateCounter();            // update solution state counter
}


void LinearStatic :: saveContext(DataStream &stream, ContextMode mode)
{
    StructuralEngngModel :: saveContext(stream, mode);

    contextIOResultType iores;
    if ( ( iores = displacementVector.storeYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }
}


void LinearStatic :: restoreContext(DataStream &stream, ContextMode mode)
{
    StructuralEngngModel :: restoreContext(stream, mode);

    contextIOResultType iores;
    if ( ( iores = displacementVector.restoreYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }
}


void
LinearStatic :: updateDomainLinks()
{
    EngngModel :: updateDomainLinks();
    this->giveNumericalMethod( this->giveCurrentMetaStep() )->setDomain( this->giveDomain(1) );
}


int
LinearStatic :: estimateMaxPackSize(IntArray &commMap, DataStream &buff, int packUnpackType)
{
    int count = 0, pcount = 0;
    Domain *domain = this->giveDomain(1);

    if ( packUnpackType == 0 ) { ///@todo Fix this old ProblemCommMode__NODE_CUT value
        for ( int map: commMap ) {
            DofManager *dman = domain->giveDofManager( map );
            for ( Dof *dof: *dman ) {
                if ( dof->isPrimaryDof() && ( dof->__giveEquationNumber() ) ) {
                    count++;
                } else {
                    pcount++;
                }
            }
        }

        // --------------------------------------------------------------------------------
        // only pcount is relevant here, since only prescribed components are exchanged !!!!
        // --------------------------------------------------------------------------------

        return buff.givePackSizeOfDouble(1) * pcount;
    } else if ( packUnpackType == 1 ) {
        for ( int map: commMap ) {
            count += domain->giveElement( map )->estimatePackSize(buff);
        }

        return count;
    }

    return 0;
}

} // end namespace oofem
