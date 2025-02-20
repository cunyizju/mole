// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "warpingcrosssection.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_CrossSection(WarpingCrossSection);


void
WarpingCrossSection :: initializeFrom(InputRecord &ir)
{
    SimpleCrossSection :: initializeFrom(ir);

    int value;
    IR_GIVE_FIELD(ir, value, _IFT_WarpingCrossSection_WarpingNodeNumber);
    this->WarpingNodeNumber = value;
    //propertyDictionary->add(CS_Thickness, thick);
}

} // end namespace oofem
