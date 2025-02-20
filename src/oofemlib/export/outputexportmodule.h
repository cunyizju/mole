// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef outputexportmodule_h_
#define outputexportmodule_h_

#include <vector>

#include "export/exportmodule.h"

///@name Input fields for OutputExportModule
//@{
#define _IFT_OutputExportModule_Name "output"
#define _IFT_OutputExportModule_nodeSets "node_sets"
#define _IFT_OutputExportModule_elementSets "element_sets"
//@}

namespace oofem {
class Domain;
class Element;
class DofManager;

/**
 * Standard output for OOFEM. Most available data is written in plain text.
 * Implementation simply relies on EngngModel::printOutputAt
 *
 * @author Mikael Öhman
 */
class OOFEM_EXPORT OutputExportModule : public ExportModule
{
protected:
    FILE *outputStream;

    /// Set which contains nodes which should be exported
    IntArray nodeSets;

    /// Set which contains elements which should be exported
    IntArray elementSets;

public:
    OutputExportModule(int n, EngngModel * e);
    virtual ~OutputExportModule() {}

    void initializeFrom(InputRecord &ir) override;
    FILE *giveOutputStream();

    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void terminate() override;

    const char *giveClassName() const override { return "OutputExportModule"; }
    const char *giveInputRecordName() const { return _IFT_OutputExportModule_Name; }
};
} // end namespace oofem
#endif // outputexportmodule_h_
