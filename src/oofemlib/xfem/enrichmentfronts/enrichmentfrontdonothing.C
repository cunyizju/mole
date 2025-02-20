// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfrontdonothing.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_EnrichmentFront(EnrFrontDoNothing)

void EnrFrontDoNothing :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);
}
} // end namespace oofem
