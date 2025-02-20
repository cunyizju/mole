// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "error/meshqualityerrorestimator.h"
#include "input/element.h"
#include "input/elementgeometrytype.h"
#include "math/mathfem.h"
#include "dofman/node.h"
#include "math/integrationrule.h"
#include "fei/feinterpol.h"
#include "math/gausspoint.h"

namespace oofem {
double MeshQualityErrorEstimator :: giveElementError(EE_ErrorType type, Element *elem, TimeStep *tStep)
{
    // A good plan would probably be to let the element determines its own quality if they implement some interface.
    // otherwise use a sane default.
    double error;
    FEInterpolation *fei = elem->giveInterpolation();
    IntegrationRule *ir = elem->giveDefaultIntegrationRulePtr();
    if ( fei && ir ) {
        error = this->computeJacobianError(* fei, * ir, elem);
    } else {
        switch ( elem->giveGeometryType() ) {
        case EGT_triangle_1:
            error = this->computeTriangleRadiusError(elem);
            break;
        case EGT_triangle_2:
            error = this->computeTriangleRadiusError(elem);
            break;
        default:
            error = 0.0;
            break;
        }
    }
    return error;
}

double MeshQualityErrorEstimator :: computeTriangleRadiusError(Element *elem)
{
    // Outside/inside circle radius fraction based for quality measurement.
    // Zero for a perfect triangle,
    const auto &c1 = elem->giveNode(1)->giveCoordinates();
    const auto &c2 = elem->giveNode(2)->giveCoordinates();
    const auto &c3 = elem->giveNode(3)->giveCoordinates();
    double a = distance(c1, c2);
    double b = distance(c1, c3);
    double c = distance(c2, c3);
    return a * b * c / ( ( b + c - a ) * ( a + c - b ) * ( a + b - c ) ) - 1.0;
    // Reciprocal error would be;
    // (b+c-a)*(a+c-b)*(a+b-c)/(a*b*c - (b+c-a)*(a+c-b)*(a+b-c));
    // Which is safe except for when all points coincide, i.e. a = b = c = 0
}

double MeshQualityErrorEstimator :: computeJacobianError(FEInterpolation &fei, IntegrationRule &ir, Element *elem)
{
    double min_rcond = 1.0, rcond;
    FloatMatrix jac;

    for ( auto &gp: ir ) {
        fei.giveJacobianMatrixAt( jac, gp->giveNaturalCoordinates(), FEIElementGeometryWrapper(elem) );
        rcond = jac.computeReciprocalCondition() * sgn( jac.giveDeterminant() ); // Signed rcond. as inverted mappings are particularly bad.
        if ( rcond < min_rcond ) {
            min_rcond = rcond;
        }
    }
    return min_rcond < 1e-6 ? 1e6 : 1.0 / min_rcond; // Cap it to avoid overflow.
}

double MeshQualityErrorEstimator :: giveValue(EE_ValueType type, TimeStep *tStep)
{
    double error = 0.0;
    for ( auto &elem : this->domain->giveElements() ) {
        double temp = this->giveElementError(unknownET, elem.get(), tStep);
        if ( temp > error ) {
            error = temp;
        }
    }
    return error;
}

int MeshQualityErrorEstimator :: estimateError(EE_ErrorMode mode, TimeStep *tStep)
{
    return true;
}

void MeshQualityErrorEstimator :: initializeFrom(InputRecord &ir)
{
    ErrorEstimator :: initializeFrom(ir);
}
} // end namespace oofem
