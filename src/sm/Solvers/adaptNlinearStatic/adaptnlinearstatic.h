// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef adaptnlinearstatic_h
#define adaptnlinearstatic_h

#include "sm/Solvers/nlinearStatic/nlinearstatic.h"
#include "math/sparsemtrx.h"
#include "error/errorestimator.h"
#include "mesher/meshpackagetype.h"

///@name Input fields for AdaptiveNonLinearStatic
//@{
#define _IFT_AdaptiveNonLinearStatic_Name "adaptnlinearstatic"
#define _IFT_AdaptiveNonLinearStatic_meshpackage "meshpackage"
#define _IFT_AdaptiveNonLinearStatic_equilmc "equilmc"
#define _IFT_AdaptiveNonLinearStatic_controlmode "controlmode"
#define _IFT_AdaptiveNonLinearStatic_ddm "ddm"
#define _IFT_AdaptiveNonLinearStatic_refloadmode "refloadmode"
#define _IFT_AdaptiveNonLinearStatic_preMappingLoadBalancingFlag "premaplbflag"
//@}

namespace oofem {
/**
 * This class implements Adaptive Non-LinearStatic Engineering problem.
 *
 * Solution of this problem is performed  as a series of increments (loading or displacement).
 * At start of Each increment we assemble new tangent stiffness, and iteratively trying
 * to fulfill balance of external and real internal forces
 * at end of load step (see numerical method ).
 *
 * The error is estimated at the end of each load increment, and based on reached error,
 * the computation continues, or remeshing happens. The solution is then mapped to
 * new mesh. The solution may be taken from current state or if error is too high then is
 * taken from previous equilibrium state.
 *
 * Tasks:
 * - Creating Numerical method for solving nonlinear problem.
 * - Assembling tangent stiffness matrix
 * - Interfacing Numerical method to Elements
 * - Managing time steps
 */
class AdaptiveNonLinearStatic : public NonLinearStatic
{
protected:
    FloatArray d2_totalDisplacement, d2_incrementOfDisplacement;
    MeshPackageType meshPackage;
    /// Flag indication whether to restore equilibrium after adaptive remapping
    int equilibrateMappedConfigurationFlag;
    /**
     * Flag to trigger load balancing before adaptive remapping.
     * The emodel loadBalancingFlag will trigger load balancing after
     * mapping and optional consistency recovery.
     */
    bool preMappingLoadBalancingFlag;
    /**
     * Array storing the load levels reached in corresponding time steps.
     * It is necessary to keep track of this load level history,
     * because after adaptive restart one has to assemble
     * the initial and total load vectors on new mesh ->
     * and for this it is necessary to know the history of loading.
     * The size of this array is equal to numberOfSolutionSteps and
     * should be stored/restored in every context file.
     */
    FloatArray timeStepLoadLevels;

public:
    AdaptiveNonLinearStatic(int i, EngngModel *master = nullptr);
    virtual ~AdaptiveNonLinearStatic();

    void solveYourselfAt(TimeStep *tStep) override;
    void updateYourself(TimeStep *tStep) override;

    void initializeFrom(InputRecord &ir) override;
    double giveUnknownComponent(ValueModeType mode, TimeStep *tStep, Domain *d, Dof *dof) override;

    /**
     * Returns the load level corresponding to given solution step number.
     */
    virtual double giveTimeStepLoadLevel(int istep);
    /**
     * Initializes the newly generated discretization state according to previous solution.
     * This process should typically include restoring old solution, instanciating newly
     * generated domain(s) and by mapping procedure.
     */
    int initializeAdaptive(int tStepNumber) override;
    /**
     * Initializes the receiver state according to state of given source problem.
     * This process should typically include mapping of source solution, internal variable mapping procedures and
     * optionally restoring new global equilibrium.
     */
    virtual int initializeAdaptiveFrom(EngngModel *sourceProblem);
    /**
     * Remaps the solution state to newly given domain. This includes mapping of source solution,
     * internal variable mapping procedures and optionally restoring new global equilibrium.
     * Given domain becomes new domain of receiver.
     */
    virtual int adaptiveRemap(Domain *dNew);

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    void updateDomainLinks() override;

    const char *giveClassName() const override { return "AdaptiveNonLinearStatic"; }
    const char *giveInputRecordName() const override { return _IFT_AdaptiveNonLinearStatic_Name; }

#ifdef __PARALLEL_MODE
    LoadBalancer *giveLoadBalancer() override;
    LoadBalancerMonitor *giveLoadBalancerMonitor() override;
#endif

protected:
    void assembleInitialLoadVector(FloatArray &loadVector, FloatArray &loadVectorOfPrescribed,
                                   AdaptiveNonLinearStatic *sourceProblem, int domainIndx, TimeStep *tStep);
    //void assembleCurrentTotalLoadVector (FloatArray& loadVector, FloatArray& loadVectorOfPrescribed,
    //            AdaptiveNonLinearStatic* sourceProblem, int domainIndx, TimeStep* tStep);
};
} // end namespace oofem
#endif // adaptnlinearstatic_h
