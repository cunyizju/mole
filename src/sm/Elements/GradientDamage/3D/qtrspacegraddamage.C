// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/3D/qtrspacegraddamage.h"
#include "../sm/Materials/Structural/structuralms.h"
#include "../sm/CrossSections/structuralcrosssection.h"
#include "fei/fei3dtetlin.h"
#include "dofman/node.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/domain.h"
#include "input/cltypes.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

#include <cstdio>

namespace oofem {
REGISTER_Element(QTRSpaceGradDamage);

FEI3dTetLin QTRSpaceGradDamage :: interpolation_lin;

QTRSpaceGradDamage :: QTRSpaceGradDamage(int n, Domain *aDomain) :  QTRSpace(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 10;
    nPrimVars = 3;
    nSecNodes = 4;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
}


void
QTRSpaceGradDamage :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    Structural3DElement :: initializeFrom(ir);
}


void
QTRSpaceGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode <= 4 ) {
        answer = {D_u, D_v, D_w, G_0};
    } else {
        answer = {D_u, D_v, D_w};
    }
}


void
QTRSpaceGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v, D_w};
}


void
QTRSpaceGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{
  /*    if ( inode <= 4 ) {
      answer = {G_0};
    } else {
      answer = {};
    }
  */
  
}
  

void
QTRSpaceGradDamage :: computeGaussPoints()
// Sets up the array containing the four Gauss points of the receiver.
{
    integrationRulesArray.resize(1);
    integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 7) );
    this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
}


void
QTRSpaceGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN(answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QTRSpaceGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

}
