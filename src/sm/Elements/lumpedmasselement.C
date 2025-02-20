// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/lumpedmasselement.h"
#include "dofman/dofmanager.h"
#include "dofman/dof.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "engng/classfactory.h"


namespace oofem {
REGISTER_Element(LumpedMassElement);

LumpedMassElement :: LumpedMassElement(int n, Domain *aDomain) : StructuralElement(n, aDomain)
{
    numberOfDofMans = 1;
}


void
LumpedMassElement :: computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep)
{
    int ndofs = this->computeNumberOfDofs();
    answer.resize(ndofs, ndofs);
    answer.zero();
    answer.beDiagonal(this->components);
}


void
LumpedMassElement :: initializeFrom(InputRecord &ir)
{
    StructuralElement :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, dofs, _IFT_LumpedMassElement_dofs);
    IR_GIVE_FIELD(ir, components, _IFT_LumpedMassElement_components);
}


int
LumpedMassElement :: checkConsistency()
{
    int _result = StructuralElement :: checkConsistency();
    if ( this->dofs.giveSize() != this->components.giveSize() ) {
        OOFEM_WARNING("component/dof array size mismatch");
        _result = 0;
    }

    return _result;
}


int
LumpedMassElement :: computeNumberOfDofs()
{
    return dofs.giveSize();
}


void
LumpedMassElement :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = dofs;
}

} // end namespace oofem
