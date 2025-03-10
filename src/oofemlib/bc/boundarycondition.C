// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/boundarycondition.h"
#include "solvers/timestep.h"
#include "func/function.h"
#include "utility/verbose.h"
#include "input/dynamicinputrecord.h"
#include "dofman/dof.h"
#include "engng/classfactory.h"
#include "utility/contextioerr.h"
#include "error/error.h"

namespace oofem {
REGISTER_BoundaryCondition(BoundaryCondition);

double BoundaryCondition :: give(Dof *dof, ValueModeType mode, TimeStep *tStep)
{
    if ( mode == VM_Incremental ) {
        return this->give(dof, VM_Total, tStep->giveTargetTime()) - this->give(dof, VM_Total, tStep->giveTargetTime() - tStep->giveTimeIncrement());
    } else {
        return this->give(dof, mode, tStep->giveIntrinsicTime());
    }
}


double BoundaryCondition :: give(Dof *dof, ValueModeType mode, double time)
{
    double factor = 0;
    if ( (mode == VM_Total) || (mode == VM_TotalIntrinsic)) {
        factor = this->giveTimeFunction()->evaluateAtTime(time);
    } else if ( mode == VM_Velocity ) {
        factor = this->giveTimeFunction()->evaluateVelocityAtTime(time);
    } else if ( mode == VM_Acceleration ) {
        factor = this->giveTimeFunction()->evaluateAccelerationAtTime(time);
    } else {
        OOFEM_ERROR("Should not be called for value mode type then total, velocity, or acceleration.");
    }
    int index = this->dofs.findFirstIndexOf( dof->giveDofID() );
    if ( !index ) {
        index = 1;
    }
    double prescribedValue = this->values.at(index);
    return prescribedValue * factor;
}


void
BoundaryCondition :: initializeFrom(InputRecord &ir)
{
    GeneralBoundaryCondition :: initializeFrom(ir);

    if ( ir.hasField(_IFT_BoundaryCondition_values) ) {
        IR_GIVE_FIELD(ir, values, _IFT_BoundaryCondition_values);
    } else {
        double prescribedValue;
        if ( ir.hasField(_IFT_BoundaryCondition_PrescribedValue) ) {
            IR_GIVE_FIELD(ir, prescribedValue, _IFT_BoundaryCondition_PrescribedValue);
        } else {
            IR_GIVE_FIELD(ir, prescribedValue, _IFT_BoundaryCondition_PrescribedValue_d);
        }
        // Backwards compatibility with old input method:
        if ( this->dofs.giveSize() ) {
            values.resize( this->dofs.giveSize() );
        } else {
            values.resize(1);
        }
        values.zero();
        values.add(prescribedValue);
    }
}


void
BoundaryCondition :: giveInputRecord(DynamicInputRecord &input)
{
    GeneralBoundaryCondition :: giveInputRecord(input);
    input.setField(this->values, _IFT_BoundaryCondition_values);
}


void
BoundaryCondition :: setPrescribedValue(double s)
{
    values.zero();
    values.add(s);
}


void
BoundaryCondition :: scale(double s)
{
    values.times(s);
}


void
BoundaryCondition :: saveContext(DataStream &stream, ContextMode mode)
{
    GeneralBoundaryCondition :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {
        contextIOResultType iores;
        if ( (iores = values.storeYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
BoundaryCondition :: restoreContext(DataStream &stream, ContextMode mode)
{
    GeneralBoundaryCondition :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        contextIOResultType iores;
        if ( (iores = values.restoreYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(CIO_IOERR);
        }
    }
}
} // end namespace oofem
