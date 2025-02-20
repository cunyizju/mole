// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "monitor/monitormanager.h"
#include "input/modulemanager.h"
#include "monitor/monitor.h"
#include "engng/classfactory.h"

namespace oofem {
MonitorManager :: MonitorManager(EngngModel *emodel) : ModuleManager< Monitor >(emodel)
{ }

MonitorManager :: ~MonitorManager()
{ }

void
MonitorManager :: initializeFrom(InputRecord &ir)
{
    this->numberOfModules = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, numberOfModules, "nmonitors");
}

std::unique_ptr<Monitor> MonitorManager :: CreateModule(const char *name, int n, EngngModel *emodel)
{
    return classFactory.createMonitor(name, n);
}

void
MonitorManager :: update(TimeStep *tStep, Monitor::MonitorEvent event)
{
    for ( auto &module: moduleList ) {
        module->update(this->emodel, tStep, event);
    }
}

} // end namespace oofem
