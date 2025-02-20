// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef GNUPLOTEXPORTMODULE_H_
#define GNUPLOTEXPORTMODULE_H_

#include "export/exportmodule.h"
#include "math/floatarray.h"

#include <unordered_map>
#include <memory>

///@name Input fields for GnuplotExportModule
//@{
#define _IFT_GnuplotExportModule_Name "gnuplot"
// Sum of reaction forces for each Dirichlet BC
#define _IFT_GnuplotExportModule_ReactionForces "reactionforces"
// Special output from boundary conditions
#define _IFT_GnuplotExportModule_BoundaryConditions "boundaryconditions"
#define _IFT_GnuplotExportModule_BoundaryConditionsExtra "boundaryconditionsextra"
// Mesh
#define _IFT_GnuplotExportModule_mesh "mesh"
// XFEM stuff
#define _IFT_GnuplotExportModule_xfem "xfem"
// Node for monitoring displacement
#define _IFT_GnuplotExportModule_monitornode "monitornode"
// Radii for material force evaluation
#define _IFT_GnuplotExportModule_materialforceradii "matforceradii"
// Export length of cracks
#define _IFT_GnuplotExportModule_cracklength "cracklength"
// Special output for interface elements
#define _IFT_GnuplotExportModule_interface_el "interfaceel"
//@}

namespace oofem {
class EnrichmentItem;
class Crack;
class PrescribedGradient;
class PrescribedGradientBCNeumann;
class PrescribedGradientBCWeak;
class PrescribedGradientBC;
class Domain;
class DofManager;
class MaterialForceEvaluator;


/**
 * (Under development) The Gnuplot export module enables OOFEM to export some
 * data in a format that can be directly plotted with Gnuplot.
 *
 * @author Erik Svenning
 *
 * Created on: Jan 29, 2014
 */
class OOFEM_EXPORT GnuplotExportModule : public ExportModule
{
public:
    GnuplotExportModule(int n, EngngModel *e);
    virtual ~GnuplotExportModule();

    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;
    void initialize() override;
    void terminate() override;

    const char *giveClassName() const override { return "GnuplotExportModule"; }
    const char *giveInputRecordName() const { return _IFT_GnuplotExportModule_Name; }

    /**
     * XFEM output
     */
    void outputXFEM(EnrichmentItem &iEI, TimeStep *tStep);
    void outputXFEM(Crack &iCrack, TimeStep *tStep);

    void outputXFEMGeometry(const std::vector< std::vector<FloatArray> > &iEnrItemPoints);

    /**
     * Boundary condition output
     */
    void outputBoundaryCondition(PrescribedGradient &iBC, TimeStep *tStep);
    void outputBoundaryCondition(PrescribedGradientBCNeumann &iBC, TimeStep *tStep);
    void outputBoundaryCondition(PrescribedGradientBCWeak &iBC, TimeStep *tStep);


    /**
     * Help functions
     */
    void outputGradient(int bc, Domain &d, FloatArray &grad, TimeStep *tStep);

    /**
     * Mesh output
     */
    void outputMesh(Domain &iDomain);

    /**
     * Monitor node output
     */
    void outputNodeDisp(DofManager &iDMan, TimeStep *tStep);

    /**
     * Custom output for interface elements
     */
    void outputInterfaceEl(Domain &d, TimeStep *tStep);

    static void WritePointsToGnuplot(const std :: string &iName, const std :: vector< std::vector<FloatArray> > &iPoints);

protected:
    bool mExportReactionForces;
    bool mExportBoundaryConditions;
    bool mExportBoundaryConditionsExtra;
    bool mExportMesh;
    bool mExportXFEM;
    bool mExportCrackLength;
    bool mExportInterfaceEl;

    int mMonitorNodeIndex;
    std::vector<FloatArray> mMonitorNodeDispHist;

    /**
     * Stores the sum of reaction forces for each BC.
     */
    std::vector< std::vector<FloatArray> > mReactionForceHistory;
    std::vector< std::vector<double> > mDispHist;

    void outputReactionForces(TimeStep *tStep);

    /**
     * Evaluator for material forces.
     */
    std :: unique_ptr< MaterialForceEvaluator > mpMatForceEvaluator;
    FloatArray mMatForceRadii;

    /**
     * Store time history of crack lengths
     */
    std::unordered_map< int, std::vector<double> > mCrackLengthHist;
    std::vector<double> mTimeHist;
};
} // end namespace oofem
#endif /* GNUPLOTEXPORTMODULE_H_ */
