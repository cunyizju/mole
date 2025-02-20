// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef linearconstraintbc_h
#define linearconstraintbc_h

#include "bc/activebc.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/chartype.h"
#include "input/valuemodetype.h"
#include "dofman/dofmanager.h"
#include "error/error.h"

#include <memory>

#define _IFT_LinearConstraintBC_Name   "linearconstraintbc"

///@name Input fields for active boundary condition
//@{
#define _IFT_LinearConstraintBC_weights "weights"
#define _IFT_LinearConstraintBC_weightsfuncs "weightsltf"
#define _IFT_LinearConstraintBC_rhs "rhs"
#define _IFT_LinearConstraintBC_rhsfuncs "rhsltf"
#define _IFT_LinearConstraintBC_dofmans "dofmans"
#define _IFT_LinearConstraintBC_dofs "dofs"
#define _IFT_LinearConstraintBC_lhstype "lhstype"
#define _IFT_LinearConstraintBC_rhstype "rhstype"

//@}

namespace oofem {
/**
 * Class implementing linear constraint on selected DOFs in the form @f$ \sum_i w_i r_i = c @f$,
 * where @f$ r_i @f$ is i-th degree of freedom entering contraint, w_i is corresponding weight,
 * and @f$ c @f$ is given value. The weights and constant can have associated loadtime function,
 * so they can evolve in time. By default, the Function value for all weights and
 * constant is set to 1.0.
 * This boundary condition is introduced as additional stationary condition in energy
 * functional using Lagrange multiplier, which is an additional degree of freedom introduced
 * by this boundary condition.
 */
class OOFEM_EXPORT LinearConstraintBC : public ActiveBoundaryCondition
{
protected:
    FloatArray weights;
    IntArray weightsTf;
    double rhs;
    int rhsTf;
    IntArray dofmans;
    IntArray dofs;
    std :: unique_ptr< DofManager > md;


    // characteristicType of LHS and RHS contributions (this makes this bc trully general, as one can customize, to which
    // characteristic component the contibution will be assembled)
    IntArray lhsType;
    IntArray rhsType;

public:
    LinearConstraintBC(int n, Domain * d);
    /// Destructor.
    virtual ~LinearConstraintBC();

    void initializeFrom(InputRecord &ir) override;
    const char *giveInputRecordName() const override { return _IFT_LinearConstraintBC_Name; }
    void assemble(SparseMtrx &answer, TimeStep *tStep,
                  CharType type, const UnknownNumberingScheme &r_s, 
                  const UnknownNumberingScheme &c_s, double scale = 1.0,
                  void* lock=nullptr) override;
    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorms=nullptr,
                        void*lock=nullptr) override;

    void giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols,
                            CharType type, const UnknownNumberingScheme &r_s,
                            const UnknownNumberingScheme &c_s) override;

    int giveNumberOfInternalDofManagers() override { return 1; }
    DofManager *giveInternalDofManager(int i) override { return this->md.get(); }

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    const char *giveClassName() const override { return "LinearConstraintBC"; }

protected:
    void giveLocArray(const UnknownNumberingScheme &r_s,  IntArray &locr, int &lambdaeq);
};
} //end of oofem namespace
#endif // LinearConstraintBC_h
