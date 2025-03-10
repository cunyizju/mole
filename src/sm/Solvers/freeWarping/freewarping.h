// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef freewarping_h
#define freewarping_h

#include "sm/EngineeringModels/structengngmodel.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrxtype.h"
#include "math/sparsemtrx.h"
#include "math/floatmatrix.h"
#include "input/connectivitytable.h"

#define _IFT_FreeWarping_Name "freewarping"

namespace oofem {
class SparseMtrx;
//class ConnectivityTable;

/**
 * This class implements the free warping engineering problem
 * (evaluation of the warping function and torsional stiffness for a given cross section).
 * Only one time step is needed and the analysis is linear.
 * The material should be linear elastic, or any other material which has a well-defined
 * shear modulus of elasticity.
 *
 * This problem leads to a linear equation system Ax=b
 *
 * Tasks:
 * - Creating Numerical method for solving @f$ K\cdot x=b @f$.
 * - Interfacing Numerical method to Elements.
 */
class FreeWarping : public StructuralEngngModel
{
protected:
    std :: unique_ptr< SparseMtrx > stiffnessMatrix;
    FloatArray loadVector;
    FloatArray displacementVector;

    LinSystSolverType solverType;
    SparseMtrxType sparseMtrxType;
    /// Numerical method used to solve the problem.
    std :: unique_ptr< SparseLinearSystemNM > nMethod;

    /// computed center of gravity
    FloatArray cg;
    FloatMatrix CG;
    FloatArray SolutionAtCG;

    int initFlag; // needed?

public:
    FreeWarping(int i, EngngModel *master = nullptr);
    virtual ~FreeWarping();

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;

    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    //void saveContext(DataStream &stream, ContextMode mode) override;
    //void restoreContext(DataStream &stream, ContextMode mode) override;

    void updateDomainLinks() override;

    void printOutputAt(FILE *file, TimeStep *tStep) override;

    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;

    void initializeFrom(InputRecord &ir) override;

    void getCenterOfGravity(FloatMatrix &answer) { answer = this->CG; }

    // identification
    const char *giveInputRecordName() const { return _IFT_FreeWarping_Name; }
    const char *giveClassName() const override { return "FreeWarping"; }
    fMode giveFormulation() override { return TL; }
    /**
     * Gives the sum of the first moment of area.
     * @return Total moments Sx Sy.
     */
    void computeCenterOfGravity();
    void computeResultAtCenterOfGravity(TimeStep *tStep);
    void updateComputedResults(FloatArray &answer, TimeStep *tStep);
    void updateStiffnessMatrix(SparseMtrx *answer);

    int estimateMaxPackSize(IntArray &commMap, DataStream &buff, int packUnpackType) override;
};
} // end namespace oofem
#endif // freewarping_h
