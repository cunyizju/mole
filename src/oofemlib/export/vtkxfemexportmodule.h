// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef vtkxfemexportmodule_h
#define vtkxfemexportmodule_h

#include "export/vtkxmlexportmodule.h"
#include "math/intarray.h"
#include "nodalrecovery/nodalrecoverymodel.h"
#include "utility/interface.h"
#include "input/internalstatevaluetype.h"
#include "math/integrationrule.h"
#include "xfem/xfemmanager.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#ifdef _WIN32
 #define NULL_DEVICE "NUL:"
#else
 #define NULL_DEVICE "/dev/null"
#endif

#include <string>
#include <list>

using namespace std;
namespace oofem {
class Node;

///@name Input fields for VTK XML export module
//@{
#define _IFT_VTKXMLXFemExportModule_Name "vtkxmlxfem"
//@}

/**
 * Represents VTK (Visualization Toolkit) export module for Xfem. 
 * It uses VTK (.vtu) file format, Unstructured grid dataset.
 * The export of data is done on Region By Region basis, possibly taking care about possible
 * nonsmooth character of some internal variables at region boundaries.
 * The exported variables are dermined by XFemManager (FemMan->vtkExportFields).
 * Each region and each enrichment item is exported as a single piece. 
 * When region contains composite cells, these are assumed to be
 * exported in individual subsequent pieces after the default one for the particular region.
 */
class OOFEM_EXPORT VTKXMLXFemExportModule : public VTKXMLExportModule
{
protected:

public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    VTKXMLXFemExportModule(int n, EngngModel *e);
    /// Destructor
    virtual ~VTKXMLXFemExportModule();

    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void terminate() override {}
    const char *giveClassName() const override { return "VTKXMLXFemExportModule"; }

protected:

    /// Returns the filename for the given time step.
    std::string giveOutputFileName(TimeStep *tStep);

    /// Returns the output stream for given solution step.
    std::ofstream giveOutputStream(TimeStep *tStep);

    bool writeXFEMVars(ExportRegion &vtkPiece, int field, int enrItIndex);
    void getNodalVariableFromXFEMST(FloatArray &answer, Node *node, TimeStep *tStep, XFEMStateType xfemstype, Set &region, EnrichmentItem *ei);
    void exportIntVars(ExportRegion &vtkPiece, Set& region, int field, int enrItIndex,  IntArray& internalVarsToExport, NodalRecoveryModel& smoother, TimeStep *tStep);
    void giveDataHeaders(std::string &pointHeader, std::string &cellHeader) override;     // returns the headers
};


} // end namespace oofem
#endif // vtkxfemexportmodule_h
