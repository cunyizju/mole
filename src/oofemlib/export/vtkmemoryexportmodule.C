// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/vtkmemoryexportmodule.h"
#include "input/element.h"
#include "solvers/timestep.h"
#include "engng/engngm.h"
#include "dofman/node.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_ExportModule(VTKMemoryExportModule)

VTKMemoryExportModule::VTKMemoryExportModule(int n, EngngModel *e) : VTKBaseExportModule(n,e) {}
VTKMemoryExportModule::~VTKMemoryExportModule() {}

void
VTKMemoryExportModule::initializeFrom(InputRecord &ir)
{
  VTKBaseExportModule::initializeFrom(ir);
  
  IR_GIVE_OPTIONAL_FIELD(ir, cellVarsToExport, _IFT_VTKXMLExportModule_cellvars); // Macro - see internalstatetype.h
  IR_GIVE_OPTIONAL_FIELD(ir, internalVarsToExport, _IFT_VTKXMLExportModule_vars); // Macro - see internalstatetype.h
  IR_GIVE_OPTIONAL_FIELD(ir, primaryVarsToExport, _IFT_VTKXMLExportModule_primvars); // Macro - see unknowntype.h
  IR_GIVE_OPTIONAL_FIELD(ir, externalForcesToExport, _IFT_VTKXMLExportModule_externalForces); // Macro - see unknowntype.h
  IR_GIVE_OPTIONAL_FIELD(ir, ipInternalVarsToExport, _IFT_VTKXMLExportModule_ipvars); // Macro - see internalstatetype.h
}

void
VTKMemoryExportModule::doOutput(TimeStep *tStep, bool forcedOutput)
{
  if ( !( testTimeStepOutput(tStep) || forcedOutput ) ) {
    return;
  }

  int nPiecesToExport = this->giveNumberOfRegions(); //old name: region, meaning: sets
  ZZNodalRecoveryModel smoother(emodel->giveDomain(1));
  NodalAveragingRecoveryModel primVarSmoother(emodel->giveDomain(1));

  this->vtkPieces.resize(nPiecesToExport);
  
  // loop over regular pieces only (no support for composite elements at present)
  for ( int pieceNum = 1; pieceNum <= nPiecesToExport; pieceNum++ ) {
    ExportRegion& p = this->vtkPieces[pieceNum-1];
    p.clear();
    // Fills a data struct (VTKPiece) with all the necessary data.
    Set* region = this->giveRegionSet(pieceNum);
    this->setupVTKPiece(p, tStep, *region);
    // Export primary, internal and XFEM variables as nodal quantities
    this->exportPrimaryVars(p, *region, primaryVarsToExport, primVarSmoother, tStep);
    this->exportIntVars(p, *region, internalVarsToExport, smoother, tStep);
    this->exportExternalForces(p, *region, externalForcesToExport, tStep);
    this->exportCellVars(p, *region, cellVarsToExport, tStep);
    
  }
}

std::vector< ExportRegion>& 
VTKMemoryExportModule::getExportRegions() {
  return this->vtkPieces;
}


} // end namespace oofem
