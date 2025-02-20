// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef solutionstatusexportmodule_h_
#define solutionstatusexportmodule_h_

#include "export/exportmodule.h"

///@name Input fields for SolutionStatusExportModule
//@{
#define _IFT_SolutionStatusExportModule_Name "solutionstatus"
#define _IFT_SolutionStatusExportModule_format "fmt" ///< Filename where rules are defined (normally the input file).
//@}

namespace oofem {
class Domain;
class Element;
class DofManager;


/**
 * Configurable solution status export module. Creates and continuously updates the status file
 according to simulation progress.
 */
class OOFEM_EXPORT SolutionStatusExportModule : public ExportModule
{
protected:
  std::string filename;
  FILE* outputFile;
  std::vector<std::string> recs;

    void checkRecs();
    void printRecsHeader();
public:
    SolutionStatusExportModule(int n, EngngModel * e);
    SolutionStatusExportModule(const SolutionStatusExportModule &) = delete;
    SolutionStatusExportModule &operator=(const SolutionStatusExportModule &) = delete;

    void initialize() override;
    void terminate() override;
    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;

    const char *giveClassName() const override { return "SolutionStatusExportModule"; }
    const char *giveInputRecordName() const { return _IFT_SolutionStatusExportModule_Name; }
};
} // end namespace oofem
#endif // solutionstatusexportmodule_h_
