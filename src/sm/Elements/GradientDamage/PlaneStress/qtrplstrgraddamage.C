// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "Elements/GradientDamage/PlaneStress/qtrplstrgraddamage.h"
#include "fei/fei2dtrlin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(QTrPlaneStressGradDamage);

FEI2dTrLin QTrPlaneStressGradDamage :: interpolation_lin(1, 2);

QTrPlaneStressGradDamage :: QTrPlaneStressGradDamage(int n, Domain *aDomain) : QTrPlaneStress2d(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 6;
    nPrimVars = 2;
    nSecNodes = 3;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
    numberOfGaussPoints = 4;
}


void
QTrPlaneStressGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const

{
    if ( inode <= 3 ) {
        answer = {D_u, D_v, G_0};
    } else {
        answer = {D_u, D_v};
    }
}


  void
QTrPlaneStressGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
QTrPlaneStressGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
  /*
  if ( inode <= 3 ) {
    answer = {G_0};
  } else {
    answer = {};
  }
  */
}



  

void
QTrPlaneStressGradDamage :: initializeFrom(InputRecord &ir)
{
    QTrPlaneStress2d :: initializeFrom(ir);
}


void
QTrPlaneStressGradDamage :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 3) );
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
QTrPlaneStressGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QTrPlaneStressGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}
}
