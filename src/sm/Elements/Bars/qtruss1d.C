// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/Bars/qtruss1d.h"
#include "fei/fei1dquad.h"
#include "cs/crosssection.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"



namespace oofem {
REGISTER_Element(QTruss1d);

FEI1dQuad QTruss1d::interpolation(1);

QTruss1d::QTruss1d(int n, Domain *aDomain) : NLStructuralElement(n, aDomain)
{
    numberOfDofMans = 3;
}


void
QTruss1d::initializeFrom(InputRecord &ir)
{
    StructuralElement::initializeFrom(ir);
}

void
QTruss1d::giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = { D_u };
}

int
QTruss1d::computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords)
{
    this->interpolation.local2global(answer, lcoords, FEIElementGeometryWrapper(this) );
    return 1;
}

void
QTruss1d::computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    answer = this->giveStructuralCrossSection()->giveRealStress_1d(strain, gp, tStep);
}

void
QTruss1d::computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = this->giveStructuralCrossSection()->giveStiffnessMatrix_1d(rMode, gp, tStep);
}

void
QTruss1d::computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = this->giveStructuralCrossSection()->giveStiffnessMatrix_dPdF_1d(rMode, gp, tStep);
}

double
QTruss1d::computeVolumeAround(GaussPoint *gp)
// Returns the length of the receiver. This method is valid only if 1
// Gauss point is used.
{
    double detJ = fabs(this->interpolation.giveTransformationJacobian(gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) ) );
    double weight  = gp->giveWeight();
    return detJ * weight * this->giveCrossSection()->give(CS_Area, gp);
}


void QTruss1d::computeGaussPoints()
// Sets up the array of Gauss Points of the receiver.XF
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize(1);
        integrationRulesArray [ 0 ] = std::make_unique< GaussIntegrationRule >(1, this, 1, 3);
        this->giveCrossSection()->setupIntegrationPoints(* integrationRulesArray [ 0 ], numberOfGaussPoints, this);
    }
}

void
QTruss1d::computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
{
    this->interpolation.evaldNdx(answer, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );
}

void
QTruss1d::computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    this->computeBmatrixAt(gp, answer);
}
} // end namespace oofem
