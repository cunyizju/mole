// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mapping/mmashapefunctprojection.h"
#include "dofman/dofmanager.h"
#include "math/gausspoint.h"
#include "input/element.h"
#include "input/domain.h"
#include "mesher/spatiallocalizer.h"
#include "solvers/timestep.h"
#include "fei/feinterpol.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "engng/classfactory.h"

#include <cstdlib>

namespace oofem {
REGISTER_MaterialMappingAlgorithm(MMAShapeFunctProjection, MMA_ShapeFunctionProjection);

MMAShapeFunctProjection :: MMAShapeFunctProjection() : MaterialMappingAlgorithm()
{
    stateCounter = 0;
    domain = NULL;
}

MMAShapeFunctProjection :: ~MMAShapeFunctProjection()
{ }

void
MMAShapeFunctProjection :: __init(Domain *dold, IntArray &varTypes, const FloatArray &coords, Set &elemSet, TimeStep *tStep, bool iCohesiveZoneGP)
//(Domain* dold, IntArray& varTypes, GaussPoint* gp, TimeStep* tStep)
{
    int nvar = varTypes.giveSize();
    // check time stemp
    if ( stateCounter == tStep->giveSolutionStateCounter() ) {
        return;
    }


    // Project Gauss point components to nodes on old mesh
    if ( (int)this->smootherList.size() != nvar ) {
        this->smootherList.clear();
        this->smootherList.reserve(nvar);
        for ( int ivar = 1; ivar <= nvar; ivar++ ) {
            this->smootherList.emplace_back( new NodalAveragingRecoveryModel(dold) );
        }
    }

    this->intVarTypes = varTypes;
    for ( int ivar = 1; ivar <= nvar; ivar++ ) {
        this->smootherList[ivar-1]->recoverValues(elemSet, ( InternalStateType ) varTypes.at(ivar), tStep);
    }

    // remember time stemp
    stateCounter = tStep->giveSolutionStateCounter();
    this->domain = dold;
}


void
MMAShapeFunctProjection :: finish(TimeStep *tStep)
{
    this->smootherList.clear();
    stateCounter = -1;
}

int
MMAShapeFunctProjection :: mapVariable(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep)
{
    Element *elem = gp->giveElement();
    int nnodes = elem->giveNumberOfDofManagers();
    std::vector< FloatArray > container;
    const FloatArray *nvec;

    int indx = this->intVarTypes.findFirstIndexOf( ( int ) type );
    if ( indx ) {
        container.reserve(nnodes);
        for ( int inode = 1; inode <= nnodes; inode++ ) {
            this->smootherList[indx-1]->giveNodalVector( nvec, elem->giveDofManager(inode)->giveNumber() );
            container.emplace_back(*nvec);
        }

        this->interpolateIntVarAt(answer, elem, gp->giveNaturalCoordinates(),
                                  container, type, tStep);
    } else {
        OOFEM_ERROR("var not initialized");
    }

    return 1;
}


int
MMAShapeFunctProjection :: __mapVariable(FloatArray &answer, const FloatArray &coords,
                                         InternalStateType type, TimeStep *tStep)
{
    FloatArray lcoords, closest;
    Element *elem = domain->giveSpatialLocalizer()->giveElementClosestToPoint(lcoords, closest, coords);
    if ( !elem ) {
        OOFEM_ERROR("no suitable source found");
    }

    int nnodes = elem->giveNumberOfDofManagers();
    std::vector< FloatArray > container;
    const FloatArray *nvec;

    int indx = this->intVarTypes.findFirstIndexOf( ( int ) type );
    if ( indx ) {
        container.reserve(nnodes);
        for ( int inode = 1; inode <= nnodes; inode++ ) {
            this->smootherList.at(indx-1)->giveNodalVector( nvec, elem->giveDofManager(inode)->giveNumber() );
            container.emplace_back(*nvec);
        }

        this->interpolateIntVarAt(answer, elem, lcoords, container, type, tStep);
    } else {
        OOFEM_ERROR("var not initialized");
    }

    return 1;
}


int
MMAShapeFunctProjection :: mapStatus(MaterialStatus &oStatus) const
{
    OOFEM_ERROR("not implemented yet.")

    return 0;
}


void
MMAShapeFunctProjection :: interpolateIntVarAt(FloatArray &answer, Element *elem, const FloatArray &lcoords,
                                               std :: vector< FloatArray > &list, InternalStateType type, TimeStep *tStep) const
{
    FloatArray n;

    elem->giveInterpolation()->evalN( n, lcoords, FEIElementGeometryWrapper(elem) );

    answer.resize(0);
    for ( int i = 0; i < n.giveSize(); ++i ) {
        answer.add(n[i], list[i]);
    }
}

} // end namespace oofem
