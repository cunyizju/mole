// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/initmodule.h"
#include "error/error.h"

namespace oofem {
InitModule :: InitModule(int n, EngngModel *e)
{
    emodel = e;
    number = n;
}


InitModule :: ~InitModule()
{ }


void
InitModule :: initializeFrom(InputRecord &ir)
{
    std :: string initFileName;
    IR_GIVE_FIELD(ir, initFileName, _IFT_InitModule_initfilename);
    if ( ( initStream = fopen(initFileName.c_str(), "r") ) == NULL ) {
        OOFEM_ERROR("failed to open file %s", initFileName.c_str());
    }
}
} // end namespace oofem
