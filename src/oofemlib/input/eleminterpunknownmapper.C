// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/eleminterpunknownmapper.h"
#include "input/element.h"
#include "input/domain.h"
#include "engng/engngm.h"
#include "mesher/spatiallocalizer.h"
#include "dofman/node.h"
#include "dofman/dof.h"
#include "input/connectivitytable.h"
#include "input/unknownnumberingscheme.h"

namespace oofem {
EIPrimaryUnknownMapper :: EIPrimaryUnknownMapper() : PrimaryUnknownMapper()
{ }

#define OOFEM_MAPPING_CHECK_REGIONS

int
EIPrimaryUnknownMapper :: mapAndUpdate(FloatArray &answer, ValueModeType mode,
                                       Domain *oldd, Domain *newd,  TimeStep *tStep)
{
    int inode, nd_nnodes = newd->giveNumberOfDofManagers();
    int nsize = newd->giveEngngModel()->giveNumberOfDomainEquations( newd->giveNumber(), EModelDefaultEquationNumbering() );
    FloatArray unknownValues;
    IntArray dofidMask;
    IntArray reglist;
#ifdef OOFEM_MAPPING_CHECK_REGIONS
    ConnectivityTable *conTable = newd->giveConnectivityTable();
    const IntArray *nodeConnectivity;
#endif

    answer.resize(nsize);
    answer.zero();

    for ( inode = 1; inode <= nd_nnodes; inode++ ) {
        DofManager *node = newd->giveNode(inode);
        /* Process local and shared nodes only */
        if ( (node->giveParallelMode() != DofManager_local ) &&
             (node->giveParallelMode() != DofManager_shared)) {
            continue;
        }

#ifdef OOFEM_MAPPING_CHECK_REGIONS
        // build up region list for node
        nodeConnectivity = conTable->giveDofManConnectivityArray(inode);
        reglist.resize( nodeConnectivity->giveSize() );
        reglist.clear();
        for ( int indx = 1; indx <= nodeConnectivity->giveSize(); indx++ ) {
            reglist.insertSortedOnce( newd->giveElement( nodeConnectivity->at(indx) )->giveRegionNumber() );
        }

#endif
        ///@todo Shouldn't we pass a primary field or something to this function?
        if ( this->evaluateAt(unknownValues, dofidMask, mode, oldd, node->giveCoordinates(), reglist, tStep) ) {
            ///@todo This doesn't respect local coordinate systems in nodes. Supporting that would require major reworking.
            for ( int ii = 1; ii <= dofidMask.giveSize(); ii++ ) {
                // exclude slaves; they are determined from masters
                auto it = node->findDofWithDofId((DofIDItem)dofidMask.at(ii));
                if ( it != node->end() ) {
                    Dof *dof = *it;
                    if ( dof->isPrimaryDof() ) {
                        int eq = dof->giveEquationNumber(EModelDefaultEquationNumbering());
                        if (eq)
                          answer.at( eq ) += unknownValues.at(ii);
                    }
                }
            }
        } else {
            OOFEM_ERROR("evaluateAt service failed for node %d", inode);
        }
    }

    return 1;
}


int
EIPrimaryUnknownMapper :: evaluateAt(FloatArray &answer, IntArray &dofMask, ValueModeType mode,
                                     Domain *oldd, const FloatArray &coords, IntArray &regList, TimeStep *tStep)
{
    Element *oelem;
    SpatialLocalizer *sl = oldd->giveSpatialLocalizer();

    FloatArray lcoords, closest;
    if ( regList.isEmpty() ) {
        oelem = sl->giveElementClosestToPoint(lcoords, closest, coords, 0);
    } else {
        // Take the minimum of any region
        double mindist = 0.0, distance;
        oelem = nullptr;
        for ( int i = 1; i <= regList.giveSize(); ++i ) {
            Element *tmpelem = sl->giveElementClosestToPoint( lcoords, closest, coords, regList.at(i) );
            if ( tmpelem ) {
                distance = distance_square(closest, coords);
                if ( distance < mindist || i == 1 ) {
                    mindist = distance;
                    oelem = tmpelem;
                    if ( distance == 0.0 ) {
                        break;
                    }
                }
            }
        }
    }
    if ( !oelem ) {
        OOFEM_WARNING("Couldn't find any element containing point.");
        return false;
    }

    oelem->giveElementDofIDMask(dofMask);
    oelem->computeField(mode, tStep, lcoords, answer);

    return true;
}
} // end namespace oofem
