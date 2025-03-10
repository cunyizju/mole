// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/Bars/qtruss1dgraddamage.h"
#include "fei/fei1dlin.h"
#include "fei/fei1dquad.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(QTruss1dGradDamage);

FEI1dLin QTruss1dGradDamage :: interpolation_lin(1);

QTruss1dGradDamage :: QTruss1dGradDamage(int n, Domain *aDomain) : QTruss1d(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 3;
    nPrimVars = 1;
    nSecNodes = 2;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
}


void
QTruss1dGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode < 3 ) {
        answer = {D_u, G_0};
    } else {
        answer = {D_u};
    }
}


  void
QTruss1dGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u};
}


void
QTruss1dGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
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
QTruss1dGradDamage :: initializeFrom(InputRecord &ir)
{
    StructuralElement :: initializeFrom(ir);
}


void
QTruss1dGradDamage :: computeGaussPoints()
{
    integrationRulesArray.resize( 1 );
    integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 1) );
    this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
}


void
QTruss1dGradDamage :: computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep)
{
    GradientDamageElement :: computeStiffnessMatrix(answer, rMode, tStep);
}


void
QTruss1dGradDamage :: giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord)
{
    GradientDamageElement :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord);
}


void
QTruss1dGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}


void
QTruss1dGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

void QTruss1dGradDamage :: computeField(ValueModeType mode, TimeStep* tStep, const FloatArray& lcoords, FloatArray& answer)
{
    FloatArray n, unknown;
    this->interpolation_lin.evalN( n, lcoords, FEIElementGeometryWrapper(this) );
    this->computeVectorOf({D_u}, mode, tStep, unknown);
    answer.at(1) = n.dotProduct(unknown);

    this->interpolation.evalN( n, lcoords, FEIElementGeometryWrapper(this) );
    this->computeVectorOf({G_0}, mode, tStep, unknown);
    answer.at(2) = n.dotProduct(unknown);
}

}
