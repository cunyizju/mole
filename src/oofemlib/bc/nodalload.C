// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/nodalload.h"
#include "engng/classfactory.h"
#include "export/datastream.h"
#include "input/dynamicinputrecord.h"
#include "utility/contextioerr.h"

namespace oofem {
REGISTER_BoundaryCondition(NodalLoad);

void
NodalLoad :: initializeFrom(InputRecord &ir)
{
    Load :: initializeFrom(ir);
    int value = 1;
    IR_GIVE_OPTIONAL_FIELD(ir, value, _IFT_NodalLoad_cstype);
    coordSystemType = ( CoordSystType ) value;
}


void NodalLoad :: giveInputRecord(DynamicInputRecord &input)
{
    Load :: giveInputRecord(input);
    input.setField(this->coordSystemType, _IFT_NodalLoad_cstype);
}


void
NodalLoad :: saveContext(DataStream &stream, ContextMode mode)
{
    Load :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.write(coordSystemType) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
NodalLoad :: restoreContext(DataStream &stream, ContextMode mode)
{
    Load :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        int _val;
        if ( !stream.read(_val) ) {
          THROW_CIOERR(CIO_IOERR);
        }
        coordSystemType = (CoordSystType) _val;
    }
}


} // end namespace oofem
