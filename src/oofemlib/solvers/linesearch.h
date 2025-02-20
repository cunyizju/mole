// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef linesearch_h
#define linesearch_h

#include "input/nummet.h"
#include "solvers/convergedreason.h"
#include "math/floatarray.h"

///@name Input fields for LineSearchNM
//@{
#define _IFT_LineSearchNM_lsearchtol "lsearchtol"
#define _IFT_LineSearchNM_lsearchamp "lsearchamp"
#define _IFT_LineSearchNM_lsearchmaxeta "lsearchmaxeta"
//@}

namespace oofem {
class EngngModel;
class SparseMtrx;
class FloatArray;
class TimeStep;

/**
 * This base class is an abstraction/implementation for numerical method solving
 * line search optimization problem.
 */
class OOFEM_EXPORT LineSearchNM : public NumericalMethod
{
public:
    enum LS_status { ls_ok, ls_failed };

    int max_iter;
    double ls_tolerance;
    double amplifFactor;
    double maxEta, minEta;
    FloatArray eta;
    FloatArray prod;

public:
    /// Constructor
    LineSearchNM(Domain * d, EngngModel * m);

    /**
     * Solves the line search optimization problem in the form of @f$ g(r)=0; r_{new}=r_{old}+\eta\delta r; 0 < \eta < 1 @f$,
     * The aim is to find @f$ \eta @f$ so that the @f$ g(r) @f$ has decreased sufficiently.
     * The total solution vector is updated at exit as well as InternalRhs vector.
     * @param r  Old total solution.
     * @param dr Increment of solution.
     * @param F  Old InternalRhs (real internal forces).
     * @param R  Reference incremental Rhs (incremental load).
     * @param R0 Initial Rhs (initial load).
     * @param eqnmask Equation numbers to mask out from computations.
     * @param lambda Scaling of R.
     * @param etaValue Reached eta value.
     * @param status Linesearch status
     * @param tStep Time step.
     */
    ConvergedReason solve(FloatArray &r, FloatArray &dr, FloatArray &F, FloatArray &R, FloatArray *R0,
                    IntArray &eqnmask, double lambda, double &etaValue, LS_status &status, TimeStep *tStep);

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const { return "LineSearchNM"; }

protected:
    void search(int istep, FloatArray &prod, FloatArray &eta, double amp, double maxeta, double mineta, int &status);
};
} // end namespace oofem
#endif // linesearch_h
