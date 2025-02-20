// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/reinforcement.h"
#include "utility/verbose.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_BoundaryCondition(Reinforcement);

void
Reinforcement :: initializeFrom(InputRecord &ir)
{
#  ifdef VERBOSE
    // VERBOSE_PRINT1 ("Instanciating load ",number)
#  endif
    //Load :: initializeFrom(ir);
    IR_GIVE_FIELD(ir, porosity, _IFT_Reinforcement_porosity);
    IR_GIVE_FIELD(ir, shapefactor,  _IFT_Reinforcement_shapeFactor);
    IR_GIVE_FIELD(ir, permeability, _IFT_Reinforcement_permeability);
}
} // end namespace oofem
