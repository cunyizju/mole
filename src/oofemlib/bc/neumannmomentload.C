// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/neumannmomentload.h"
#include "engng/classfactory.h"
#include "func/function.h"
#include "input/inputrecord.h"
#include "input/domain.h"
#include "utility/set.h"
#include "input/element.h"
#include "fei/feinterpol.h"
#include "math/gausspoint.h"
#include "solvers/timestep.h"
#include "math/mathfem.h"


namespace oofem {
REGISTER_BoundaryCondition(NeumannMomentLoad);

void
NeumannMomentLoad :: initializeFrom(InputRecord &ir)
{
    BoundaryLoad :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, g, _IFT_NeumannMomentLoad_Gradient);
    p = 0.;
    IR_GIVE_OPTIONAL_FIELD(ir, p, _IFT_NeumannMomentLoad_Constant);
    IR_GIVE_FIELD(ir, cset, _IFT_NeumannMomentLoad_CenterSet);

    xbar.resize(0);
}

void
NeumannMomentLoad :: computeXbar()
{

    xbar.resize(this->giveDomain()->giveNumberOfSpatialDimensions());
    xbar.zero();

    celements = this->giveDomain()->giveSet(cset)->giveElementList();

    double V = 0.0;

    for ( auto elementID : celements ) {

        Element *thisElement = this->giveDomain()->giveElement(elementID);
        FEInterpolation *i = thisElement->giveInterpolation();

        auto iRule = i->giveIntegrationRule(3);

        FloatArray coord;
        for ( auto &gp: *iRule ) {
            FloatArray lcoords = gp->giveNaturalCoordinates();
            double detJ = i->giveTransformationJacobian(lcoords, FEIElementGeometryWrapper(thisElement));

            i->local2global(coord, lcoords, FEIElementGeometryWrapper(thisElement));
            coord.times(gp->giveWeight()*fabs(detJ));

            V += gp->giveWeight()*fabs(detJ);

            xbar.add(coord);
        }
    }
    xbar.times(1.0/V);
}

void
NeumannMomentLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    // we overload general implementation on the boundary load level due
    // to implementation efficiency

    computeXbar();

    double factor;

    if ( ( mode != VM_Total ) && ( mode != VM_Incremental ) ) {
        OOFEM_ERROR("mode not supported");
    }

    OOFEM_ERROR("Should not happen!");

    factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer = componentArray;
    answer.times(factor);
}

void
NeumannMomentLoad :: computeNormal(FloatArray &answer, Element *e, int side)
{

    FloatArray xi;

    if ( this->domain->giveNumberOfSpatialDimensions() == 3 ) {
        xi.resize(2);
        xi(0) = 0.25;
        xi(1) = 0.25;
    } else {
        xi.resize(1);
        xi(0) = 0.5;
    }

    FEInterpolation *interpolation = e->giveInterpolation();

    interpolation->boundaryEvalNormal( answer, side, xi, FEIElementGeometryWrapper(e) );
}

void
NeumannMomentLoad :: computeValueAtBoundary(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode, Element *e, int boundary)
{
    computeXbar();

    FEInterpolation *interpolation = e->giveInterpolation();

    // Compute normal
    FloatArray Normal, lcoords;
    interpolation->global2local(lcoords, coords, FEIElementGeometryWrapper(e));
    interpolation->boundaryEvalNormal( Normal, boundary, lcoords, FEIElementGeometryWrapper(e) );

    // Compute x in current configuration
    FloatArray u;
    IntArray bNodes;

    FloatArray xdiff = coords-xbar;;

    double l = p+g.dotProduct(xdiff);

    answer = l*Normal;

    // Finally, compute value of loadtimefunction
    double factor;
    factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer=answer*factor;

}

}
