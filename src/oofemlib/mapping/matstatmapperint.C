// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mapping/matstatmapperint.h"

#include "error/error.h"
#include "material/materialmappingalgorithm.h"
#include "mapping/mmaclosestiptransfer.h"
#include "material/matstatus.h"

namespace oofem {
MaterialStatusMapperInterface :: MaterialStatusMapperInterface() :
    mpMaterialMapper(std::make_unique<MMAClosestIPTransfer>())
{}

int MaterialStatusMapperInterface :: MSMI_map(const GaussPoint &iGP, const Domain &iOldDom, Set &sourceSet, const TimeStep &iTStep, MaterialStatus &oStatus)
{
    // Mapping of "regular" Gauss points
    int result = 1;

    Domain *dold = const_cast< Domain * >(& iOldDom);
    GaussPoint *gp = const_cast< GaussPoint * >(& iGP);
    TimeStep *tStep = const_cast< TimeStep * >(& iTStep);
    IntArray type;

    mpMaterialMapper->init(dold, type, gp, sourceSet, tStep);

    mpMaterialMapper->mapStatus(oStatus);


    return result;
}

int MaterialStatusMapperInterface :: MSMI_map_cz(const GaussPoint &iGP, const Domain &iOldDom, Set &sourceSet, const TimeStep &iTStep, MaterialStatus &oStatus)
{
    // Mapping of cohesive zone Gauss points
    int result = 1;

    Domain *dold = const_cast< Domain * >(& iOldDom);
    GaussPoint *gp = const_cast< GaussPoint * >(& iGP);
    TimeStep *tStep = const_cast< TimeStep * >(& iTStep);
    IntArray type;
    bool gpBelongsToCohesiveZone = true;

    mpMaterialMapper->init(dold, type, gp, sourceSet, tStep, gpBelongsToCohesiveZone);

    mpMaterialMapper->mapStatus(oStatus);

    return result;
}


int MaterialStatusMapperInterface :: MSMI_update(const GaussPoint &iGP, const TimeStep &iTStep)
{
    int result = 1;



    return result;
}

int MaterialStatusMapperInterface :: MSMI_finish(const TimeStep &iTStep)
{
    int result = 1;



    return result;
}
} /* namespace oofem */
