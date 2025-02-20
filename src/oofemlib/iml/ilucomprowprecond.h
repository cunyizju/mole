// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef ilucomprowprecond_h
#define ilucomprowprecond_h

#include "math/floatarray.h"
#include "math/intarray.h"
#include "dyncomprow.h"
#include "precond.h"

///@name Input fields for CompRowPrecond
//@{
#define _IFT_CompRow_ILUPrecond_droptol "droptol"
#define _IFT_CompRow_ILUPrecond_partfill "partfill"
//@}

namespace oofem {
/**
 * Implemantation of ILU (Incomplete LU) Preconditioner for compressed row sparse matrices.
 * Fill - up supported.
 */
class OOFEM_EXPORT CompRow_ILUPreconditioner : public Preconditioner
{
private:
    DynCompRow A;

    double drop_tol;
    int part_fill;

public:
    /// Constructor. Initializes the the receiver (constructs the precontioning matrix M) of given matrix.
    CompRow_ILUPreconditioner(const SparseMtrx & A, InputRecord & attributes);
    /// Constructor. The user should call initializeFrom and init services in this given order to ensure consistency.
    CompRow_ILUPreconditioner() : Preconditioner() { }
    /// Destructor
    virtual ~CompRow_ILUPreconditioner(void) { }

    void init(const SparseMtrx &a) override;

    //void initialize (const CompCol &A);
    void initialize(const DynCompRow &A);

    void solve(const FloatArray &x, FloatArray &y) const override;
    void trans_solve(const FloatArray &x, FloatArray &y) const override;

    const char *giveClassName() const override { return "ILUT"; }
    void initializeFrom(InputRecord &ir) override;

protected:
    void qsortCol(IntArray &, FloatArray &, int l, int r);
    int  qsortColPartition(IntArray &, FloatArray &, int l, int r);
};
} // end namespace oofem
#endif // ilucomprowprecond_h
