// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef eigenvaluedynamic_h
#define eigenvaluedynamic_h

#include "engng/engngm.h"
#include "math/sparsegeneigenvalsystemnm.h"
#include "math/sparsemtrx.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "solvers/geneigvalsolvertype.h"
#include "fields/eigenvectorprimaryfield.h"

///@name Input fields for EigenValueDynamic
//@{
#define _IFT_EigenValueDynamic_Name "eigenvaluedynamic"
#define _IFT_EigenValueDynamic_nroot "nroot"
#define _IFT_EigenValueDynamic_rtolv "rtolv"
#define _IFT_EigenValueDynamic_stype "stype"
//@}

namespace oofem {
/**
 * This class implements way for examining eigenvalues and eigenvectors in
 * dynamic problems.
 *
 * Solution of this problem is base on equation in the form of: @f$ K\cdot y=w M\cdot y @f$
 * Currently eigenvalue problem is solved using subspace iteration.
 * Tasks:
 * - Assembling the governing equation in the form @f$ K\cdot y=wM\cdot y@f$.
 * - Creating Numerical method for @f$ K\cdot y=wM\cdot y@f$.
 * - Interfacing Numerical method to Elements.
 */
class EigenValueDynamic : public EngngModel
{
private:
    std :: unique_ptr< EigenVectorPrimaryField > field;
    FloatArray eigVal;
    int activeVector;
    int restoreFlag;
    SparseMtrxType sparseMtrxType;
    int numberOfRequiredEigenValues;
    /// Relative tolerance.
    double rtolv;
    std :: unique_ptr< SparseGeneralEigenValueSystemNM > nMethod;
    GenEigvalSolverType solverType;

public:
    EigenValueDynamic(int i, EngngModel *master=nullptr);
    virtual ~EigenValueDynamic() { }

    void solveYourself() override;
    void doStepOutput(TimeStep *tStep) override;
    void printOutputAt(FILE *file, TimeStep *tStep) override;
    void updateYourself(TimeStep *tStep) override;

    int giveUnknownDictHashIndx(ValueModeType mode, TimeStep *tStep) override;
    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    bool newDofHandling() override { return true; }
    void initializeFrom(InputRecord &ir) override;
    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;
    void setActiveVector(int i) override;

    double giveEigenValue(int eigNum) override { return eigVal.at(eigNum); }

    const char *giveClassName() const override { return "EigenValueDynamic"; }
};
} // end namespace oofem
#endif // eigenvaluedynamic_h
