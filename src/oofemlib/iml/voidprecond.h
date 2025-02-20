// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef voidprecond_h
#define voidprecond_h

#include "math/floatarray.h"
#include "math/sparsemtrx.h"
#include "precond.h"

namespace oofem {
/**
 * Class implementing void preconditioner.
 */
class OOFEM_EXPORT VoidPreconditioner : public Preconditioner
{
public:
    /// Constructor. Creates the empty preconditioner.
    VoidPreconditioner(const SparseMtrx & a, InputRecord & attributes);
    /// Constructor. The user should call initializeFrom and init services in this given order to ensure consistency.
    VoidPreconditioner();
    /// Destructor
    virtual ~VoidPreconditioner(void) { }

    void init(const SparseMtrx &a) override { }
    void solve(const FloatArray &rhs, FloatArray &solution) const override { solution = rhs; }
    void trans_solve(const FloatArray &rhs, FloatArray &solution) const override { solution = rhs; }

    const char *giveClassName() const override { return "VoidPreconditioner"; }
};
} // end namespace oofem
#endif // voidprecond_h
