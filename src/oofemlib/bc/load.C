// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/load.h"
#include "utility/verbose.h"
#include "solvers/timestep.h"
#include "func/function.h"
#include "input/dynamicinputrecord.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"

namespace oofem {
Load :: Load(int i, Domain *aDomain) :
    GeneralBoundaryCondition(i, aDomain), componentArray(), dofExcludeMask(), reference(false)
{
    timeFunction = 0;
}


const FloatArray &
Load :: giveComponentArray() const
{
    return componentArray;
}


void
Load :: computeValues(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, const IntArray &dofids, ValueModeType mode)
{
    ///@note Backwards compatibility with input files that don't specify dofs.
#if 1
    if ( this->dofs.giveSize() == 0 ) {
        this->computeValueAt(answer, tStep, coords, mode);
        return;
    }
#endif

    FloatArray loaded_dofs;
    this->computeValueAt(loaded_dofs, tStep, coords, mode);

    answer.resize(dofids.giveSize());
    answer.zero();
    for ( int i = 0; i < dofids.giveSize(); ++i ) {
        int index = this->dofs.findFirstIndexOf(dofids[i]);
        if ( index > 0 ) {
            answer[i] = loaded_dofs.at(index);
        }
    }
}


void
Load :: computeComponentArrayAt(FloatArray &answer, TimeStep *tStep, ValueModeType mode)
// Returns an array, the load induced at tStep by the receiver.
{
    double factor;

    factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer = componentArray;
    answer.times(factor);

    if ( !isImposed(tStep) ) {
        answer.zero();
    }
}


void
Load :: initializeFrom(InputRecord &ir)
{
    GeneralBoundaryCondition :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, componentArray, _IFT_Load_components);

    int size = componentArray.giveSize();
    dofExcludeMask.resize(size);
    dofExcludeMask.zero();
    IR_GIVE_OPTIONAL_FIELD(ir, dofExcludeMask, _IFT_Load_dofexcludemask);
    if ( dofExcludeMask.giveSize() != size ) {
        throw ValueInputException(ir, _IFT_Load_dofexcludemask, "dofExcludeMask and componentArray size mismatch");
    } else {
        for ( int i = 1; i <= size; i++ ) {
            if ( dofExcludeMask.at(i) ) {
                componentArray.at(i) = 0.0;
            }
        }
    }

    this->reference = ir.hasField(_IFT_Load_reference);
}


void Load :: giveInputRecord(DynamicInputRecord &input)
{
    GeneralBoundaryCondition :: giveInputRecord(input);
    input.setField(this->componentArray, _IFT_Load_components);
    if ( !this->dofExcludeMask.containsOnlyZeroes() ) {
        input.setField(this->dofExcludeMask, _IFT_Load_dofexcludemask);
    }
    if ( this->reference ) input.setField(_IFT_Load_reference);
}


int
Load :: isDofExcluded(int indx)
{
    if ( ( indx > 0 ) && ( indx <= dofExcludeMask.giveSize() ) ) {
        return dofExcludeMask.at(indx);
    } else {
        OOFEM_ERROR("dof index out of range");
    }

    return 0;
}


void
Load :: scale(double s)
{
    this->componentArray.times(s);
    this->reference = false;
}


void
Load :: saveContext(DataStream &stream, ContextMode mode)
{
    GeneralBoundaryCondition :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {
        contextIOResultType iores;
        if ( ( iores = componentArray.storeYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }
        if ( ( iores = dofExcludeMask.storeYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }
        if ( !stream.write(reference) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
Load :: restoreContext(DataStream &stream, ContextMode mode)
{
    GeneralBoundaryCondition :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        contextIOResultType iores;
        if ( ( iores = componentArray.restoreYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }
        if ( ( iores = dofExcludeMask.restoreYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }
        if ( !stream.read(reference) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}

  
} // end namespace oofem
