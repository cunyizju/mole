// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dynamicrelaxationsolver_h
#define dynamicrelaxationsolver_h

#include "solvers/sparselinsystemnm.h"
#include "solvers/sparsenonlinsystemnm.h"
#include "solvers/convergedreason.h"
#include "math/sparsemtrx.h"
#include "math/floatarray.h"
#include "solvers/linesearch.h"
#include "solvers/nrsolver.h"

#include <vector>


///@name Input fields for DynamicRelaxationSolver
//@{
#define _IFT_DynamicRelaxationSolver_Name "drsolver"
//@}

namespace oofem {
class Domain;
class EngngModel;


/**
 * Solves static equilibrium by means of explicit dynamic iterations.
 * @author Mikael Öhman
 */
class OOFEM_EXPORT DynamicRelaxationSolver : public NRSolver
{
public:
    DynamicRelaxationSolver(Domain * d, EngngModel * m);
    virtual ~DynamicRelaxationSolver() {}

    ConvergedReason solve(SparseMtrx &k, FloatArray &R, FloatArray *R0,
                    FloatArray &X, FloatArray &dX, FloatArray &F,
                    const FloatArray &internalForcesEBENorm, double &l, referenceLoadInputModeType rlm,
                    int &nite, TimeStep *) override;

    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "DynamicRelaxationSolver"; }
    const char *giveInputRecordName() const override { return _IFT_DynamicRelaxationSolver_Name; }
};
} // end namespace oofem
#endif // dynamicrelaxationsolver_h
