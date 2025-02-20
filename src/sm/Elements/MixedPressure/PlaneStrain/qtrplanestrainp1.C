// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/MixedPressure/PlaneStrain/qtrplanestrainp1.h"
#include "fei/fei2dtrlin.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(QTrPlaneStrainP1);

FEI2dTrLin QTrPlaneStrainP1 :: interpolation_lin(1, 2);

QTrPlaneStrainP1 :: QTrPlaneStrainP1(int n, Domain *aDomain) : QTrPlaneStrain(n, aDomain), BaseMixedPressureElement()
{
    displacementDofsOrdering = {
        1, 2, 4, 5, 7, 8, 10, 11, 12, 13, 14, 15
    };
    pressureDofsOrdering = {
        3, 6, 9
    };
}


void
QTrPlaneStrainP1 :: computeVolumetricBmatrixAt(GaussPoint *gp, FloatArray &answer, NLStructuralElement *elem)
{
    answer.resize(12);
    FloatMatrix dN;
    elem->giveInterpolation()->evaldNdx( dN, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    for ( int j = 0, k = 0; j < 6; j++, k += 2 ) {
        answer(k)     = dN(j, 0);
        answer(k + 1) = dN(j, 1);
    }
}

void
QTrPlaneStrainP1 :: computePressureNMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}


void
QTrPlaneStrainP1 :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode <= 3 ) {
        answer = {
            D_u, D_v, P_f
        };
    } else {
        answer = {
            D_u, D_v
        };
    }
}



void
QTrPlaneStrainP1 :: giveDofManDofIDMask_u(IntArray &answer)
{
    answer = {
        D_u, D_v
    };
}


void
QTrPlaneStrainP1 :: giveDofManDofIDMask_p(IntArray &answer)
{
    answer = {
        P_f
    };
}



void
QTrPlaneStrainP1 ::  postInitialize()
{
    BaseMixedPressureElement :: postInitialize();
    QTrPlaneStrain :: postInitialize();
}
} // end namespace oofem
