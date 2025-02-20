// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/*
 * The original idea for this class comes from
 * Dubois-Pelerin, Y.: "Object-Oriented  Finite Elements: Programming concepts and Implementation",
 * PhD Thesis, EPFL, Lausanne, 1992.
 */

#include "input/femcmpnn.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "input/dynamicinputrecord.h"

#include <cstdarg>


namespace oofem {
void
FEMComponent :: saveContext(DataStream &stream, ContextMode mode)
{
    if ( mode & CM_Definition ) {
        if ( !stream.write(number) ) {
            THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
FEMComponent :: restoreContext(DataStream &stream, ContextMode mode)
{
    if ( mode & CM_Definition ) {
        if ( !stream.read(number) ) {
            THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
FEMComponent :: giveInputRecord(DynamicInputRecord &input)
{
    input.setRecordKeywordField( this->giveInputRecordName(), this->giveNumber() );
}


std :: string
FEMComponent :: errorInfo(const char *func) const
{
    return std :: string(this->giveClassName()) + "::" + func + ", number: " + std::to_string(this->giveNumber());
}

void FEMComponent :: initializeFrom(InputRecord &ir)
{
}

int FEMComponent :: checkConsistency()
{
    return 1;
}

} // end namespace oofem
