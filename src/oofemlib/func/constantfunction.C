// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "func/constantfunction.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"

namespace oofem {
REGISTER_Function(ConstantFunction);

void
ConstantFunction :: initializeFrom(InputRecord &ir)
{
    Function :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, value, _IFT_ConstantFunction_f);
}


void
ConstantFunction :: giveInputRecord(DynamicInputRecord &input)
{
    Function :: giveInputRecord(input);
    input.setField(this->value, _IFT_ConstantFunction_f);
}

void
ConstantFunction :: saveContext(DataStream &stream, ContextMode mode)
{
    Function :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.write(value) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
ConstantFunction :: restoreContext(DataStream &stream, ContextMode mode)
{
    Function :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.read(value) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}

} // end namespace oofem
