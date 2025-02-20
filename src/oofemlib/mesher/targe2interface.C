// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mesher/targe2interface.h"
#include "error/errorestimator.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "input/element.h"
#include "mesher/remeshingcrit.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Mesher(Targe2Interface, MPT_TARGE2);

MesherInterface :: returnCode
Targe2Interface :: createMesh(TimeStep *tStep, int domainNumber, int domainSerNum, Domain **dNew)
{
    * dNew = NULL;
    if ( this->createInput(this->domain, tStep) ) {
        return MI_NEEDS_EXTERNAL_ACTION;
    } else {
        return MI_FAILED;
    }
}

int
Targe2Interface :: createInput(Domain *d, TimeStep *tStep)
{
    FILE *outputStrem;
    RemeshingCriteria *rc = d->giveErrorEstimator()->giveRemeshingCrit();

    outputStrem = fopen("targe2.bmf", "w");
    // print header for 2D

    for ( auto &ielem : domain->giveElements() ) {
        fprintf( outputStrem, "MC-T %e %e %e %e %e %e   %e %e %e\n",
                ielem->giveNode(1)->giveCoordinate(1), ielem->giveNode(1)->giveCoordinate(2),
                ielem->giveNode(2)->giveCoordinate(1), ielem->giveNode(2)->giveCoordinate(2),
                ielem->giveNode(3)->giveCoordinate(1), ielem->giveNode(3)->giveCoordinate(2),
                rc->giveRequiredDofManDensity(ielem->giveNode(1)->giveNumber(), tStep),
                rc->giveRequiredDofManDensity(ielem->giveNode(2)->giveNumber(), tStep),
                rc->giveRequiredDofManDensity(ielem->giveNode(3)->giveNumber(), tStep) );
    }

    fclose(outputStrem);

    OOFEM_LOG_INFO("Targe2 .bmf file created\n");
    return 1;
}
} // end namespace oofem
