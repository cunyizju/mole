// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef vtkbaseexportmodule_h
#define vtkbaseexportmodule_h

#include "export/exportmodule.h"
#include "export/exportregion.h"
#include "math/intarray.h"
#include "input/internalstatevaluetype.h"
#include "input/internalstatetype.h"
#include "dofman/unknowntype.h"
#include "math/integrationrule.h"
#include "input/element.h"
#include "nodalrecovery/nodalrecoverymodel.h"
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

class VTKBaseExportModule;
  

/**
 * Base class for VTK related export modules. Defines commmon methods.
 */
class OOFEM_EXPORT VTKBaseExportModule : public ExportModule
{
protected:
    /// Map from Voigt to full tensor.
    static IntArray redToFull;

public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    VTKBaseExportModule(int n, EngngModel *e);
    /// Destructor
    virtual ~VTKBaseExportModule();

    void initialize() override;
    void terminate() override;
    const char *giveClassName() const override { return "VTKBaseExportModule"; }
    /**
     * Computes a cell average of an InternalStateType varible based on the weights
     * in the integrationpoints (=> volume/area/length average)
     */
    static void computeIPAverage(FloatArray &answer, IntegrationRule *iRule, Element *elem,  InternalStateType isType, TimeStep *tStep);
    /**
     * Returns corresponding element cell_type.
     * Some common element types are supported, others can be supported via interface concept.
    */
    int giveCellType(Element *element);
    int giveCellType(int num) ;
    
protected:

    virtual void setupVTKPiece(ExportRegion &vtkPiece, TimeStep *tStep, Set& region);
       
    /**
     * Export primary variables.
     */
    virtual void exportPrimaryVars(ExportRegion &piece, Set& region, IntArray& primaryVarsToExport, NodalRecoveryModel& smoother, TimeStep *tStep);
    /**
     * Export internal variables by smoothing.
     */
    virtual void exportIntVars(ExportRegion &piece, Set& region, IntArray& internalVarsToExport, NodalRecoveryModel& smoother, TimeStep *tStep);
    /**
     * Export external forces.
     */
    void exportExternalForces(ExportRegion &piece, int region, TimeStep *tStep);
    
    //void exportXFEMVarAs(XFEMStateType xfemstype, int regionDofMans, int ireg, TimeStep *tStep, EnrichmentItem *ei);
    ///  Exports cell variables (typically internal variables).
    void exportCellVars(ExportRegion &piece, Set& region, IntArray &cellVarsToExport, TimeStep *tStep);
    /**
     * Export external forces.
     */
    void exportExternalForces(ExportRegion &piece, Set& region, IntArray& externalForcesToExport, TimeStep *tStep);


    //  Tries to find the value of a primary field on the given DofManager.
    //  Some elements have different interpolation of some fields, and requires some additional code to compute node values (if available).
    void getNodalVariableFromPrimaryField(FloatArray &answer, DofManager *dman, TimeStep *tStep, UnknownType type, Set& region, NodalRecoveryModel& smoother);
    //
    //  Exports single internal variable by smoothing.
    //
    void getNodalVariableFromIS(FloatArray &answer, Node *node, TimeStep *tStep, InternalStateType type, Set& region, NodalRecoveryModel& smoother);
    // void getNodalVariableFromXFEMST(FloatArray &answer, Node *node, TimeStep *tStep, XFEMStateType xfemstype,Set& region, EnrichmentItem *ei);
    //
    //  Exports a single cell variable (typically an internal variable).
    //
    void getCellVariableFromIS(FloatArray &answer, Element *el, InternalStateType type, TimeStep *tStep);
 

    /// Gives the full form of given symmetrically stored tensors, missing components are filled with zeros.
    static void makeFullTensorForm(FloatArray &answer, const FloatArray &reducedForm, InternalStateValueType vtype);
    /**
     * Returns number of nodes corresponding to cell type
     */
    int giveNumberOfNodesPerCell(int cellType);
    /**
     * Returns the element cell geometry.
     */
    void giveElementCell(IntArray &answer, Element *elem);
    /**
     * Assembles the region node map. Also computes the total number of nodes in region.
     * The region are numbered starting from offset+1.
     * If mode == 0 then regionNodalNumbers is array with mapping from global numbering to local region numbering.
     * The i-th value contains the corresponding local region number (or zero, if global number is not in region).
     * If mode == 1 then regionNodalNumbers is array with mapping from local to global numbering.
     * The i-th value contains the corresponding global node number.
     */
    virtual int initRegionNodeNumbering(ExportRegion& vtkPiece,
                                        Domain *domain, TimeStep *tStep, Set& region); 

    // Export of composite elements (built up from several subcells)
    bool isElementComposite(Element *elem); /// Returns true if element geometry type is composite (not a single cell).
    void exportCompositeElement(ExportRegion &vtkPiece, Element *el, TimeStep *tStep);
    void exportCompositeElement(std::vector< ExportRegion > &vtkPieces, Element *el, TimeStep *tStep);
};

} // end namespace oofem
#endif // vtkbaseexportmodule_h
