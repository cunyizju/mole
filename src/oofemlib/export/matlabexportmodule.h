// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef matlabexportmodule_h_
#define matlabexportmodule_h_

#include <vector>

#include "export/exportmodule.h"

///@name Input fields for MatlabExportModule
//@{
#define _IFT_MatlabExportModule_Name "matlab"
#define _IFT_MatlabExportModule_mesh "mesh"
#define _IFT_MatlabExportModule_data "data"
#define _IFT_MatlabExportModule_DataNodeSet "datanodeset"   // dofmanager set from which data should be exported //JF 
#define _IFT_MatlabExportModule_area "area"
#define _IFT_MatlabExportModule_specials "specials"
#define _IFT_MatlabExportModule_homogenizeInternalVars "homogenizeivars"
#define _IFT_MatlabExportModule_noScaledHomogenization "noscaling"
// Reaction forces
#define _IFT_MatlabExportModule_ReactionForces "reactionforces"
#define _IFT_MatlabExportModule_DofManList "dofmanlist"
#define _IFT_MatlabExportModule_ReactionForcesNodeSet "reactionforcesnodeset"
// Integration points
#define _IFT_MatlabExportModule_IntegrationPoints "integrationpoints"
#define _IFT_MatlabExportModule_internalVarsToExport "internalvars"
#define _IFT_MatlabExportModule_ElementList "elementlist"
#define _IFT_MatlabExportModule_IPFieldsElSet "ipelset"
//@}

namespace oofem {
/**
 * (Under development) The Matlab export module enables oofem to export the results to a textfile containing the description of the mesh used
 * along with the pertinent results.
 *
 * @author Carl Sandström
 * @author Jim Brouzoulis
 */
class OOFEM_EXPORT MatlabExportModule : public ExportModule
{
protected:
    /// list of InternalStateType values, identifying the selected vars for export
    IntArray internalVarsToExport;
    IntArray elList;
    /// list of primary unknowns to export
    IntArray primaryVarsToExport;
    std :: string functionname;

    FILE *giveOutputStream(TimeStep *);
    std :: vector< double >smax;
    std :: vector< double >smin;

    std :: vector< double > partVolume;
    std :: vector< std :: string > partName;

    double Area, Volume;
    int ndim;

    bool exportMesh;
    bool exportData;
    bool exportArea;
    bool exportSpecials;
    bool exportReactionForces;
    bool exportIntegrationPointFields;
    bool exportHomogenizeIST;

    int reactionForcesNodeSet;
    int dataNodeSet;
    int IPFieldsElSet;

    bool noscaling;

private:
    void computeArea(TimeStep *tStep);

    // Export reaction forces
    IntArray reactionForcesDofManList; // Holds which dof managers reaction forces should be exported from.
    IntArray dataDofManList;           // Holds which dof managers data should be exported from.

public:
    MatlabExportModule(int n, EngngModel * e);
    virtual ~MatlabExportModule();

    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void initialize() override;
    void terminate() override;

    void doOutputMesh(TimeStep *tStep, FILE *FID);
    void doOutputData(TimeStep *tStep, FILE *FID);
    void doOutputSpecials(TimeStep *tStep, FILE *FID);
    void doOutputReactionForces(TimeStep *tStep, FILE *FID);
    void doOutputIntegrationPointFields(TimeStep *tStep, FILE *FID);
    void doOutputHomogenizeDofIDs(TimeStep *tStep, FILE *FID);

    const char *giveClassName() const override { return "MatlabExportModule"; }
    const char *giveInputRecordName() const { return _IFT_MatlabExportModule_Name; }
};
} // end namespace oofem
#endif // matlabexportmodule_h_
