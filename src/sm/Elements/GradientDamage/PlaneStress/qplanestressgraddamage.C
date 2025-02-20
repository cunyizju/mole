// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/PlaneStress/qplanestressgraddamage.h"
#include "fei/fei2dquadlin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(QPlaneStressGradDamage);

FEI2dQuadLin QPlaneStressGradDamage :: interpolation_lin(1, 2);
IntArray QPlaneStressGradDamage :: locationArray_u = {1,2, 4,5, 7,8, 10,11, 13,14,15,16,17,18,19,20};
IntArray QPlaneStressGradDamage :: locationArray_d = {3,6,9,12};


  
QPlaneStressGradDamage :: QPlaneStressGradDamage(int n, Domain *aDomain) : QPlaneStress2d(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 8;
    nPrimVars = 2;
    nSecNodes = 4;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
    numberOfGaussPoints = 4;
}


void
QPlaneStressGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode <= 4 ) {
        answer = {D_u, D_v, G_0};
    } else {
        answer = {D_u, D_v};
    }
}



  void
QPlaneStressGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v};
}


void
QPlaneStressGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
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
QPlaneStressGradDamage :: initializeFrom(InputRecord &ir)
{
    QPlaneStress2d :: initializeFrom(ir);
}


void
QPlaneStressGradDamage :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 3) );
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
QPlaneStressGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN(answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QPlaneStressGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

void
QPlaneStressGradDamage :: giveLocationArray_u(IntArray &answer)
{
  answer = locationArray_u;
}

  
void
QPlaneStressGradDamage :: giveLocationArray_d(IntArray &answer)
{
  answer = locationArray_d;
}


void
QPlaneStressGradDamage :: postInitialize()
{
  GradientDamageElement:: postInitialize();
  QPlaneStress2d :: postInitialize();
}

  
}
