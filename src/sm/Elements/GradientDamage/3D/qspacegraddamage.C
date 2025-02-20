// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/3D/qspacegraddamage.h"
#include "fei/fei3dhexalin.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "cs/crosssection.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(QSpaceGradDamage);

FEI3dHexaLin QSpaceGradDamage :: interpolation_lin;

QSpaceGradDamage :: QSpaceGradDamage(int n, Domain *aDomain) :  QSpace(n, aDomain), GradientDamageElement()
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
QSpaceGradDamage :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 27;
    Structural3DElement :: initializeFrom(ir);
}


void
QSpaceGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode <= 4 ) {
        answer = {D_u, D_v, D_w, G_0};
    } else {
        answer = {D_u, D_v, D_w};
    }
}

void
QSpaceGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v, D_w};
}


void
QSpaceGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{

  /*    if ( inode <= 4 ) {
      answer = {G_0};
    } else {
      answer = {};
    }
  */
}

  


void
QSpaceGradDamage :: computeGaussPoints()
// Sets up the array containing the four Gauss points of the receiver.
{
    integrationRulesArray.resize(1);
    integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 7) );
    this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
}



void
QSpaceGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QSpaceGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

}
