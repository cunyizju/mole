// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "Elements/GradientDamage/PlaneStress/trplstrgraddamage.h"
#include "fei/fei2dtrlin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(TrPlaneStressGradDamage);


IntArray TrPlaneStressGradDamage :: locationArray_u = {1, 2, 4, 5, 7, 8};
IntArray TrPlaneStressGradDamage :: locationArray_d = {3, 6, 9};

FEI2dTrLin TrPlaneStressGradDamage :: interpolation_lin(1, 2);

TrPlaneStressGradDamage :: TrPlaneStressGradDamage(int n, Domain *aDomain) : TrPlaneStress2d(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 3;
    nPrimVars = 2;
    nSecNodes = 3;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
    numberOfGaussPoints = 1;
}


void
TrPlaneStressGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const

{
        answer = {D_u, D_v, G_0};
}


  void
TrPlaneStressGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
TrPlaneStressGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
    answer = {G_0};
}



  

void
TrPlaneStressGradDamage :: initializeFrom(InputRecord &ir)
{
    TrPlaneStress2d :: initializeFrom(ir);
}


void
TrPlaneStressGradDamage :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 3) );
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
TrPlaneStressGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
TrPlaneStressGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}


void
TrPlaneStressGradDamage :: giveLocationArray_u(IntArray &answer)
{
  answer = locationArray_u;
}

  
void
TrPlaneStressGradDamage :: giveLocationArray_d(IntArray &answer)
{
  answer = locationArray_d;
}

  

void
TrPlaneStressGradDamage :: postInitialize()
{
  GradientDamageElement:: postInitialize();
  NLStructuralElement :: postInitialize();
}



  
} // end namespace oofem

