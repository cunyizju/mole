// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "monitor/monitor.h"
#include "engng/classfactory.h"
#include "input/logger.h"

namespace oofem {

REGISTER_Monitor( DummyMonitor )

void DummyMonitor::initializeFrom(InputRecord &ir) 
{}
void DummyMonitor::update(EngngModel* eModel, TimeStep *tStep, MonitorEvent event) 
{
    OOFEM_LOG_FORCED ("DummyMonitor: received event %d\n", event);
}
} // end namespace oofem