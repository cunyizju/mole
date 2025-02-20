// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dyncomprow.h"
#include "ilucomprowprecond.h"
#include "utility/verbose.h"

namespace oofem {
CompRow_ILUPreconditioner ::
CompRow_ILUPreconditioner(const SparseMtrx &A, InputRecord &attributes) : Preconditioner(A, attributes)
{ }

void
CompRow_ILUPreconditioner :: initializeFrom(InputRecord &ir)
{
    Preconditioner :: initializeFrom(ir);
    this->drop_tol = 1.e-8;
    IR_GIVE_OPTIONAL_FIELD(ir, this->drop_tol, _IFT_CompRow_ILUPrecond_droptol);

    part_fill = 5;
    IR_GIVE_OPTIONAL_FIELD(ir, part_fill, _IFT_CompRow_ILUPrecond_partfill);
}


void
CompRow_ILUPreconditioner :: init(const SparseMtrx &A)
{
    if ( dynamic_cast< const DynCompRow *>(& A) ) {
        this->A = static_cast< const DynCompRow &>(A);
        this->A.ILUPYourself(part_fill, drop_tol);
    } else {
        OOFEM_ERROR("unsupported sparse matrix type");
    }
}


void
CompRow_ILUPreconditioner :: solve(const FloatArray &x, FloatArray &y) const
{
    A.ILUPsolve(x, y);
}


void
CompRow_ILUPreconditioner :: trans_solve(const FloatArray &x, FloatArray &y) const
{
    A.ILUPtrans_solve(x, y);
}
} // end namespace oofem
