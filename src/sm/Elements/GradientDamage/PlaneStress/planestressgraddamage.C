// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/PlaneStress/planestressgraddamage.h"
#include "fei/fei2dquadlin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(PlaneStressGradDamage);

  IntArray PlaneStressGradDamage :: locationArray_u = {1,2, 4,5, 7,8, 10,11};
  IntArray PlaneStressGradDamage :: locationArray_d = {3,6,9,12};
  
PlaneStressGradDamage :: PlaneStressGradDamage(int n, Domain *aDomain) : PlaneStress2d(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 4;
    nPrimVars = 2;
    nSecNodes = 4;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
    numberOfGaussPoints = 4;
}


void
PlaneStressGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode <= 4 ) {
        answer = {D_u, D_v, G_0};
    } else {
        answer = {D_u, D_v};
    }
}



  void
PlaneStressGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
PlaneStressGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
    answer = {G_0};
}

  


void
PlaneStressGradDamage :: initializeFrom(InputRecord &ir)
{
    PlaneStress2d :: initializeFrom(ir);
}


void
PlaneStressGradDamage :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 3) );
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
PlaneStressGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation.evalN(answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
PlaneStressGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}


void
PlaneStressGradDamage :: giveLocationArray_u(IntArray &answer)
{
  answer = locationArray_u;
}

  
void
PlaneStressGradDamage :: giveLocationArray_d(IntArray &answer)
{
  answer = locationArray_d;
}


void
PlaneStressGradDamage :: postInitialize()
{
  GradientDamageElement:: postInitialize();
  PlaneStress2d :: postInitialize();
}



}
