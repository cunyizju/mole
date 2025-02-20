// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/Bars/truss1dgraddamage.h"
#include "fei/fei1dlin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"


namespace oofem {
REGISTER_Element(Truss1dGradDamage);
IntArray Truss1dGradDamage :: locationArray_u = {1, 3};
IntArray Truss1dGradDamage :: locationArray_d = {2, 4};
  

Truss1dGradDamage :: Truss1dGradDamage(int n, Domain *aDomain) : Truss1d(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 2;
    nPrimVars = 1;
    nSecNodes = 2;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
}


void
Truss1dGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
  answer = {D_u, G_0};
}


void
Truss1dGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u};
}


void
Truss1dGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
      answer = {G_0};

}

  


void
Truss1dGradDamage :: initializeFrom(InputRecord &ir)
{
    GradientDamageElement :: initializeFrom(ir);
    StructuralElement :: initializeFrom(ir);
}



void
Truss1dGradDamage :: computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep)
{
    GradientDamageElement :: computeStiffnessMatrix(answer, rMode, tStep);
}


void
Truss1dGradDamage :: giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord)
{
    GradientDamageElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord);
}


void
Truss1dGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interp.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}


void
Truss1dGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interp.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

void
Truss1dGradDamage :: computeField(ValueModeType mode, TimeStep* tStep, const FloatArray& lcoords, FloatArray& answer)
{
    FloatArray n, unknown;
    this->interp.evalN( n, lcoords, FEIElementGeometryWrapper(this) );
    this->computeVectorOf({D_u}, mode, tStep, unknown);
    answer.at(1) = n.dotProduct(unknown);

    this->interp.evalN( n, lcoords, FEIElementGeometryWrapper(this) );
    this->computeVectorOf({G_0}, mode, tStep, unknown);
    answer.at(2) = n.dotProduct(unknown);
}



  
void
Truss1dGradDamage :: giveLocationArray_u(IntArray &answer)
{
  answer = locationArray_u;
}

  
void
Truss1dGradDamage :: giveLocationArray_d(IntArray &answer)
{
  answer = locationArray_d;
}


void
Truss1dGradDamage :: postInitialize()
{
  GradientDamageElement:: postInitialize();
  NLStructuralElement :: postInitialize();
}

  
}
