// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "tf1.h"
#include "solvers/timestep.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(TF1);

void
TF1 :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
// Returns the value of the receiver at time and given position respecting the mode.
{
    FloatArray cd(3);
    double t, k, c, h, result;
    answer.resize(1);

    k = 2.0e-4;
    c = 1.0;
    h = 200.0;


    t = tStep->giveTargetTime();
    for ( int i = 1; i <= coords.giveSize(); i++ ) {
        cd.at(i) = coords.at(i);
    }

    result = -1.e-5 - k *macbra( atan ( c *t + cd.at ( 2 ) - h ) );

    if ( ( mode == VM_Incremental ) && ( !tStep->isTheFirstStep() ) ) {
        t = tStep->giveTargetTime() - tStep->giveTimeIncrement();
        result -= -1.e-5 - k *macbra( atan ( c *t + cd.at ( 2 ) - h ) );
    }

    answer.at(1) = result;
}

void
TF1 :: initializeFrom(InputRecord &ir)
{
}
} // end namespace oofem
