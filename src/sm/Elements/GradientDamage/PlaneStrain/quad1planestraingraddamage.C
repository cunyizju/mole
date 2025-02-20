// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "Elements/GradientDamage/PlaneStrain/quad1planestraingraddamage.h"
#include "fei/fei2dquadlin.h"
#include "dofman/node.h"
#include "cs/crosssection.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(Quad1PlaneStrainGradDamage);
  IntArray Quad1PlaneStrainGradDamage :: locationArray_u = {1, 2, 4, 5, 7, 8, 10, 11};
  IntArray Quad1PlaneStrainGradDamage :: locationArray_d = {3, 6, 9, 12};

Quad1PlaneStrainGradDamage :: Quad1PlaneStrainGradDamage(int n, Domain *aDomain) : Quad1PlaneStrain(n, aDomain), GradientDamageElement()
{
  nPrimNodes = 4;
    nPrimVars = 2;
    nSecNodes = 4;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
  
}





  

void
Quad1PlaneStrainGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const

{
    answer = {D_u, D_v, G_0};  
}


void
Quad1PlaneStrainGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
Quad1PlaneStrainGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
  answer = {G_0};   
}

  
  

void
Quad1PlaneStrainGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interp.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
Quad1PlaneStrainGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interp.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

  
void
Quad1PlaneStrainGradDamage :: giveLocationArray_u(IntArray &answer)
{
  answer = locationArray_u;
}

  
void
Quad1PlaneStrainGradDamage :: giveLocationArray_d(IntArray &answer)
{
  answer = locationArray_d;
}
  
void
Quad1PlaneStrainGradDamage :: postInitialize()
{
  GradientDamageElement:: postInitialize();
  NLStructuralElement :: postInitialize();
}


  

} // end namespace oofem


