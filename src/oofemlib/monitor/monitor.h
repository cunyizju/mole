// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef monitor_h
#define monitor_h

#include "oofemcfg.h"

namespace oofem {
class EngngModel;
class TimeStep;
class InputRecord;

///@name Input fields for Monitor
//@{
#define _IFT_DummyMonitor_Name "dummymonitor"


/**
 * Class representing monitor, an abstract class inplementing solution monitor.
 * Monitors are managed by MonitorManager and are invoked at user controlled events, such as solution step termoination.
 * Derived classes can perform specific actions (updating model, plotting proogress, etc) 
 */
class OOFEM_EXPORT Monitor
{
protected:
    int number;
public:
    enum MonitorEvent {
        TimeStepTermination
    };
public:
    Monitor(int n) {
        this->number = n;
    }
    virtual ~Monitor() {}

    /// Initializes receiver according to object description stored in input record.
    virtual void initializeFrom(InputRecord &ir) = 0;
    /**
     * updates the monitor state. This can also mean updating received eModel state.
     * @param eModel instance of EngngModel
     * @param tStep time step
     * @param event event type 
     */
    virtual void update(EngngModel* eModel, TimeStep *tStep, MonitorEvent event) = 0;
 
    /// Returns class name of the receiver.
    virtual const char *giveClassName() const = 0;

};

/** Dummy monitor */
class OOFEM_EXPORT DummyMonitor : public Monitor
{
    public:
      DummyMonitor (int n) : Monitor (n) {}

        /// Initializes receiver according to object description stored in input record.
        void initializeFrom(InputRecord &ir) override;
        void update(EngngModel* eModel, TimeStep *tStep, MonitorEvent event) override;
        virtual const char *giveClassName() const override {return "DummuMonitor"; }
};

} // end namespace oofem
#endif // monitor_h
