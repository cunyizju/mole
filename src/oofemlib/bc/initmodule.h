// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef initmodule_h
#define initmodule_h

#include "oofemcfg.h"
#include "input/inputrecord.h"

#include <cstdio>

///@name Input fields for init module
//@{
#define _IFT_InitModule_initfilename "initfile"
//@}

namespace oofem {
class EngngModel;
class TimeStep;

/**
 * Represents init module - a base class for all init modules. InitModule is an abstraction
 * for module performing some specific kind of initialization. The modules can declare necessary component
 * services using the interface concept. The basic class declares the basic services (the general
 * interface).
 * The initialization modules are maintained by InitModuleManager.
 * The initialization for is done only once, at simulation startup by one of above
 * described method.
 */
class OOFEM_EXPORT InitModule
{
protected:
    /// Number.
    int number;
    /// Problem pointer.
    EngngModel *emodel;
    /// Initialization file.
    FILE *initStream;

public:
    /// Constructor. Creates empty Init Module with number n.
    InitModule(int n, EngngModel * e);
    /// Destructor
    virtual ~InitModule();
    /// Initializes receiver according to object description stored in input record.
    virtual void initializeFrom(InputRecord &ir) = 0;
    /// Reads the input. Abstract service.
    virtual void doInit() = 0;
    /// Returns class name of the receiver.
    virtual const char *giveClassName() const { return "InitModule"; }
    /// Error printing helper.
    std :: string errorInfo(const char *func) const { return std :: string(giveClassName()) + func; }
};
} // end namespace oofem
#endif // initmodule_h
