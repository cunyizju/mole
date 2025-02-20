// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "material/materialmappingalgorithm.h"
#include "math/gausspoint.h"
#include "input/element.h"

namespace oofem {
void
MaterialMappingAlgorithm :: init(Domain *dold, IntArray &type, GaussPoint *gp, Set &elemSet, TimeStep *tStep, bool iCohesiveZoneGP)
{
    FloatArray coords = gp->giveGlobalCoordinates();

    this->__init(dold, type, coords, elemSet, tStep, iCohesiveZoneGP);
}

int
MaterialMappingAlgorithm :: mapVariable(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep)
{
    FloatArray coords = gp->giveGlobalCoordinates();

    return this->__mapVariable(answer, coords, type, tStep);
}
} // end namespace oofem
