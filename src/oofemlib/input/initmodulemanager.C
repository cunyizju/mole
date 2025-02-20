// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/initmodulemanager.h"
#include "input/modulemanager.h"
#include "engng/classfactory.h"

namespace oofem {
InitModuleManager :: InitModuleManager(EngngModel *emodel) : ModuleManager< InitModule >(emodel)
{ }

InitModuleManager :: ~InitModuleManager()
{ }

void
InitModuleManager :: initializeFrom(InputRecord &ir)
{
    this->numberOfModules = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, numberOfModules, "ninitmodules"); ///@todo This is named differently from _IFT_ModuleManager_nmodules . Should we change or keep it?
}

std::unique_ptr<InitModule> InitModuleManager :: CreateModule(const char *name, int n, EngngModel *emodel)
{
    return classFactory.createInitModule(name, n, emodel);
}

void
InitModuleManager :: doInit()
{
    for ( auto &module: moduleList ) {
        module->doInit();
    }
}
} // end namespace oofem
