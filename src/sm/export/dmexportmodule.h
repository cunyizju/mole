// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dmexportmodule_h
#define dmexportmodule_h

#include "export/exportmodule.h"

///@name Input fields for DofManExportModule
//@{
#define _IFT_DofManExportModule_Name "dm"
#define _IFT_DofManExportModule_dmlist "dmlist"
//@}

namespace oofem {
/**
 * Represents DofManager export module.
 * This module writes the coordinates of all dof managers
 * along with the values of displacements
 * for further processing.
 * @author Milan Jirasek
 */
class DofManExportModule : public ExportModule
{
protected:
    IntArray dofManList;

public:
    /// Constructor
    DofManExportModule(int n, EngngModel * e);

    /// Destructor
    virtual ~DofManExportModule();

    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    const char *giveClassName() const override { return "DofManExportModuleClass"; }
    const char *giveInputRecordName() const { return _IFT_DofManExportModule_Name; }

protected:
    FILE *giveOutputStream(TimeStep *tStep);
};
} // end namespace
#endif
