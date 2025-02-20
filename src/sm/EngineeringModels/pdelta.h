// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef pdelta_h
#define pdelta_h

#include "sm/Solvers/linearStatic/linearstatic.h"
#include "solvers/geneigvalsolvertype.h"
#include "math/sparsegeneigenvalsystemnm.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrx.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "input/nummet.h"

///@name Input fields for LinearStability
//@{
#define _IFT_Pdelta_Name "pdelta"
#define _IFT_Pdelta_rtolv "rtolv"
#define _IFT_Pdelta_stype "stype"
#define _IFT_Pdelta_maxiter "maxiter"
#define _IFT_Pdelta_lumpedInitialStressMatrix "lumped"
//@}

namespace oofem {

/**
 * This class implements p-delta analysis, where the effects of normal force on deformed configuration
 * is taken into account by means of initial stress matrix.
 *
 * Solution of this problem is base on equation in the form of: @f$ (K+K_\sigma(r))\cdot r=f @f$.
 * This is a nonlinear problem solved using simple iteration method.
 */
class Pdelta : public LinearStatic
{
private:
    std :: unique_ptr< SparseMtrx > initialStressMatrix;
    double rtolv;
    int maxiter;
    bool lumpedInitialStressMatrix; 

public:
    Pdelta(int i, EngngModel *master=nullptr);
    virtual ~Pdelta() { }

    void solveYourselfAt(TimeStep *tStep) override;
    void initializeFrom(InputRecord &ir) override;

    // identification
    const char *giveInputRecordName() const override { return _IFT_Pdelta_Name; }
    const char *giveClassName() const override { return "Pdelta"; }
};
} // end namespace oofem
#endif // pdelta_h
