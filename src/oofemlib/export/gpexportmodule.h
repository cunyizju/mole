// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef gpexportmodule_h_
#define gpexportmodule_h_

#include "export/exportmodule.h"

#include <cstdio>

///@name Input fields for Gausspoint export module
//@{
// gpexportmodule consistent with the documentation
#define _IFT_GPExportModule_Name "gpexportmodule"
#define _IFT_GPExportModule_vartypes "vars"
#define _IFT_GPExportModule_ncoords "ncoords"
//@}

namespace oofem {
/**
 * Represents GP (Gauss point) export module.
 * This module writes the coordinates of all Gauss points
 * along with the values of certain internal variables
 * for further processing.
 */
class OOFEM_EXPORT GPExportModule : public ExportModule
{
protected:
    /// Identification numbers of variables to be exported
    IntArray vartypes;
    /// Number of coordinates to be exported (at each Gauss point)
    int ncoords;
    /// List of elements
    IntArray elements;

public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    GPExportModule(int n, EngngModel * e);
    /// Destructor
    virtual ~GPExportModule();

    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void initialize() override;
    void terminate() override;
    const char *giveClassName() const override { return "GPExportModule"; }
    const char *giveInputRecordName() const { return _IFT_GPExportModule_Name; }

protected:
    /// Returns the output stream for given solution step
    FILE *giveOutputStream(TimeStep *tStep);
};
} // namespace oofem

#endif // gpexportmodule_h_
