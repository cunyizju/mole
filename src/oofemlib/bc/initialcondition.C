// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/initialcondition.h"
#include "input/inputrecord.h"
#include "input/cltypes.h"

namespace oofem {
double InitialCondition :: give(ValueModeType type)
// Returns the prescribed value of the kinematic unknown 'u'.
{
    char u;
    u =  cltypesGiveUnknownTypeModeKey(type);
    if ( this->hasConditionOn(u) ) {
        return initialValueDictionary.at(u);
    } else {
        return 0.;
    }
}


int InitialCondition :: hasConditionOn(int u)
// Returns True if the receiver submits the unknown 'u' to an initial
// condition, else returns False.
{
    return  ( initialValueDictionary.includes(u) );
}


int InitialCondition :: hasConditionOn(ValueModeType type)
// Returns True if the receiver submits the unknown 'u' to an initial
// condition, else returns False.
{
    char u = cltypesGiveUnknownTypeModeKey(type);
    return  ( initialValueDictionary.includes(u) );
}

void InitialCondition :: printYourself()
// Prints the receiver on screen.
{
    printf("Initial condition %d\ninitial values :\n", number);
    initialValueDictionary.printYourself();
}


void
InitialCondition :: initializeFrom(InputRecord &ir)
// Sets up the dictionary where the receiver stores the conditions it
// imposes.
{
    IR_GIVE_FIELD(ir, initialValueDictionary, _IFT_InitialCondition_conditions);

    int val = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, val, _IFT_InitialCondition_valType);
    valType = ( bcValType ) val;

    ///@todo Make these both not optional (and remove the old approach). Not done right now because it breaks backwards compatibility with input files.
    this->set = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, this->set, _IFT_InitialCondition_set);
    this->dofIDs.clear();
    IR_GIVE_OPTIONAL_FIELD(ir, this->dofIDs, _IFT_InitialCondition_dofs);
}


void
InitialCondition :: scale(ValueModeType type, double s)
{
    if ( this->hasConditionOn(type) ) {
        initialValueDictionary.at(type) *= s;
    }
}
} // end namespace oofem
