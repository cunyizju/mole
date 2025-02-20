// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "Elements/MixedPressure/PlaneStrain/quad1planestrainp1.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"


namespace oofem {
REGISTER_Element(Quad1PlaneStrainP1);


Quad1PlaneStrainP1 :: Quad1PlaneStrainP1(int n, Domain *aDomain) : Quad1PlaneStrain(n, aDomain), BaseMixedPressureElement()
{
    displacementDofsOrdering = {
        1, 2, 4, 5, 7, 8, 10, 11
    };
    pressureDofsOrdering = {
        3, 6, 9, 12
    };
}



void
Quad1PlaneStrainP1 :: computeVolumetricBmatrixAt(GaussPoint *gp, FloatArray &answer, NLStructuralElement *elem)
{
    answer.resize(8);
    FloatMatrix dN;
    elem->giveInterpolation()->evaldNdx( dN, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    for ( int j = 0, k = 0; j < 4; j++, k += 2 ) {
        answer(k)     = dN(j, 0);
        answer(k + 1) = dN(j, 1);
    }
}

void
Quad1PlaneStrainP1 :: computePressureNMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    NLStructuralElement *elem = this->giveElement();
    elem->giveInterpolation()->evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}


void
Quad1PlaneStrainP1 :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = {
        D_u, D_v, P_f
    };
}


void
Quad1PlaneStrainP1 :: giveDofManDofIDMask_u(IntArray &answer)
{
    answer = {
        D_u, D_v
    };
}


void
Quad1PlaneStrainP1 :: giveDofManDofIDMask_p(IntArray &answer)
{
    answer = {
        P_f
    };
}

void
Quad1PlaneStrainP1 ::  postInitialize()
{
    BaseMixedPressureElement :: postInitialize();
    Quad1PlaneStrain :: postInitialize();
}
} // end namespace oofem
