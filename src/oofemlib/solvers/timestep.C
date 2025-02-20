// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/*
 * The original idea for this class comes from
 * Dubois-Pelerin, Y.: "Object-Oriented  Finite Elements: Programming concepts and Implementation",
 * PhD Thesis, EPFL, Lausanne, 1992.
 */

#include "solvers/timestep.h"
#include "engng/engngm.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "error/error.h"
#include "solvers/convergedreason.h"

namespace oofem {
TimeStep :: TimeStep(int n, EngngModel *e, int mn, double tt, double dt, StateCounterType counter, TimeDiscretizationType td) :
    eModel(e), targetTime(tt), intrinsicTime(tt), deltaT(dt), solutionStateCounter(counter),
    number(n), version(0), subStepNumber(0), mStepNumber(mn), timeDiscretization(td), numberOfIterations(0), numberOfAttempts(0), convergedReason(ConvergedReason::CR_UNKNOWN)
{
    // Target time and intrinsic time is the same in the constructor.
}

TimeStep :: TimeStep(EngngModel *e)
{
    eModel = e;
    deltaT = 0.0;
    targetTime = 0.0;
    intrinsicTime = 0.0;
    solutionStateCounter = 0;
    number = -1;
    version = 0;
    mStepNumber = 0;
    subStepNumber = 0;
    numberOfIterations = 0;
    numberOfAttempts = 0;
    convergedReason = ConvergedReason::CR_UNKNOWN;
}

TimeStep :: TimeStep(const TimeStep &src)
{
    eModel = src.eModel;
    targetTime = src.targetTime;
    intrinsicTime = src.intrinsicTime;
    deltaT = src.deltaT;
    solutionStateCounter = src.solutionStateCounter;
    number = src.number;
    version = src.version;
    mStepNumber = src.mStepNumber;
    subStepNumber = src.subStepNumber;
    numberOfIterations = src.numberOfIterations;
    numberOfAttempts = src.numberOfAttempts;
    convergedReason = src.convergedReason;
}

TimeStep :: TimeStep(const TimeStep &previous, double dt)
{
    eModel = previous.eModel;
    targetTime = previous.targetTime + dt;
    intrinsicTime = previous.intrinsicTime + dt;
    deltaT = dt;
    solutionStateCounter = previous.solutionStateCounter + 1;
    number = previous.number + 1;
    version = 0;
    mStepNumber = previous.mStepNumber ? previous.mStepNumber : 1;
    subStepNumber = 0;
    numberOfIterations = 0;
    numberOfAttempts = 0;
    convergedReason = ConvergedReason::CR_UNKNOWN;
}


TimeStep &
TimeStep :: operator = ( const TimeStep & src )
{
    eModel = src.eModel;
    targetTime = src.targetTime;
    intrinsicTime = src.intrinsicTime;
    deltaT = src.deltaT;
    solutionStateCounter = src.solutionStateCounter;
    number = src.number;
    version = src.version;
    mStepNumber = src.mStepNumber;
    subStepNumber = src.subStepNumber;
    numberOfIterations = src.numberOfIterations;
    numberOfAttempts = src.numberOfAttempts;
    convergedReason = src.convergedReason;

    return * this;
}


TimeStep *TimeStep :: givePreviousStep()
{
    if ( isTheCurrentTimeStep() ) {
        return eModel->givePreviousStep();
    } else {
        OOFEM_ERROR("Could not return previous step of noncurrent step");
    }

    return NULL; // to make compiler happy
}


bool TimeStep :: isNotTheLastStep()
{
    return  ( number != eModel->giveNumberOfSteps() );
}


bool TimeStep :: isTheFirstStep()
{
    return  ( number == eModel->giveNumberOfFirstStep() );
}


bool TimeStep :: isIcApply()
{
    // Returns True if the receiver is the  time step,
    // when Initial conditions apply
    // else returns False.

    return  ( number == eModel->giveNumberOfTimeStepWhenIcApply() );
}


bool TimeStep :: isTheCurrentTimeStep()
{
    return this == eModel->giveCurrentStep();
}


void
TimeStep :: saveContext(DataStream &stream)
{
    if ( !stream.write(number) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.write(mStepNumber) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.write(this->targetTime) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.write(this->intrinsicTime) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.write(this->deltaT) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.write(this->solutionStateCounter) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    int tDiscretization = ( int ) timeDiscretization;
    if ( !stream.write(tDiscretization) ) {
        THROW_CIOERR(CIO_IOERR);
    }
}


void
TimeStep :: restoreContext(DataStream &stream)
{
    if ( !stream.read(number) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.read(mStepNumber) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.read(this->targetTime) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.read(this->intrinsicTime) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.read(this->deltaT) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    if ( !stream.read(this->solutionStateCounter) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    int tDiscretization = 0;
    if ( !stream.read(tDiscretization) ) {
        THROW_CIOERR(CIO_IOERR);
    }
    timeDiscretization = ( TimeDiscretizationType ) tDiscretization;
}
} // end namespace oofem
