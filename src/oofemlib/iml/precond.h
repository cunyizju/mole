// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef precond_h
#define precond_h

#include "oofemcfg.h"
#include "math/floatarray.h"
#include "math/sparsemtrx.h"
#include "input/inputrecord.h"

namespace oofem {
/**
 * Abstract class for IML++ compatible preconditioner.
 * Each preconditioner provides solve() and transpose_solve() functionality,
 * so that they can be used interchangeably in the same base iterative method code.
 *
 * Preconditioner matrix M is typically used to compute @f$ M^{-1}\cdot x @f$ or @f$ (M^{\mathrm{T}})^{-1}\cdot x @f$ during the
 * course of a basic iterartion, and thus can be seen as taking some input vector
 * and return a corresponding vector.
 */
class OOFEM_EXPORT Preconditioner
{
public:
    /**
     * Constructor.
     * Initializes the the receiver (constructs the preconditioning matrix M) of given matrix.
     * Calls virtual init service.
     * @param a Sparse matrix to be preconditioned.
     * @param attributes Attributes of receiver.
     */
    Preconditioner(const SparseMtrx & a, InputRecord & attributes);
    /**
     * Constructor.
     * The user should call initializeFrom and init services in this given order to ensure consistency.
     */
    Preconditioner() { }
    /// Destructor
    virtual ~Preconditioner(void) { }

    /**
     * Initializes the receiver (constructs the preconditioning matrix M) of given matrix.
     * Virtual service, to be implemented by derived classes. Should be called after initializeFrom service.
     * @param a Sparse matrix to be preconditioned.
     */
    virtual void init(const SparseMtrx &a) { }

    /**
     * Solves the linear system.
     * @param rhs Right hand side
     * @return Solution.
     */
    FloatArray solve(const FloatArray &rhs) const {
        FloatArray y;
        this->solve(rhs, y);
        return y;
    }
    /**
     * Solves transposed system.
     * @param rhs Right hand side.
     * @return Solution.
     */
    FloatArray trans_solve(const FloatArray &rhs) const {
        FloatArray y;
        this->trans_solve(rhs, y);
        return y;
    }
    /**
     * Solves the linear system.
     * @param rhs Right hand side.
     * @param solution Solution.
     */
    virtual void solve(const FloatArray &rhs, FloatArray &solution) const = 0;
    /**
     * Solves the transposed system.
     * @param rhs Right hand side.
     * @param solution Solution.
     */
    virtual void trans_solve(const FloatArray &rhs, FloatArray &solution) const = 0;

    /// Returns the preconditioner name.
    virtual const char *giveClassName() const { return "Preconditioner"; }
    /// Initializes receiver from given record. Empty implementation.
    virtual void initializeFrom(InputRecord &ir) { }
};
} // end namespace oofem
#endif // precond_h
