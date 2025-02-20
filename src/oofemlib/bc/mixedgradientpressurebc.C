// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/mixedgradientpressurebc.h"
#include "math/floatarray.h"
#include "engng/engngm.h"
#include "input/element.h"
#include "fei/feinterpol.h"
#include "utility/set.h"

namespace oofem {
double MixedGradientPressureBC :: domainSize()
{
    int nsd = this->domain->giveNumberOfSpatialDimensions();
    double domain_size = 0.0;
    // This requires the boundary to be consistent and ordered correctly.
    Set *set = this->giveDomain()->giveSet(this->set);
    const IntArray &boundaries = set->giveBoundaryList();

    for ( int pos = 1; pos <= boundaries.giveSize() / 2; ++pos ) {
        Element *e = this->giveDomain()->giveElement( boundaries.at(pos * 2 - 1) );
        int boundary = boundaries.at(pos * 2);
        FEInterpolation *fei = e->giveInterpolation();
        domain_size += fei->evalNXIntegral( boundary, FEIElementGeometryWrapper(e) );
    }
    return domain_size / nsd;
}


void MixedGradientPressureBC :: initializeFrom(InputRecord &ir)
{
    GeneralBoundaryCondition :: initializeFrom(ir);

    FloatArray devGradient;
    double pressure;

    IR_GIVE_FIELD(ir, devGradient, _IFT_MixedGradientPressure_devGradient);
    IR_GIVE_FIELD(ir, pressure, _IFT_MixedGradientPressure_pressure);

    this->setPrescribedDeviatoricGradientFromVoigt(devGradient);
    this->setPrescribedPressure(pressure);
}
} // end namespace oofem
