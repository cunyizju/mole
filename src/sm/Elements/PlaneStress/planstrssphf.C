// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStress/planstrssphf.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(PlaneStressPhF2d);

PlaneStressPhF2d::PlaneStressPhF2d( int n, Domain *aDomain ) : PhaseFieldElement( n, aDomain ), 
PlaneStress2d(n, aDomain ) { }

void
PlaneStressPhF2d :: giveDofManDofIDMask(int inode, EquationID, IntArray &answer) const
{
    answer = {D_u, D_v, T_f}; ///@todo add damage dofID later
}

void
PlaneStressPhF2d :: giveDofManDofIDMask_u( IntArray &answer )
{
    answer = {D_u, D_v};
}

void
PlaneStressPhF2d :: giveDofManDofIDMask_d( IntArray &answer )
{
    answer = {T_f};
}



} // end namespace oofem
