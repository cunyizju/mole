// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef diidynamic_h
#define diidynamic_h

#include "sm/EngineeringModels/structengngmodel.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrx.h"
#include "solvers/timediscretizationtype.h"

///@name Input fields for DIIDynamic
//@{
#define _IFT_DIIDynamic_Name "diidynamic"
#define _IFT_DIIDynamic_deltat "deltat"
#define _IFT_DIIDynamic_ddtScheme "ddtscheme"
#define _IFT_DIIDynamic_gamma "gamma"
#define _IFT_DIIDynamic_beta "beta"
#define _IFT_DIIDynamic_eta "eta"
#define _IFT_DIIDynamic_delta "delta"
#define _IFT_DIIDynamic_theta "theta"
//@}

namespace oofem {
/**
 * This class implements Direct Implicit Integration of Dynamic problem
 *
 * Solution of this problem is series of loading cases, maintained as sequence of
 * time-steps. This solution is in form of linear equation system @f$ A\cdot x = b @f$.
 * The damping Matrix is assumed to be modeled as Rayleigh damping ( @f$ C = \eta M + \delta K @f$)
 *
 * Initial conditions is specified at time 0.
 *
 * Solution procedure described in:
 * A SURVEY OF DIRECT TIME-INTEGRATION METHODS IN COMPUTATIONAL STRUCTURAL DYNAMICS - II. IMPLICIT METHODS
 * K. Subbaraj and M. A. Dokainish
 * Computers & Structures Vol. 32. No. 6. pp. 1387-1401, 1989
 *
 * @author Andreas Feymark
 *
 */
class DIIDynamic : public StructuralEngngModel
{
protected:
    bool initFlag;
    std :: unique_ptr< SparseMtrx > stiffnessMatrix;
    FloatArray loadVector, previousLoadVector, rhs, rhs2;
    FloatArray displacementVector, velocityVector, accelerationVector;
    FloatArray previousDisplacementVector, previousVelocityVector, previousAccelerationVector;
    FloatArray previousIncrementOfDisplacement;
    FloatArray help;
    double a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
    double gamma, beta, deltaT;
    double eta, delta;
    double theta;

    LinSystSolverType solverType;
    SparseMtrxType sparseMtrxType;

    TimeDiscretizationType previousTimeDiscretization;
    TimeDiscretizationType initialTimeDiscretization;

    /// Numerical method used to solve the problem
    std :: unique_ptr< SparseLinearSystemNM > nMethod;

public:
    DIIDynamic(int i, EngngModel *master = nullptr);
    virtual ~DIIDynamic();

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;
    void updateYourself(TimeStep *tStep) override;
    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    void initializeFrom(InputRecord &ir) override;
    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;
    void initializeYourself(TimeStep *tStep) override;

    // identification
    const char *giveClassName() const override { return "DIIDynamic"; }
    const char *giveInputRecordName() const { return _IFT_DIIDynamic_Name; }
    fMode giveFormulation() override { return TL; }

    void printDofOutputAt(FILE *stream, Dof *iDof, TimeStep *tStep) override;

    void timesMtrx(FloatArray &answer, FloatArray &vec, CharType type, Domain *domain, TimeStep *tStep);
    void assembleLoadVector(FloatArray &_loadVector, Domain *domain, ValueModeType mode, TimeStep *tStep);


    void assembleDirichletBcRhsVector (FloatArray& answer, Domain* d, TimeStep *tStep);
    void determineConstants(TimeStep *tStep);
    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

	TimeDiscretizationType giveInitialTimeDiscretization() { return initialTimeDiscretization; }
};
} // end namespace oofem
#endif // diidynamic_h
