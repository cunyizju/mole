// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef adaptlinearstatic_h
#define adaptlinearstatic_h

#include "sm/Solvers/linearStatic/linearstatic.h"
#include "mesher/meshpackagetype.h"

///@name Input fields for AdaptiveLinearStatic
//@{
#define _IFT_AdaptiveLinearStatic_Name "adaptlinearstatic"
#define _IFT_AdaptiveLinearStatic_meshpackage "meshpackage"
//@}

namespace oofem {
/**
 * This class implements an adaptive linear static engineering problem.
 * Multiple loading cases are not supported.
 * Due to linearity of a problem, the complete reanalysis from the beginning
 * is done after adaptive remeshing.
 * Solution steps represent a series of adaptive analyses.
 */
class AdaptiveLinearStatic : public LinearStatic
{
protected:
    /// Meshing package used for refinements.
    MeshPackageType meshPackage;

public:
    AdaptiveLinearStatic(int i, EngngModel *master = nullptr) : LinearStatic(i, master) { }
    virtual ~AdaptiveLinearStatic() { }

    void updateYourself(TimeStep *tStep) override;

    /**
     * Initializes the newly generated discretization state according to previous solution.
     * This process should typically include restoring old solution, instanciating newly
     * generated domain(s) and by mapping procedure.
     */
    int initializeAdaptive(int tStepNumber) override;
    void printOutputAt(FILE *file, TimeStep *tStep) override;

    void restoreContext(DataStream &stream, ContextMode mode) override;

    void updateDomainLinks() override;

    void initializeFrom(InputRecord &ir) override;

    // identification
    const char *giveClassName() const override { return "AdaptiveLinearStatic"; }
    const char *giveInputRecordName() const override { return _IFT_AdaptiveLinearStatic_Name; }
};
} // end namespace oofem
#endif // adaptlinearstatic_h
