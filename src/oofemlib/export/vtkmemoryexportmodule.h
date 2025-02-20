// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef vtkmemoryexportmodule_h
#define vtkmemoryexportmodule_h

#include "export/vtkbaseexportmodule.h"
#include "math/intarray.h"
#include "nodalrecovery/nodalrecoverymodel.h"
#include "input/internalstatevaluetype.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#ifdef _WIN32
 #define NULL_DEVICE "NUL:"
#else
 #define NULL_DEVICE "/dev/null"
#endif

///@name Input fields for VTK XML export module
//@{
#define _IFT_VTKXMLExportModule_cellvars "cellvars"
#define _IFT_VTKXMLExportModule_vars "vars"
#define _IFT_VTKXMLExportModule_primvars "primvars"
#define _IFT_VTKXMLExportModule_externalForces "externalforces"
#define _IFT_VTKXMLExportModule_ipvars "ipvars"
#define _IFT_VTKMemoryExportModule_Name "vtkmemory"
//@}

using namespace std;
namespace oofem {


/** 
 * VTK-like export class intended mainly to provide easy to use Pythonic interface by providing 
 * acccess to VTKPieces.
 */  
class OOFEM_EXPORT VTKMemoryExportModule : public VTKBaseExportModule
{
protected:
    /// List of InternalStateType values, identifying the selected vars for export.
    IntArray internalVarsToExport;
    /// List of primary unknowns to export.
    IntArray primaryVarsToExport;
    /// List of primary unknowns to export.
    IntArray externalForcesToExport;
    /// List of cell data to export.
    IntArray cellVarsToExport;
    /// List of internal variables to export directly in Integration Points (no smoothing to nodes)
    IntArray ipInternalVarsToExport;


  std::vector< ExportRegion > vtkPieces;
public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    VTKMemoryExportModule(int n, EngngModel *e);
    /// Destructor
    virtual ~VTKMemoryExportModule();
    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    std::vector< ExportRegion>& getExportRegions(); 
};

} // end namespace oofem
#endif // vtkmemoryexportmodule_h
