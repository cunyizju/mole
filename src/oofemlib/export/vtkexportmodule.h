// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef vtkexportmodule_h
#define vtkexportmodule_h

#include "export/exportmodule.h"
#include "math/intarray.h"
#include "nodalrecovery/nodalrecoverymodel.h"
#include "input/internalstatevaluetype.h"
#include "dofman/unknowntype.h"
#include "input/valuemodetype.h"

///@name Input fields for VTK export module
//@{
#define _IFT_VTKExportModule_Name "vtk"
#define _IFT_VTKExportModule_cellvars "cellvars"
#define _IFT_VTKExportModule_vars "vars"
#define _IFT_VTKExportModule_primvars "primvars"
#define _IFT_VTKExportModule_stype "stype"
#define _IFT_VTKExportModule_regionstoskip "regionstoskip"
//@}

namespace oofem {
class DofManager;

/**
 * Represents VTK (Visualization Toolkit) export module. It uses VTK file format, Unstructured grid dataset.
 * There is built in support for Region By Region output, taking care about possible nonsmooth character of
 * some internal variables at region boundaries. This, however, is rather complication and since application
 * of VTK is naturally in 3D, the corresponding sections are commented out.
 */
class OOFEM_EXPORT VTKExportModule : public ExportModule
{
protected:
    /// List of InternalStateType values, identifying the selected vars for export.
    IntArray internalVarsToExport;
    /// List of primary unknowns to export.
    IntArray primaryVarsToExport;
    /// List of cell data to export.
    IntArray cellVarsToExport;

    /// Determines how regions should be exported.
    enum modeType {
        wdmode, ///< Whole domain
        rbrmode ///< Region by region.
    };

    modeType outMode;
    modeType mode;

    /// Smoother type.
    NodalRecoveryModel :: NodalRecoveryModelType stype;
    /// Smoother.
    std::unique_ptr<NodalRecoveryModel> smoother;
    /// List of regions to skip.
    IntArray regionsToSkip;

public:
    /// Constructor. Creates empty Output Manager with number n. By default all components are selected.
    VTKExportModule(int n, EngngModel * e);
    /// Destructor
    virtual ~VTKExportModule();

    void initializeFrom(InputRecord &ir) override;

    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void initialize() override;
    void terminate() override;
    const char *giveClassName() const override { return "VTKExportModule"; }
    const char *giveInputRecordName() const { return _IFT_VTKExportModule_Name; }

protected:
    /// Returns the internal smoother.
    NodalRecoveryModel *giveSmoother();

    /// Returns the output stream for given solution step.
    FILE *giveOutputStream(TimeStep *tStep);
    /**
     * Returns corresponding element cell_type.
     * Some common element types are supported, others can be supported via interface concept.
     */
    int giveCellType(Element *tStep);
    /**
     * Returns the number of elements vtk cells.
     */
    int giveNumberOfElementCells(Element *);
    /**
     * Returns number of nodes corresponding to cell type.
     */
    int giveNumberOfNodesPerCell(int cellType);
    /**
     * Returns the element cell geometry.
     */
    void giveElementCell(IntArray &answer, Element *elem, int cell);
    /**
     * Export internal variables.
     */
    void exportIntVars(FILE *stream, TimeStep *tStep);
    /**
     * Export primary variables.
     */
    void exportPrimaryVars(FILE *stream, TimeStep *tStep);
    /**
     * Exports single variable.
     */
    void exportIntVarAs(InternalStateType valID, InternalStateValueType type, FILE *stream, TimeStep *tStep);
    /**
     * Exports single variable.
     */
    void exportPrimVarAs(UnknownType valID, FILE *stream, TimeStep *tStep);
    /**
     * Export variables defined on cells.
     */
    void exportCellVars(FILE *stream, int elemToProcess, TimeStep *tStep);

    /**
     * Assembles the region node map. Also computes the total number of nodes in region.
     * The region are numbered starting from offset+1.
     * if mode == 0 then regionNodalNumbers is array with mapping from global numbering to local region numbering.
     * The i-th value contains the corresponding local region number (or zero, if global numbar is not in region).
     * if mode == 1 then regionNodalNumbers is array with mapping from local to global numbering.
     * The i-th value contains the corresponding global node number.
     */
    int initRegionNodeNumbering(IntArray &regionNodalNumbers, int &regionDofMans,
                                int offset, Domain *domain, int reg, int mode);
    /**
     * Computes total number of nodes (summed Region by Region, nodes on region boundaries are
     * added multiple times.
     */
    int giveTotalRBRNumberOfNodes(Domain *d);
    /**
     * Returns the value of Primary variable at given dof manager.
     * If such variable not directly available in dofman dofs, can use
     * smoother to recover this nodal value.
     */
    void getDofManPrimaryVariable(FloatArray &answer, DofManager *dman, IntArray &dofIDMask,
                                  ValueModeType mode, TimeStep *tStep, InternalStateType iType);
};
} // end namespace oofem
#endif // vtkexportmodule_h
