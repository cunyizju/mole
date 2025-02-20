// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef vtkpfemexportmodule_h
#define vtkpfemexportmodule_h

#include "export/vtkbaseexportmodule.h"
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

///@name Input fields for VTK XML export module

using namespace std;
namespace oofem {
class Node;

#define _IFT_VTKPFEMXMLExportModule_Name "vtkpfem"          

class OOFEM_EXPORT VTKPFEMXMLExportModule : public VTKBaseExportModule
{
protected:
    std::ofstream fileStream;
public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    VTKPFEMXMLExportModule(int n, EngngModel *e);
    /// Destructor
    virtual ~VTKPFEMXMLExportModule();

    void initializeFrom(InputRecord &ir) override;

    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
protected:
    /// Returns the filename for the given time step.
    std::string giveOutputFileName(TimeStep *tStep);

    /// Returns the output stream for given solution step.
    std::ofstream giveOutputStream(TimeStep *tStep);

};

} // end namespace oofem
#endif // vtkxmlexportmodule_h
