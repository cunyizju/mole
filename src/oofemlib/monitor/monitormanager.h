// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef monitormanager_h
#define monitormanager_h

#include "input/modulemanager.h"
#include "monitor/monitor.h"

namespace oofem {
class EngngModel;

/**
 * Class representing and implementing MonitorManager. It is attribute of EngngModel.
 * It manages individual monitors, which perform monitor - specific output operations.
 * Monitors can be called at user specific events, such as time step termination, etc. 
 * The event type is preopagated to each monitor.
 */
class OOFEM_EXPORT MonitorManager : public ModuleManager< Monitor >
{
public:
    MonitorManager(EngngModel * emodel);
    virtual ~MonitorManager();

    void initializeFrom(InputRecord &ir) override;
    std::unique_ptr<Monitor> CreateModule(const char *name, int num, EngngModel *emodel) override;

    /**
     * Writes the output. Loops over all modules and calls corresponding doOutput module service.
     * @param tStep Time step.
     * @param substepFlag is set to true, only the modules with substepFlag set to true will be processed.
     */
    void update(TimeStep *tStep, Monitor::MonitorEvent event);
    /**
     * Initializes output manager. The corresponding initialize module services are called.
     */
    //void initialize();
    /**
     * Terminates the receiver, the corresponding terminate module services are called.
     */
    //void terminate();
    const char *giveClassName() const override { return "MonitorManager"; }
};
} // end namespace oofem
#endif // monitormanager_h
