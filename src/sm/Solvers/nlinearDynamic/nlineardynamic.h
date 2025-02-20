// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef nlineardynamic_h
#define nlineardynamic_h

#include "sm/EngineeringModels/structengngmodel.h"
#include "math/sparsemtrxtype.h"
#include "solvers/sparselinsystemnm.h"
#include "solvers/sparsenonlinsystemnm.h"
#include "solvers/timediscretizationtype.h"

///@name Input fields for NonLinearDynamic
//@{
#define _IFT_NonLinearDynamic_Name "nonlineardynamic"
#define _IFT_NonLinearDynamic_deltat "deltat"
#define _IFT_NonLinearDynamic_refloadmode "refloadmode"
#define _IFT_NonLinearDynamic_nonlocstiff "nonlocstiff"
#define _IFT_NonLinearDynamic_nonlocalext "nonlocalext"
#define _IFT_NonLinearDynamic_ddtScheme "ddtscheme"
#define _IFT_NonLinearDynamic_gamma "gamma"
#define _IFT_NonLinearDynamic_beta "beta"
#define _IFT_NonLinearDynamic_eta "eta"
#define _IFT_NonLinearDynamic_delta "delta"
//@}

namespace oofem {

/**
 * This class implements nonlinear dynamic engineering problem.
 * Solution of this problem is performed  as a series of increments (loading or displacement).
 * At start of Each increment we assemble new tangent stiffness, and iteratively trying
 * to fulfill balance of external and real internal forces
 * at end of load step (see numerical method ).
 * The loading applied can bo of two types:
 * - proportional incremental  loading
 * - non-proportional fixed loading, reflecting the previous history,
 *   but could not be scaled (like dead weight).
 *
 * Tasks:
 * - Creating Numerical method for solving nonlinear problem.
 * - Assembling tangent stiffness matrix.
 * - Interfacing Numerical method to Elements.
 * - Managing time steps.
 *
 * Solution proceedure described in:
 * A SURVEY OF DIRECT TIME-INTEGRATION METHODS IN COMPUTATIONAL STRUCTURAL DYNAMICS - II. IMPLICIT METHODS
 * K. Subbaraj and M. A. Dokainish
 * Computers & Structures Vol. 32. No. 6. pp. 1387-1401, 1989
 *
 * @author Andreas Feymark
 * @author Alper Cesur
 */
class NonLinearDynamic : public StructuralEngngModel
{
protected:
    std :: unique_ptr< SparseMtrx > effectiveStiffnessMatrix, massMatrix;

    LinSystSolverType solverType;
    SparseMtrxType sparseMtrxType;

    int initFlag;
    TimeDiscretizationType initialTimeDiscretization;
    double gamma, beta;
    double eta, delta;
    double a0, a1, a2, a3, a4, a5, a6, a7;

    FloatArray velocityVector, accelerationVector, previousLoadVector;
    FloatArray previousVelocityVector, previousAccelerationVector;
    FloatArray help, rhs, rhs2, previousInternalForces;
    FloatArray previousIncrementOfDisplacement;
    FloatArray previousTotalDisplacement, totalDisplacement,  incrementOfDisplacement;
    FloatArray internalForces, forcesVector;

    int currentIterations, totIterations, MANRMSteps;
    int commInitFlag;
    int nonlocalStiffnessFlag;
    /// Numerical method used to solve the problem.
    std :: unique_ptr< SparseNonLinearSystemNM > nMethod;
    /// Intrinsic time increment.
    double deltaT;

public:
    NonLinearDynamic(int i, EngngModel *master = nullptr);
    virtual ~NonLinearDynamic();

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;

    void printOutputAt(FILE *file, TimeStep *tStep) override;
    void printDofOutputAt(FILE *stream, Dof *iDof, TimeStep *tStep) override;

    void updateYourself(TimeStep *tStep) override;
    void updateComponent(TimeStep *tStep, NumericalCmpn, Domain *d) override;
    void updateSolution(FloatArray &solutionVector, TimeStep *tStep, Domain *d) override;
    void updateInternalRHS(FloatArray &answer, TimeStep *tStep, Domain *d, FloatArray *eNorm) override;
    void updateMatrix(SparseMtrx &mat, TimeStep *tStep, Domain *d) override;
    void updateAttributes(MetaStep *mStep) override;
    void initializeYourself(TimeStep *tStep) override;

    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;

    void initializeFrom(InputRecord &ir) override;
    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    void updateDomainLinks() override;

    // Identification
    const char *giveInputRecordName() const { return _IFT_NonLinearDynamic_Name; }
    const char *giveClassName() const override { return "NonLinearDynamic"; }
    fMode giveFormulation() override { return nonLinFormulation; }
    int useNonlocalStiffnessOption() override { return this->nonlocalStiffnessFlag; }
    int giveUnknownDictHashIndx(ValueModeType mode, TimeStep *tStep) override { return ( int ) mode; }
    void timesMtrx(FloatArray &answer, FloatArray &vec, CharType type, Domain *domain, TimeStep *tStep);

    TimeDiscretizationType giveInitialTimeDiscretization() { return initialTimeDiscretization; }

    int estimateMaxPackSize(IntArray &commMap, DataStream &buff, int packUnpackType) override;
#ifdef __PARALLEL_MODE
    LoadBalancer *giveLoadBalancer() override;
    LoadBalancerMonitor *giveLoadBalancerMonitor() override;
#endif

protected:
    void assemble(SparseMtrx &answer, TimeStep *tStep, const MatrixAssembler &ma,
                  const UnknownNumberingScheme &, Domain *domain) override;

    void proceedStep(int di, TimeStep *tStep);
    void determineConstants(TimeStep *tStep);

    void packMigratingData(TimeStep *tStep) override;
    void unpackMigratingData(TimeStep *tStep) override;
};
} // end namespace oofem
#endif // nlineardynamic_h
