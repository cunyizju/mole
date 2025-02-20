// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/exportregion.h"
#include "input/element.h"
#include "math/gausspoint.h"
#include "solvers/timestep.h"
#include "engng/engngm.h"
#include "dofman/node.h"
#include "dofman/dof.h"
#include "material/materialinterface.h"
#include "math/mathfem.h"
#include "input/cltypes.h"
#include "material/material.h"
#include "engng/classfactory.h"
#include "cs/crosssection.h"
#include "input/unknownnumberingscheme.h"

#include <string>
#include <sstream>
#include <ctime>

namespace oofem {

void
ExportRegion::setNumberOfNodes(int numNodes)
{
    this->numNodes = numNodes;
    this->nodeCoords.resize(numNodes);
}

void
ExportRegion::setNumberOfCells(int numCells)
{
    this->numCells = numCells;
    this->connectivity.resize(numCells);
    this->elCellTypes.resize(numCells);
    this->elOffsets.resize(numCells);
}

void
ExportRegion::setConnectivity(int cellNum, IntArray &nodes)
{
    this->connectivity [ cellNum - 1 ] = nodes;
}

void
ExportRegion::setNodeCoords(int nodeNum, const FloatArray &coords)
{
    this->nodeCoords [ nodeNum - 1 ] = coords;
}

void
ExportRegion::setNumberOfPrimaryVarsToExport(const IntArray& primVars, int numNodes)
{
  for (int it=1; it<= primVars.giveSize(); it++) {
    UnknownType type = (UnknownType) primVars.at(it);
    this->nodeVars [type].resize(numNodes);
  }
}

void
ExportRegion::setNumberOfLoadsToExport(int numVars, int numNodes)
{
    this->nodeLoads.resize(numVars);
    for ( int i = 1; i <= numVars; i++ ) {
        this->nodeLoads [ i - 1 ].resize(numNodes);
    }
}

void
ExportRegion::setNumberOfInternalVarsToExport(const IntArray& ists, int numNodes)
{
    for ( int i = 1; i <= ists.giveSize(); i++ ) {
      InternalStateType itype = (InternalStateType) ists.at(i);
        this->nodeVarsFromIS [ itype ].resize(numNodes);
    }
}

void
ExportRegion::setNumberOfInternalXFEMVarsToExport(int numVars, int numEnrichmentItems, int numNodes)
{
    this->nodeVarsFromXFEMIS.resize(numVars);
    for ( int i = 1; i <= numVars; i++ ) {
        this->nodeVarsFromXFEMIS [ i - 1 ].resize(numEnrichmentItems);
        for ( int j = 1; j <= numEnrichmentItems; j++ ) {
            this->nodeVarsFromXFEMIS [ i - 1 ] [ j - 1 ].resize(numNodes);
        }
    }
}

void
ExportRegion::setNumberOfCellVarsToExport(const IntArray& cellVars, int numCells)
{
     for ( int i = 1; i <= cellVars.giveSize(); i++ ) {
       InternalStateType type = (InternalStateType) cellVars.at(i);
        this->cellVars [ type ].resize(numCells);
    }
}

void
ExportRegion::setPrimaryVarInNode(UnknownType type, int nodeNum, FloatArray valueArray)
{
  this->nodeVars [ type ] [ nodeNum - 1 ] = std::move(valueArray);
}

void
ExportRegion::setLoadInNode(int varNum, int nodeNum, FloatArray valueArray)
{
    this->nodeLoads [ varNum - 1 ] [ nodeNum - 1 ] = std::move(valueArray);
}

void
ExportRegion::setInternalVarInNode(InternalStateType type, int nodeNum, FloatArray valueArray)
{
    this->nodeVarsFromIS [ type ] [ nodeNum - 1 ] = std::move(valueArray);
}

void
ExportRegion::setInternalXFEMVarInNode(int varNum, int eiNum, int nodeNum, FloatArray valueArray)
{
    this->nodeVarsFromXFEMIS [ varNum - 1 ] [ eiNum - 1 ] [ nodeNum - 1 ] = std::move(valueArray);
}


void
ExportRegion::setCellVar(InternalStateType type, int cellNum, FloatArray valueArray)
{
    this->cellVars [ type ] [ cellNum - 1 ] = std::move(valueArray);
}

void
ExportRegion::clear()
{
    ///@todo Will this give a memory leak? / JB
    numCells = 0;
    numNodes = 0;
    this->connectivity.clear();
    this->elCellTypes.clear();
    this->elOffsets.clear();
    this->cellVars.clear();
    this->nodeCoords.clear();
    this->nodeVars.clear();
    this->nodeVarsFromIS.clear();
    this->nodeVarsFromXFEMIS.clear();
}

} // end namespace oofem