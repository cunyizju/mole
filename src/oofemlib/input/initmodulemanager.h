// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef initmodulemanager_h
#define initmodulemanager_h

#include "input/modulemanager.h"
#include "bc/initmodule.h"
#include "input/datareader.h"

namespace oofem {
class EngngModel;

/**
 * Class representing and implementing InitModuleManager. It is attribute of EngngModel.
 * It manages the init modules, which perform module - specific init oprations.
 */
class OOFEM_EXPORT InitModuleManager : public ModuleManager< InitModule >
{
public:
    InitModuleManager(EngngModel * emodel);
    virtual ~InitModuleManager();

    std::unique_ptr<InitModule> CreateModule(const char *name, int n, EngngModel *emodel) override;

    /**
     * Performs the initialization of individual modules.
     * Loops over all modules and calls corresponding doInit module service.
     */
    void doInit();

    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "InitModuleManager"; }
};
} // end namespace oofem
#endif // initmodulemanager_h
