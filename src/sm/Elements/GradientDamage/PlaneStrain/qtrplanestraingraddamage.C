// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/PlaneStrain/qtrplanestraingraddamage.h"
#include "fei/fei2dtrlin.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/intarray.h"
#include "cs/crosssection.h"



namespace oofem {

FEI2dTrLin QTrPlaneStrainGradDamage :: interpolation_lin(1, 2);

QTrPlaneStrainGradDamage :: QTrPlaneStrainGradDamage(int n, Domain *aDomain) : QTrPlaneStrain(n, aDomain), GradientDamageElement()
{
    nPrimNodes = 6;
    nPrimVars = 2;
    nSecNodes = 3;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
}


void
QTrPlaneStrainGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode <= 3 ) {
        answer = {D_u, D_v, G_0};
    } else {
        answer = {D_u, D_v};
    }
}



void
QTrPlaneStrainGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
QTrPlaneStrainGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
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
QTrPlaneStrainGradDamage :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    QTrPlaneStrain :: initializeFrom(ir);
}

void
QTrPlaneStrainGradDamage :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 3) );
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
QTrPlaneStrainGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatArray n;
    this->interpolation_lin.evalN( n, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beNMatrixOf(n, 1);
}

void
QTrPlaneStrainGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}
}
