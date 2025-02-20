// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/exportmodulemanager.h"
#include "input/modulemanager.h"
#include "export/exportmodule.h"
#include "engng/classfactory.h"

namespace oofem {
ExportModuleManager :: ExportModuleManager(EngngModel *emodel) : ModuleManager< ExportModule >(emodel)
{ }

ExportModuleManager :: ~ExportModuleManager()
{ }

void
ExportModuleManager :: initializeFrom(InputRecord &ir)
{
    this->numberOfModules = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, numberOfModules, _IFT_ModuleManager_nmodules);
}

std::unique_ptr<ExportModule> ExportModuleManager :: CreateModule(const char *name, int n, EngngModel *emodel)
{
    return classFactory.createExportModule(name, n, emodel);
}

void
ExportModuleManager :: doOutput(TimeStep *tStep, bool substepFlag)
{
    for ( auto &module: moduleList ) {
        if ( substepFlag ) {
            if ( module->testSubStepOutput() ) {
                module->doOutput(tStep);
            }
        } else {
            module->doOutput(tStep);
        }
    }
}

void
ExportModuleManager :: initialize()
{
    for ( auto &module: moduleList ) {
        module->initialize();
    }
}


void
ExportModuleManager :: terminate()
{
    for ( auto &module: moduleList ) {
        module->terminate();
    }
}
} // end namespace oofem
