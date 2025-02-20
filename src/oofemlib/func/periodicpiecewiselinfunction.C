// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "func/periodicpiecewiselinfunction.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"
#include "input/domain.h"

namespace oofem {
REGISTER_Function(PeriodicPiecewiseLinFunction);

double PeriodicPiecewiseLinFunction :: evaluateAtTime(double time)
// Returns the value of the receiver at time 'time'. 'time' should be
// one of the dates of the receiver (currently there is no interpola-
// tion between two points).
{
    double add, last;

    if ( !this->dates.giveSize() ) {
        OOFEM_ERROR("Undefined dates and values!");
    }

    if ( addTF && !domain->giveFunction(addTF) ) {
        OOFEM_ERROR("Undefined time function to add!");
    }

    if ( addTF ) {
        add = domain->giveFunction(addTF)->evaluateAtTime(time);
    } else {
        add = 0.;
    }

    // periodicity
    last = dates.at( this->dates.giveSize() ); // time of last date
    if ( ( period >= 0.0 ) && ( time > last ) ) {
        double d = ( time - last ) / period; // periods after last
        time = last + ( d - floor(d) - 1. ) * period;
    }

    return add + PiecewiseLinFunction :: evaluateAtTime(time);
}


double PeriodicPiecewiseLinFunction :: evaluateVelocityAtTime(double time)
// Returns the value of the receiver at time 'time'. 'time' should be
// one of the dates of the receiver (currently there is no interpola-
// tion between two points).
{
    double add, last;

    if ( !this->dates.giveSize() ) {
        OOFEM_ERROR("Undefined dates and values!");
    }

    if ( addTF && !domain->giveFunction(addTF) ) {
        OOFEM_ERROR("Undefined time function to add!");
    }

    if ( addTF ) {
        add = domain->giveFunction(addTF)->evaluateVelocityAtTime(time);
    } else {
        add = 0.;
    }

    // periodicity
    last = dates.at( this->dates.giveSize() ); // time of last date
    if ( ( period >= 0.0 ) && ( time > last ) ) {
        double d = ( time - last ) / period; // periods after last
        time = last + ( d - floor(d) - 1. ) * period;
    }

    return add + PiecewiseLinFunction :: evaluateVelocityAtTime(time);
}

void
PeriodicPiecewiseLinFunction :: initializeFrom(InputRecord &ir)
{
    PiecewiseLinFunction :: initializeFrom(ir);
    period = -1.0;
    IR_GIVE_OPTIONAL_FIELD(ir, period, _IFT_PeriodicPiecewiseLinFunction_period);
    addTF = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, addTF, _IFT_PeriodicPiecewiseLinFunction_addtf);
}


void PeriodicPiecewiseLinFunction :: giveInputRecord(DynamicInputRecord &input)
{
    PiecewiseLinFunction :: giveInputRecord(input);
    input.setField(this->period, _IFT_PeriodicPiecewiseLinFunction_period);
    input.setField(this->addTF, _IFT_PeriodicPiecewiseLinFunction_addtf);
}
} // end namespace oofem
