// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "eigenvaluedynamic.h"
#include "solvers/timestep.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "export/exportmodulemanager.h"
#include "utility/verbose.h"
#include "engng/classfactory.h"
#include "export/datastream.h"
#include "solvers/geneigvalsolvertype.h"
#include "utility/contextioerr.h"
#include "engng/classfactory.h"
#include "dofman/dofmanager.h"
#include "dofman/dof.h"
#include "input/domain.h"
#include "input/unknownnumberingscheme.h"



namespace oofem {
REGISTER_EngngModel(EigenValueDynamic);


EigenValueDynamic :: EigenValueDynamic(int i, EngngModel *master) : EngngModel(i, master)
{
    numberOfSteps = 1;
    ndomains = 1;
}


NumericalMethod *EigenValueDynamic :: giveNumericalMethod(MetaStep *mStep)
{
    if ( !nMethod ) {
        nMethod = classFactory.createGeneralizedEigenValueSolver(solverType, this->giveDomain(1), this);
        if ( !nMethod ) {
            OOFEM_ERROR("solver creation failed");
        }
    }

    return nMethod.get();
}


void
EigenValueDynamic :: initializeFrom(InputRecord &ir)
{
    //EngngModel::instanciateFrom (ir);

    IR_GIVE_FIELD(ir, numberOfRequiredEigenValues, _IFT_EigenValueDynamic_nroot);
    this->field = std::make_unique<EigenVectorPrimaryField>(this, 1, FT_Displacements, numberOfRequiredEigenValues);

    // numberOfSteps set artificially to numberOfRequiredEigenValues
    // in order to allow
    // use restoreContext function for different eigenValues
    // numberOfSteps = numberOfRequiredEigenValues;
    numberOfSteps = 1;

    IR_GIVE_FIELD(ir, rtolv, _IFT_EigenValueDynamic_rtolv);
    if ( rtolv < 1.e-12 ) {
        rtolv =  1.e-12;
    } else if ( rtolv > 0.01 ) {
        rtolv =  0.01;
    }

    int val = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, val, _IFT_EigenValueDynamic_stype);
    solverType = ( GenEigvalSolverType ) val;

    val = 0; //Default Skyline
    IR_GIVE_OPTIONAL_FIELD(ir, val, _IFT_EngngModel_smtype);
    sparseMtrxType = ( SparseMtrxType ) val;

    suppressOutput = ir.hasField(_IFT_EngngModel_suppressOutput);

    if ( suppressOutput ) {
        printf("Suppressing output.\n");
    } else {
        if ( ( outputStream = fopen(this->dataOutputFileName.c_str(), "w") ) == NULL ) {
            OOFEM_ERROR("Can't open output file %s", this->dataOutputFileName.c_str());
        }

        fprintf(outputStream, "%s", PRG_HEADER);
        fprintf(outputStream, "\nStarting analysis on: %s\n", ctime(& this->startTime) );
        fprintf(outputStream, "%s\n", simulationDescription.c_str());
    }
}


int EigenValueDynamic :: giveUnknownDictHashIndx(ValueModeType mode, TimeStep *tStep)
{
    return tStep->giveNumber() % this->numberOfRequiredEigenValues; 
}


double EigenValueDynamic :: giveUnknownComponent(ValueModeType mode, TimeStep *tStep, Domain *d, Dof *dof)
{
    return field->giveUnknownValue(dof, mode, tStep);
}


TimeStep *EigenValueDynamic :: giveNextStep()
{
    int istep = giveNumberOfFirstStep();
    StateCounterType counter = 1;

    if ( currentStep ) {
        istep = currentStep->giveNumber() + 1;
        counter = currentStep->giveSolutionStateCounter() + 1;
    }

    previousStep = std :: move(currentStep);
    currentStep = std::make_unique<TimeStep>(istep, this, 1, ( double ) istep, 0., counter);

    return currentStep.get();
}


void EigenValueDynamic :: solveYourself()
{
    this->timer.startTimer(EngngModelTimer :: EMTT_AnalysisTimer);

    TimeStep *tStep = this->giveNextStep();
    this->updateAttributes( this->giveCurrentMetaStep() );

    OOFEM_LOG_INFO("Assembling stiffness and mass matrices\n");

    FloatMatrix eigVec;
    {
        std :: unique_ptr< SparseMtrx > stiffnessMatrix;
        std :: unique_ptr< SparseMtrx > massMatrix;

        stiffnessMatrix = classFactory.createSparseMtrx(sparseMtrxType);
        stiffnessMatrix->buildInternalStructure( this, 1, EModelDefaultEquationNumbering() );

        massMatrix = classFactory.createSparseMtrx(sparseMtrxType);
        massMatrix->buildInternalStructure( this, 1, EModelDefaultEquationNumbering() );

        this->assemble( *stiffnessMatrix, tStep, TangentAssembler(TangentStiffness), EModelDefaultEquationNumbering(), this->giveDomain(1) );
        this->assemble( *massMatrix, tStep, MassMatrixAssembler(), EModelDefaultEquationNumbering(), this->giveDomain(1) );

        this->giveNumericalMethod( this->giveMetaStep( tStep->giveMetaStepNumber() ) );
        OOFEM_LOG_INFO("Solving ...\n");
        nMethod->solve(*stiffnessMatrix, *massMatrix, eigVal, eigVec, rtolv, numberOfRequiredEigenValues);
    }
    this->field->updateAll(eigVec, EModelDefaultEquationNumbering());

    this->terminate( tStep );

    double steptime = this->giveSolutionStepTime();
    OOFEM_LOG_INFO("EngngModel info: user time consumed by solution: %.2fs\n", steptime);
}


void EigenValueDynamic :: updateYourself(TimeStep *tStep)
{ }


void EigenValueDynamic :: doStepOutput(TimeStep *tStep)
{
    if ( !suppressOutput ) {
        this->printOutputAt(this->giveOutputStream(), tStep);
        fflush( this->giveOutputStream() );
    }

    for ( int i = 1; i <= numberOfRequiredEigenValues; i++ ) {
        // export using export manager
        tStep->setTime( ( double ) i ); // we use time as intrinsic eigen value index
        tStep->setNumber(i);
        exportModuleManager.doOutput(tStep);
    }
}


void EigenValueDynamic :: printOutputAt(FILE *file, TimeStep *tStep)
{
    Domain *domain = this->giveDomain(1);

    // print loadcase header
    fprintf(file, "\nOutput for time %.3e \n\n", 1.0);
    // print eigen values on output
    fprintf(file, "\n\nEigen Values (Omega^2) are:\n-----------------\n");

    for ( int i = 1; i <= numberOfRequiredEigenValues; i++ ) {
        fprintf(file, "%15.8e ", eigVal.at(i) );
        if ( ( i % 5 ) == 0 ) {
            fprintf(file, "\n");
        }
    }

    fprintf(file, "\n\n");

    for ( int i = 1; i <=  numberOfRequiredEigenValues; i++ ) {
        fprintf(file, "\nOutput for eigen value no.  %.3e \n", ( double ) i);
        fprintf(file, "Printing eigen vector no. %d, corresponding eigen value is %15.8e\n\n", i, eigVal.at(i) );
        tStep->setTime( ( double ) i ); // we use time as intrinsic eigen value index
        tStep->setNumber(i);

        if ( this->requiresUnknownsDictionaryUpdate() ) {
            for ( auto &dman : domain->giveDofManagers() ) {
                this->updateDofUnknownsDictionary(dman.get(), tStep);
            }
        }


        for ( auto &dman : domain->giveDofManagers() ) {
            dman->updateYourself(tStep);
            dman->printOutputAt(file, tStep);
        }
    }

    double utsec = this->timer.getUtime(EngngModelTimer :: EMTT_AnalysisTimer);
    fprintf(file, "\nUser time consumed by solution step: %.3f [s]\n\n", utsec);
}


void EigenValueDynamic :: saveContext(DataStream &stream, ContextMode mode)
{
    EngngModel :: saveContext(stream, mode);

    contextIOResultType iores;
    if ( ( iores = eigVal.storeYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }

    this->field->saveContext(stream);
}


void EigenValueDynamic :: restoreContext(DataStream &stream, ContextMode mode)
{
    EngngModel :: restoreContext(stream, mode);

    contextIOResultType iores;
    if ( ( iores = eigVal.restoreYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }

    this->field->restoreContext(stream);
}


void EigenValueDynamic :: setActiveVector(int i)
{
    this->activeVector = i;
    if ( activeVector > numberOfRequiredEigenValues ) {
        activeVector = numberOfRequiredEigenValues;
    }

    this->giveCurrentStep()->setNumber( activeVector );
    this->giveCurrentStep()->setTime( ( double ) activeVector );
}

} // end namespace oofem
