// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef quasicontinuumvtkxmlexportmodule_h
#define quasicontinuumvtkxmlexportmodule_h

#include "export/vtkxmlexportmodule.h"
//#include "export/exportmodule.h"
#include "math/intarray.h"
#include "nodalrecovery/nodalrecoverymodel.h"
#include "utility/interface.h"
#include "input/internalstatevaluetype.h"
#include "math/integrationrule.h"
#include "xfem/xfemmanager.h"


#ifdef __VTK_MODULE
 #include <vtkUnstructuredGrid.h>
 #include <vtkSmartPointer.h>
#endif

#include <string>
#include <list>

///@name Input fields for QcVTK XML export module
//@{
#define _IFT_QuasicontinuumVTKXMLExportModule_Name "qcvtkxml"
#define _IFT_QuasicontinuumVTKXMLExportModule_ExportDeactivatedElements "expdeaktelem"
//@}

namespace oofem {
class OOFEM_EXPORT QuasicontinuumVTKXMLExportModule : public VTKXMLExportModule
{
protected:
    int deactivatedElementsExportFlag;

    /// List of InternalStateType values, identifying the selected vars for export.
    IntArray internalVarsToExport;
    /// List of primary unknowns to export.
    IntArray primaryVarsToExport;
    /// List of cell data to export.
    IntArray cellVarsToExport;

public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    QuasicontinuumVTKXMLExportModule(int n, EngngModel * e);
    /// Destructor
    virtual ~QuasicontinuumVTKXMLExportModule();

    void initializeFrom(InputRecord &ir) override;

protected:
    //
    //  Exports single internal variable by smoothing.
    //
    void setupVTKPiece(ExportRegion &vtkPiece, TimeStep *tStep, Set& region) override;
    /**
     * Assembles the region node map. Also computes the total number of nodes in region.
     * The region are numbered starting from offset+1.
     * If mode == 0 then regionNodalNumbers is array with mapping from global numbering to local region numbering.
     * The i-th value contains the corresponding local region number (or zero, if global number is not in region).
     * If mode == 1 then regionNodalNumbers is array with mapping from local to global numbering.
     * The i-th value contains the corresponding global node number.
     */
    int initRegionNodeNumbering(ExportRegion& p, 
                                Domain *domain, TimeStep *tStep, Set& region) override;
};
} // end namespace oofem
#endif // quasicontinuumvtkxmlexportmodule_h
