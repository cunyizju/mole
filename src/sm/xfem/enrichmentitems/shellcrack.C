// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#include "shellcrack.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_EnrichmentItem(ShellCrack)


ShellCrack :: ShellCrack(int n, XfemManager *xm, Domain *aDomain) : Crack(n, xm, aDomain)
{
    mpEnrichesDofsWithIdArray = {
        D_u, D_v, D_w, W_u, W_v, W_w
    };
}


void ShellCrack :: initializeFrom(InputRecord &ir)
{
    Crack :: initializeFrom(ir);

    this->xiBottom = 0.0;
    this->xiTop = 0.0;
    IR_GIVE_FIELD(ir, this->xiBottom, _IFT_ShellCrack_xiBottom);
    IR_GIVE_FIELD(ir, this->xiTop, _IFT_ShellCrack_xiTop);
}
} // end namespace oofem
