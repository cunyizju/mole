// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/PlaneStrain/qplanestraingraddamage.h"
#include "fei/fei2dquadlin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(QPlaneStrainGradDamage);

FEI2dQuadLin QPlaneStrainGradDamage :: interpolation_lin(1, 2);

QPlaneStrainGradDamage :: QPlaneStrainGradDamage(int n, Domain *aDomain) : QPlaneStrain(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 8;
    nPrimVars = 2;
    nSecNodes = 4;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
}


void
QPlaneStrainGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const

{
    if ( inode <= 4 ) {
        answer = {D_u, D_v, G_0};
    } else {
        answer = {D_u, D_v};
    }
}


void
QPlaneStrainGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
QPlaneStrainGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
  /*
  if ( inode <= 4 ) {
    answer = {G_0};
  } else {
    answer = {};
  }
  */
}

  

void
QPlaneStrainGradDamage :: initializeFrom(InputRecord &ir)
{
    QPlaneStrain :: initializeFrom(ir);
}

void
QPlaneStrainGradDamage :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 3) );
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
QPlaneStrainGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QPlaneStrainGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}
}
