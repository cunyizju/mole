// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStress/qplanstrssphf.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(QPlaneStressPhF2d);

QPlaneStressPhF2d::QPlaneStressPhF2d( int n, Domain *aDomain ) : PhaseFieldElement( n, aDomain ), 
QPlaneStress2d(n, aDomain ) { }

void
QPlaneStressPhF2d :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = { D_u, D_v, T_f }; ///@todo add damage dofID later
}

void
QPlaneStressPhF2d::giveDofManDofIDMask_u( IntArray &answer )
{
    answer = { D_u, D_v };
}

void
QPlaneStressPhF2d::giveDofManDofIDMask_d( IntArray &answer )
{
    answer = IntArray{ T_f };
}



} // end namespace oofem
