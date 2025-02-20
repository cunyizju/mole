// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef gpinitmodule_h
#define gpinitmodule_h

#include "bc/initmodule.h"

#define _IFT_GPInitModule_Name "gpinitmodule"

namespace oofem {
/**
 * Represents GP (Gauss point) initialization module.
 * This module reads certain internal variables
 * of all Gauss points from a file.
 * In this way, one can specify e.g. initial damage
 * and initial stresses computed by another model.
 */
class GPInitModule : public InitModule
{
public:
    /// Creates empty GPInitModule.
    GPInitModule(int n, EngngModel * e);

    /// Destructor
    virtual ~GPInitModule();

    void initializeFrom(InputRecord &ir) override;
    void doInit() override;
    const char *giveClassName() const override { return "GPInitModule"; }
};
} // namespace oofem
#endif
