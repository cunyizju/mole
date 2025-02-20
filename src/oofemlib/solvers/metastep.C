// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "solvers/metastep.h"

namespace oofem {
MetaStep :: MetaStep(int n, EngngModel *e) :
    eModel(e),
    numberOfSteps(0),
    number(n)
{}

MetaStep :: MetaStep(int n, EngngModel *e, int nsteps, InputRecord &attrib) :
    eModel(e),
    numberOfSteps(nsteps),
    attributes(attrib.clone()),
    number(n)
{}


void
MetaStep :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, numberOfSteps, _IFT_MetaStep_nsteps);

    this->attributes = ir.clone();
}

int
MetaStep :: setStepBounds(int startStepNumber)
{
    sindex = startStepNumber;

    return sindex + numberOfSteps;
}

void
MetaStep :: setNumberOfSteps(int newNumberOfSteps)
{
    this->numberOfSteps = newNumberOfSteps;
}

int
MetaStep :: isStepValid(int solStepNumber)
{
    return solStepNumber >= sindex && solStepNumber < ( sindex + numberOfSteps );
}
} // end namespace oofem
