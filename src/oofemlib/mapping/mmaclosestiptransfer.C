// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mapping/mmaclosestiptransfer.h"
#include "mesher/spatiallocalizer.h"
#include "input/domain.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "mapping/matstatmapperint.h"
#include "engng/classfactory.h"
#include "xfem/xfemelementinterface.h"

namespace oofem {
REGISTER_MaterialMappingAlgorithm(MMAClosestIPTransfer, MMA_ClosestPoint);

MMAClosestIPTransfer :: MMAClosestIPTransfer() : MaterialMappingAlgorithm()
{ }

void
MMAClosestIPTransfer :: __init(Domain *dold, IntArray &type, const FloatArray &coords, Set &elemSet, TimeStep *tStep, bool iCohesiveZoneGP)
{
    SpatialLocalizer *sl = dold->giveSpatialLocalizer();
    this->source = sl->giveClosestIP(coords, elemSet, iCohesiveZoneGP);

    if ( !source ) {
        OOFEM_ERROR("no suitable source found");
    }

    mpMaterialStatus = dynamic_cast<MaterialStatus*>(source->giveMaterialStatus());
    if( mpMaterialStatus == NULL ) {
        OOFEM_ERROR("Could not find material status.");
    }
}

int
MMAClosestIPTransfer :: __mapVariable(FloatArray &answer, const FloatArray &coords,
                                      InternalStateType type, TimeStep *tStep)
{
    if ( source ) {
        source->giveMaterial()->giveIPValue(answer, source, type, tStep);
        return 1;
    }

    return 0;
}

int
MMAClosestIPTransfer :: mapStatus(MaterialStatus &oStatus) const
{
    if ( mpMaterialStatus != NULL ) {
        MaterialStatusMapperInterface *interface = dynamic_cast< MaterialStatusMapperInterface * >(&oStatus);
        interface->copyStateVariables(* mpMaterialStatus);

        return 1;
    } else {
        OOFEM_ERROR("source not set.");
    }

    return 0;
}
} // end namespace oofem
