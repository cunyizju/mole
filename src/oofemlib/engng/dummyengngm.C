// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

// Milan ?????????????????
//#include "gpinitmodule.h"
// Milan ?????????????????

#include "engng//dummyengngm.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"


namespace oofem {
REGISTER_EngngModel(DummyEngngModel);

DummyEngngModel :: DummyEngngModel(int i, EngngModel *_master) : EngngModel (i, _master)
{
    ndomains = 1;
}

void
DummyEngngModel :: initializeFrom(InputRecord &ir)
{
    this->numberOfSteps = 1;
    this->nMetaSteps   = 0;
    this->suppressOutput = true;
    
}

TimeStep *DummyEngngModel :: giveNextStep()
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

void DummyEngngModel :: solveYourselfAt(TimeStep *tStep)
{

}
} // end namespace oofem
