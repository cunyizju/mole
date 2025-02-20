// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "../sm/Elements/GradientDamage/3D/qwedgegraddamage.h"
#include "../sm/Materials/Structural/structuralms.h"
#include "../sm/CrossSections/structuralcrosssection.h"
#include "fei/fei3dwedgelin.h"
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
REGISTER_Element(QWedgeGradDamage);

FEI3dWedgeLin QWedgeGradDamage :: interpolation_lin;

QWedgeGradDamage :: QWedgeGradDamage(int n, Domain *aDomain) :  QWedge(n, aDomain), GradientDamageElement()
    // Constructor.
{
    nPrimNodes = 15;
    nPrimVars = 3;
    nSecNodes = 6;
    nSecVars = 1;
    totalSize = nPrimVars * nPrimNodes + nSecVars * nSecNodes;
    locSize   = nPrimVars * nPrimNodes;
    nlSize    = nSecVars * nSecNodes;
}


void
QWedgeGradDamage :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 9;
    Structural3DElement :: initializeFrom(ir);
}


void
QWedgeGradDamage :: giveDofManDofIDMask(int inode, IntArray &answer) const
// returns DofId mask array for inode element node.
// DofId mask array determines the dof ordering requsted from node.
// DofId mask array contains the DofID constants (defined in cltypes.h)
// describing physical meaning of particular DOFs.
{
    if ( inode <= 6 ) {
        answer = {D_u, D_v, D_w, G_0};
    } else {
        answer = {D_u, D_v, D_w};
    }
}


void
QWedgeGradDamage :: giveDofManDofIDMask_u(IntArray &answer) const
{
  answer = {D_u, D_v, D_w};
}


void
QWedgeGradDamage :: giveDofManDofIDMask_d(IntArray &answer) const
{

  /*    if ( inode <= 6 ) {
      answer = {G_0};
    } else {
      answer = {};
    }
  */
}

  

void
QWedgeGradDamage :: computeGaussPoints()
// Sets up the array containing the four Gauss points of the receiver.
{
    integrationRulesArray.resize(1);
    integrationRulesArray [ 0 ].reset( new GaussIntegrationRule(1, this, 1, 7) );
    this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
}


void
QWedgeGradDamage :: computeNdMatrixAt(GaussPoint *gp, FloatArray &answer)
{
    this->interpolation_lin.evalN( answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QWedgeGradDamage :: computeBdMatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    FloatMatrix dnx;
    this->interpolation_lin.evaldNdx( dnx, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
    answer.beTranspositionOf(dnx);
}

}
