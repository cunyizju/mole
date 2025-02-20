// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef poiexportmodule_h
#define poiexportmodule_h

#include "export/exportmodule.h"
#include "math/intarray.h"
#include "input/internalstatetype.h"
#include "dofman/unknowntype.h"

#include <list>
#include <memory>

///@name Input fields for Point-of-Interest export module
//@{
#define _IFT_POIExportModule_Name "poi"
#define _IFT_POIExportModule_vars "vars"
#define _IFT_POIExportModule_primvars "primvars"
#define _IFT_POIExportModule_mtype "mtype"
#define _IFT_POIExportModule_poifilename "poifilename"
//@}

namespace oofem {
class MaterialMappingAlgorithm;

/**
 * Represents POI (Point Of Interest) export module.
 * It is able to perform output on given points, which are inside domain but can have arbitrary position.
 */
class POIExportModule : public ExportModule
{
protected:
    /// POIs data structure.
    struct POI_dataType {
        int id;
        double x, y, z;
        // bp: region is now the set number (which is more flexible)
        int region;
    };

    /// List of InternalStateType values, identifying the selected vars for export.
    IntArray internalVarsToExport;
    /// List of primary unknowns to export.
    IntArray primaryVarsToExport;
    /// List of POIs.
    std :: list< POI_dataType >POIList;

    /// Smoother type.
    enum POIEM_MapperType { POI_CPT, POI_SFT, POI_LST } mtype;
    /// Mapper.
    std :: unique_ptr< MaterialMappingAlgorithm > mapper;
    //

public:
    /// Constructor. Creates empty Output Manager. By default all components are selected.
    POIExportModule(int n, EngngModel * e);
    /// Destructor
    virtual ~POIExportModule();

    void initializeFrom(InputRecord &ir) override;

    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void initialize() override;
    void terminate() override;
    const char *giveClassName() const override { return "POIExportModule"; }
    const char *giveInputRecordName() const { return _IFT_POIExportModule_Name; }

protected:
    void readPOIFile(const std :: string &poiFileName);
    /// Returns the output stream for given solution step
    FILE *giveOutputStream(TimeStep *tStep);
    /**
     * Export internal variables.
     */
    void exportIntVars(FILE *stream, TimeStep *tStep);
    /**
     * Export primary variables.
     */
    void exportPrimaryVars(FILE *stream, TimeStep *tStep);
    /** Exports single variable */
    void exportIntVarAs(InternalStateType valID, FILE *stream, TimeStep *tStep);
    /** Exports single variable */
    void exportPrimVarAs(UnknownType valID, FILE *stream, TimeStep *tStep);
    MaterialMappingAlgorithm *giveMapper();
};
} // end namespace oofem
#endif // poiexportmodule_h
