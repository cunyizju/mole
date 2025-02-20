// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef linearstatic_h
#define linearstatic_h

#include "sm/EngineeringModels/structengngmodel.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrxtype.h"
#include "input/unknownnumberingscheme.h"

#define _IFT_LinearStatic_Name "linearstatic"

namespace oofem {
class SparseMtrx;

/**
 * This class implements linear static engineering problem.
 * Multiple loading works only if linear elastic material (such as isoLE)  is used.
 * (Other non-linear materials keep load history, so such multiple loading
 * will cause that next step will be assumed as new load increment,
 * not the total new load). Because they always compute real stresses according
 * to reached strain state, they are not able to respond to linear analysis.
 *
 * Solution of this problem is series of loading cases, maintained as sequence of
 * time-steps. This solution is in form of linear equation system Ax=b
 * Tasks:
 * - Creating Numerical method for solving @f$ K\cdot x=b @f$.
 * - Interfacing Numerical method to Elements.
 * - Managing time steps.
 */
class LinearStatic : public StructuralEngngModel
{
protected:
    std :: unique_ptr< SparseMtrx > stiffnessMatrix;
    FloatArray loadVector;
    FloatArray displacementVector;

    LinSystSolverType solverType;
    SparseMtrxType sparseMtrxType;
    /// Numerical method used to solve the problem.
    std :: unique_ptr< SparseLinearSystemNM > nMethod;

    int initFlag;
    EModelDefaultEquationNumbering equationNumbering;

public:
    LinearStatic(int i, EngngModel *master = nullptr);
    virtual ~LinearStatic();

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;

    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    void updateDomainLinks() override;

    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;

    void initializeFrom(InputRecord &ir) override;

    virtual UnknownNumberingScheme &giveEquationNumbering() { return equationNumbering; }

    // identification
    virtual const char *giveInputRecordName() const { return _IFT_LinearStatic_Name; }
    const char *giveClassName() const override { return "LinearStatic"; }
    fMode giveFormulation() override { return TL; }

    int estimateMaxPackSize(IntArray &commMap, DataStream &buff, int packUnpackType) override;
};
} // end namespace oofem
#endif // linearstatic_h
