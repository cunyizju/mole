// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mapping/mmacontainingelementprojection.h"
#include "mesher/spatiallocalizer.h"
#include "input/domain.h"
#include "input/element.h"
#include "material/material.h"
#include "math/integrationrule.h"
#include "math/gausspoint.h"

namespace oofem {
MMAContainingElementProjection :: MMAContainingElementProjection() : MaterialMappingAlgorithm()
{ }

void
MMAContainingElementProjection :: __init(Domain *dold, IntArray &type, const FloatArray &coords, Set &elemSet, TimeStep *tStep, bool iCohesiveZoneGP)
{
    SpatialLocalizer *sl = dold->giveSpatialLocalizer();
    FloatArray jGpCoords;
    double minDist = 1.e6;
    Element *srcElem;

    if ( ( srcElem = sl->giveElementContainingPoint(coords, elemSet) ) ) {
        this->source = nullptr;
        for ( auto &jGp: *srcElem->giveDefaultIntegrationRulePtr() ) {
            if ( srcElem->computeGlobalCoordinates( jGpCoords, jGp->giveNaturalCoordinates() ) ) {
                double dist = distance(coords, jGpCoords);
                if ( dist < minDist ) {
                    minDist = dist;
                    this->source = jGp;
                }
            }
        }

        if ( !source ) {
            OOFEM_ERROR("no suitable source found");
        }
    } else {
        OOFEM_ERROR("No suitable element found");
    }
}

int
MMAContainingElementProjection :: __mapVariable(FloatArray &answer, const FloatArray &coords,
                                                InternalStateType type, TimeStep *tStep)
{
    if ( source ) {
        source->giveMaterial()->giveIPValue(answer, source, type, tStep);
        return 1;
    }

    return 0;
}

int
MMAContainingElementProjection :: mapStatus(MaterialStatus &oStatus) const
{
    OOFEM_ERROR("mapStatus() is not implemented yet.")

    return 0;
}
} // end namespace oofem
