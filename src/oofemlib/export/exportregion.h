// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef exportregion_h
#define exportregion_h

#include "export/exportmodule.h"
#include "math/intarray.h"
#include "input/internalstatevaluetype.h"
#include "input/internalstatetype.h"
#include "dofman/unknowntype.h"
#include "input/element.h"

#include <string>
#include <list>

using namespace std;
namespace oofem {
    ///@todo Rename this to something like "ExportPiece" and move it to a separate file (it doesn't actually contain anything VTK-specific).
/// Stores all neccessary data (of a region) in a VTKPiece so it can be exported later.
class OOFEM_EXPORT ExportRegion
{
public:
    ExportRegion()
    {
        numCells = 0;
        numNodes = 0;
    }

    void clear();

    void setNumberOfNodes(int numNodes);
    int giveNumberOfNodes() { return this->numNodes; }

    void setNumberOfCells(int numCells);
    int giveNumberOfCells() { return this->numCells; }

    void setConnectivity(int cellNum, IntArray &nodes);
    IntArray &giveCellConnectivity(int cellNum) { return this->connectivity [ cellNum - 1 ]; }

    void setCellType(int cellNum, int type) { this->elCellTypes.at(cellNum) = type; }
    int giveCellType(int cellNum) { return this->elCellTypes.at(cellNum); }

    void setOffset(int cellNum, int offset) { this->elOffsets.at(cellNum) = offset; }
    int giveCellOffset(int cellNum) { return this->elOffsets.at(cellNum); }

    void setNodeCoords(int nodeNum, const FloatArray &coords);
    FloatArray &giveNodeCoords(int nodeNum) { return this->nodeCoords [ nodeNum - 1 ]; }

    void setNumberOfPrimaryVarsToExport(const IntArray& primVars, int numNodes);
    void setNumberOfLoadsToExport(int numVars, int numNodes);
    void setNumberOfInternalVarsToExport(const IntArray& ists, int numNodes);
    void setNumberOfInternalXFEMVarsToExport(int numVars, int numEnrichmentItems, int numNodes);
    void setNumberOfCellVarsToExport(const IntArray& cellVars, int numCells);

    void setPrimaryVarInNode(UnknownType  type, int nodeNum, FloatArray valueArray);
    FloatArray &givePrimaryVarInNode(UnknownType type, int nodeNum) { return this->nodeVars [ type ] [ nodeNum - 1 ]; }

    void setLoadInNode(int varNum, int nodeNum, FloatArray valueArray);
    FloatArray &giveLoadInNode(int varNum, int nodeNum) { return this->nodeLoads [ varNum - 1 ] [ nodeNum - 1 ]; }

    void setInternalVarInNode(InternalStateType type, int nodeNum, FloatArray valueArray);
    FloatArray &giveInternalVarInNode (InternalStateType type, int nodeNum) { return this->nodeVarsFromIS [ type ] [ nodeNum - 1 ]; }

    void setInternalXFEMVarInNode(int varNum, int eiNum, int nodeNum, FloatArray valueArray);
    FloatArray &giveInternalXFEMVarInNode(int varNum, int eiNum, int nodeNum) { return this->nodeVarsFromXFEMIS [ varNum - 1 ] [ eiNum - 1 ] [ nodeNum - 1 ]; }

    void setCellVar(InternalStateType type, int cellNum, FloatArray valueArray);
    FloatArray &giveCellVar(InternalStateType type, int cellNum) { return this->cellVars [ type ] [ cellNum - 1 ]; }

    IntArray& getMapG2L () {return this->mapG2L;}
    IntArray& getMapL2G () {return this->mapL2G;}
    //void setRegionCells(IntArray& cells) {this->regionElInd = cells;}
    IntArray& getRegionCells () {return this->regionElInd;}

private:
    int numCells;
    int numNodes;
    IntArray elCellTypes;
    IntArray elOffsets;
    // dofman local->global and global->local region map 
    IntArray mapG2L, mapL2G;
    // region elements 
    IntArray regionElInd;


    std::vector< FloatArray >nodeCoords;   // all the nodes in the piece [node][coords]
    std::vector< IntArray >connectivity;   // cell connectivity [cell][nodes]
    std::map< UnknownType, std::vector< FloatArray > >nodeVars;     // [field][node][valArray]
    std::vector< std::vector< FloatArray > >nodeLoads;     // [field][node][valArray]
    std::map< InternalStateType, std::vector< FloatArray > >nodeVarsFromIS;     // [field][node][valArray]
    std::vector< std::vector< std::vector< FloatArray > > >nodeVarsFromXFEMIS;       // [field][ei][node][valArray]
    std::map< InternalStateType, std::vector< FloatArray > >cellVars;     // [el][field][valArray]
};

} // end namespace oofem
#endif // exportregion_h