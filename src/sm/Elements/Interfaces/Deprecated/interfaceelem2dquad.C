// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "interfaceelem2dquad.h"
#include "dofman/node.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatmatrixf.h"
#include "math/floatarray.h"
#include "math/floatarrayf.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "fei/fei2dlinequad.h"
#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(InterfaceElem2dQuad);

FEI2dLineQuad InterfaceElem2dQuad :: interp(1, 2);


InterfaceElem2dQuad :: InterfaceElem2dQuad(int n, Domain *aDomain) :
    StructuralElement(n, aDomain)
{
    numberOfDofMans       = 6;
    axisymmode            = false;
}


void
InterfaceElem2dQuad :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
//
// Returns linear part of geometrical equations of the receiver at gp.
// Returns the linear part of the B matrix
//
{
    ///@todo Use the interpolator everywhere in this file:
    double ksi, n1, n2, n3;

    ksi = gp->giveNaturalCoordinate(1);
    n3  = 1. - ksi * ksi;
    n1  = ( 1. - ksi ) * 0.5 - 0.5 * n3;
    n2  = ( 1. + ksi ) * 0.5 - 0.5 * n3;
    answer.resize(2, 12);
    answer.zero();

    answer.at(1, 2) = answer.at(2, 1) = -n1;
    answer.at(1, 4) = answer.at(2, 3) = -n2;
    answer.at(1, 6) = answer.at(2, 5) = -n3;

    answer.at(1, 8) = answer.at(2, 7) = n1;
    answer.at(1, 10) = answer.at(2, 9) = n2;
    answer.at(1, 12) = answer.at(2, 11) = n3;
}


void
InterfaceElem2dQuad :: computeGaussPoints()
// Sets up the array of Gauss Points of the receiver.
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        //integrationRulesArray[0] = std::make_unique<LobattoIntegrationRule>(1,domain, 1, 2);
        integrationRulesArray [ 0 ] = std::make_unique<GaussIntegrationRule>(1, this, 1, 2);
        integrationRulesArray [ 0 ]->SetUpPointsOnLine(4, _2dInterface);
    }
}


double
InterfaceElem2dQuad :: computeVolumeAround(GaussPoint *gp)
// Returns the length of the receiver. This method is valid only if 1
// Gauss point is used.
{
    double weight  = gp->giveWeight();
    double ksi = gp->giveNaturalCoordinate(1);
    double dn1 = ksi - 0.5;
    double dn2 = ksi + 0.5;
    double dn3 = -2.0 * ksi;

    double x1 = this->giveNode(1)->giveCoordinate(1);
    double x2 = this->giveNode(2)->giveCoordinate(1);
    double x3 = this->giveNode(3)->giveCoordinate(1);

    double y1 = this->giveNode(1)->giveCoordinate(2);
    double y2 = this->giveNode(2)->giveCoordinate(2);
    double y3 = this->giveNode(3)->giveCoordinate(2);

    double dx = ( dn1 * x1 ) + ( dn2 * x2 ) + ( dn3 * x3 );
    double dy = ( dn1 * y1 ) + ( dn2 * y2 ) + ( dn3 * y3 );
    double thickness   = this->giveCrossSection()->give(CS_Thickness, gp);

    double r = 1.0;
    if (this->axisymmode) {
      double n3  = 1. - ksi * ksi;
      double n1  = ( 1. - ksi ) * 0.5 - 0.5 * n3;
      double n2  = ( 1. + ksi ) * 0.5 - 0.5 * n3;
      r = n1*this->giveNode(1)->giveCoordinate(1) + n2*this->giveNode(2)->giveCoordinate(1)+ n3*this->giveNode(3)->giveCoordinate(1);
    }

    return sqrt(dx * dx + dy * dy) * weight * thickness * r;
}


void
InterfaceElem2dQuad :: computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->giveEngTraction_2d(strain, gp, tStep);
}


void
InterfaceElem2dQuad :: computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = static_cast< StructuralInterfaceCrossSection* >(this->giveCrossSection())->give2dStiffnessMatrix_Eng(rMode, gp, tStep);
}


void
InterfaceElem2dQuad :: initializeFrom(InputRecord &ir)
{
    this->axisymmode = ir.hasField(_IFT_InterfaceElem2dQuad_axisymmode);
    StructuralElement :: initializeFrom(ir);
}


void
InterfaceElem2dQuad :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = {D_u, D_v};
}


bool
InterfaceElem2dQuad :: computeGtoLRotationMatrix(FloatMatrix &answer)
{
    FloatArray grad(2);

    //double ksi = gp -> giveCoordinate(1) ;
    double ksi = 0.0; // compute tangent in the middle
    double dn1 = ksi - 0.5;
    double dn2 = ksi + 0.5;
    double dn3 = -2.0 * ksi;

    // tangent
    grad.at(1) = dn1 * this->giveNode(1)->giveCoordinate(1) + dn2 *this->giveNode(2)->giveCoordinate(1) + dn3 *this->giveNode(3)->giveCoordinate(1);
    grad.at(2) = dn1 * this->giveNode(1)->giveCoordinate(2) + dn2 *this->giveNode(2)->giveCoordinate(2) + dn3 *this->giveNode(3)->giveCoordinate(2);
    grad.normalize();

    answer.resize(12, 12);
    for ( int i = 0; i < 6; i++ ) {
        answer.at(i * 2 + 1, i * 2 + 1) = grad.at(1);
        answer.at(i * 2 + 1, i * 2 + 2) = grad.at(2);
        answer.at(i * 2 + 2, i * 2 + 1) = -grad.at(2);
        answer.at(i * 2 + 2, i * 2 + 2) = grad.at(1);
    }

    return 1;
}


FEInterpolation *
InterfaceElem2dQuad :: giveInterpolation() const
{
    return & interp;
}

///@todo Deprecated? Is so, remove it. / Mikael
/*
 * void
 * InterfaceElem2dQuad :: computeStiffnessMatrix (FloatMatrix& answer, MatResponseMode rMode,
 *                                             TimeStep* tStep)
 * // Computes numerically the stiffness matrix of the receiver.
 * {
 * double      dV ;
 * FloatMatrix d, bj, bjl, dbj, t;
 * GaussPoint  *gp ;
 * IntegrationRule* iRule;
 * bool matStiffSymmFlag = this->giveCrossSection()->isCharacteristicMtrxSymmetric(rMode, this->material);
 *
 * answer.resize (computeNumberOfDofs(),computeNumberOfDofs());
 * answer.zero();
 *
 * iRule = integrationRulesArray[giveDefaultIntegrationRule()];
 *
 * for ( GaussPoint *gp: *iRule ) {
 *  this -> computeBmatrixAt(gp, bjl) ;
 *  this -> computeConstitutiveMatrixAt(d, rMode, gp, tStep);
 *  this -> computeGtoLRotationMatrix(t, gp);
 *  bj.beProductOf(bjl,t);
 *  dbj.beProductOf (d, bj) ;
 *  dV = this->computeVolumeAround (gp);
 *  if (matStiffSymmFlag) answer.plusProductSymmUpper (bj,dbj,dV); else answer.plusProductUnsym (bj,dbj,dV);
 *
 * }
 * }
 */
} // end namespace oofem
