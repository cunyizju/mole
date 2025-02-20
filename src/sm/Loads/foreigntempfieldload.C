// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "foreigntempfieldload.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"
#include "fields/field.h"

namespace oofem {
REGISTER_BoundaryCondition(ForeignTemperatureFieldLoad);

void
ForeignTemperatureFieldLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
// Returns the value of the receiver at time and given position respecting the mode.
{
    if(!foreignField){
        OOFEM_ERROR("ForeignTemperatureFieldLoad: foreignField must be assigned from python (is NULL).")  }

    if ( ( mode != VM_Incremental ) && ( mode != VM_Total ) ) {
        OOFEM_ERROR("unknown mode (%s)", __ValueModeTypeToString(mode) );
    }

    if(foreignField->evaluateAt(answer, coords, mode, tStep)){
        OOFEM_ERROR("ForeignTemperatureFieldLoad::foreignField.evaluateAt(...) failed.");
    }
}

void
ForeignTemperatureFieldLoad :: initializeFrom(InputRecord &ir)
{
}
} // end namespace oofem
