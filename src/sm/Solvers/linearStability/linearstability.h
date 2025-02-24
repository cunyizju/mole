// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef linearstability_h
#define linearstability_h

#include "sm/EngineeringModels/structengngmodel.h"
#include "solvers/geneigvalsolvertype.h"
#include "math/sparsegeneigenvalsystemnm.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrx.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "input/nummet.h"

///@name Input fields for LinearStability
//@{
#define _IFT_LinearStability_Name "linearstability"
#define _IFT_LinearStability_nroot "nroot"
#define _IFT_LinearStability_rtolv "rtolv"
#define _IFT_LinearStability_stype "stype"
//@}

namespace oofem {

class EigenVectorPrimaryField;
//class DofDistributedPrimaryField;

/**
 * This class implements way for examining critical load of structure.
 *
 * Solution of this problem is base on equation in the form of: @f$ K\cdot y=w (K_\sigma)y @f$.
 * Currently eigenvalue problem is solved using subspace iteration.
 * The linear static solution, determining normal forces is done in time = 0.
 *
 * Tasks:
 * - Assembling the governing equation in the form.
 * - Creating Numerical method for @f$ K\cdot y=w(K_\sigma)y @f$.
 * - Interfacing Numerical method to Elements.
 */
class LinearStability : public StructuralEngngModel
{
private:
    std :: unique_ptr< SparseMtrx > stiffnessMatrix;
    std :: unique_ptr< SparseMtrx > initialStressMatrix;
    std :: unique_ptr< EigenVectorPrimaryField > field;
    //std :: unique_ptr< DofDistributedPrimaryField > initialDisplacements;
    FloatArray eigVal;

    int numberOfRequiredEigenValues;
    double rtolv;
    /// Numerical method used to solve the problem.
    GenEigvalSolverType solverType;
    std :: unique_ptr< SparseGeneralEigenValueSystemNM > nMethod;
    /// Numerical method used to solve the static problem.
    std :: unique_ptr< SparseLinearSystemNM > nMethodLS;

public:
    LinearStability(int i, EngngModel *master=nullptr);
    virtual ~LinearStability() { }

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;

    void doStepOutput(TimeStep *tStep) override;
    void printOutputAt(FILE *file, TimeStep *tStep) override;
    //virtual void printOutputAt(FILE *file, TimeStep *tStep, const IntArray &nodeSets, const IntArray &elementSets);
    void terminateLinStatic(TimeStep *tStep);
    int requiresNewLsh() { return 0; }
    void updateYourself(TimeStep *tStep) override;

    // the intrinsic time of time step defines active eigen value and corresponding vector,
    // for which values can be requested using
    // giveUnknownComponent method.
    // When DisplacementVector is requested, then if time==0 linear elastic solution displacement are returned,
    // otherwise corresponding eigen vector is considered as displacement vector
    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    int giveUnknownDictHashIndx(ValueModeType mode, TimeStep *tStep) override;
    bool newDofHandling() override { return true; }
    void initializeFrom(InputRecord &ir) override;
    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
    TimeStep *giveNextStep() override;

    double giveEigenValue(int eigNum) override { return eigVal.at(eigNum); }
    void setActiveVector(int i) override;

    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;
    SparseLinearSystemNM *giveNumericalMethodForLinStaticProblem(TimeStep *tStep);

    // identification
    const char *giveInputRecordName() const { return _IFT_LinearStability_Name; }
    const char *giveClassName() const override { return "LinearStability"; }
    fMode giveFormulation() override { return TL; }
};
} // end namespace oofem
#endif // linearstability_h
