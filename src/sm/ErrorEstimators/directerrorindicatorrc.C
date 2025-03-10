// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "error/errorestimator.h"
#include "input/domain.h"
#include "input/element.h"
#include "dofman/dofmanager.h"
#include "engng/engngm.h"
#include "input/connectivitytable.h"
#include "math/mathfem.h"
#include "solvers/timestep.h"

#ifdef __PARALLEL_MODE
 #include "parallel/problemcomm.h"
#endif

namespace oofem {
DirectErrorIndicatorRC :: DirectErrorIndicatorRC(int n, ErrorEstimator *e) : RemeshingCriteria(n, e)
{
    stateCounter = -1;
#ifdef __PARALLEL_MODE
    dofManDensityExchangeFlag = true;
#endif
}

DirectErrorIndicatorRC :: ~DirectErrorIndicatorRC()
{
}

void
DirectErrorIndicatorRC :: giveNodeChar(int inode, TimeStep *tStep, double &indicatorVal, double &currDensity)
{
    currDensity = this->giveDofManDensity(inode);
    indicatorVal =  this->giveDofManIndicator(inode, tStep);
}


double
DirectErrorIndicatorRC :: giveDofManDensity(int num)
{
#ifdef __PARALLEL_MODE
    Domain *d = this->giveDomain();
    if ( d->giveDofManager(num)->isShared() ) {
        return this->sharedDofManDensities [ num ];
    } else {
        return this->giveLocalDofManDensity(num);
    }

#else
    return this->giveLocalDofManDensity(num);

#endif
}

double
DirectErrorIndicatorRC :: giveLocalDofManDensity(int num)
{
    int isize;
    double currDensity = 0.0;
    const IntArray *con;
    Domain *d = this->giveDomain();
    ConnectivityTable *ct = d->giveConnectivityTable();

    con = ct->giveDofManConnectivityArray(num);
    isize = con->giveSize();

    for ( int i = 1; i <= isize; i++ ) {
        // ask for indicator variable value and determine current mesh density
        Element *element = d->giveElement( con->at(i) );

        if ( i == 1 ) {
            currDensity = element->computeMeanSize();
        } else {
            currDensity =  max( currDensity, element->computeMeanSize() );
        }
    }

    return currDensity;
}


double
DirectErrorIndicatorRC :: giveDofManIndicator(int num, TimeStep *tStep)
{
#ifdef __PARALLEL_MODE
    Domain *d = this->giveDomain();
    if ( d->giveDofManager(num)->isShared() ) {
        return this->sharedDofManIndicatorVals [ num ];
    } else {
        return this->giveLocalDofManIndicator(num, tStep);
    }

#else
    return this->giveLocalDofManIndicator(num, tStep);

#endif
}

double
DirectErrorIndicatorRC :: giveLocalDofManIndicator(int inode, TimeStep *tStep)
{
    int isize;
    const IntArray *con;
    Domain *d = this->giveDomain();
    ConnectivityTable *ct = d->giveConnectivityTable();
    double indicatorVal = 0.0;

    con = ct->giveDofManConnectivityArray(inode);
    isize = con->giveSize();

    for ( int i = 1; i <= isize; i++ ) {
        // ask for indicator variable value and determine current mesh density
        Element *element = d->giveElement( con->at(i) );

        if ( i == 1 ) {
            indicatorVal = ee->giveElementError(indicatorET, element, tStep);
        } else {
            indicatorVal = max( indicatorVal, ee->giveElementError(indicatorET, element, tStep) );
        }
    }

    return indicatorVal;
}



int
DirectErrorIndicatorRC :: estimateMeshDensities(TimeStep *tStep)
{
    Domain *d = this->giveDomain();
    int inode, nnodes = d->giveNumberOfDofManagers();
    double indicatorVal, currDensity, proposedDensity;

    if ( stateCounter == tStep->giveSolutionStateCounter() ) {
        return 1;
    }

#ifdef __PARALLEL_MODE
    if ( initCommMap ) {
        communicator->setUpCommunicationMaps(d->giveEngngModel(), true, true);
        OOFEM_LOG_INFO("DirectErrorIndicatorRC :: estimateMeshDensities: initialized comm maps\n");
        initCommMap = false;
    }


    this->exchangeDofManDensities();
    this->exchangeDofManIndicatorVals(tStep);
#endif

    this->currStrategy  = NoRemeshing_RS;
    this->nodalDensities.resize(nnodes);

    for ( inode = 1; inode <= nnodes; inode++ ) {
        this->giveNodeChar(inode, tStep, indicatorVal, currDensity);

        if ( indicatorVal < minIndicatorLimit ) {
            this->nodalDensities.at(inode) = zeroIndicatorDensity; //currDensity;
        } else if ( indicatorVal >= maxIndicatorLimit ) {
            this->nodalDensities.at(inode) = proposedDensity = maxIndicatorDensity;

            if ( proposedDensity < ( currDensity * this->remeshingDensityRatioToggle ) ) {
                this->currStrategy  = RemeshingFromCurrentState_RS;
                OOFEM_LOG_DEBUG("DirectEI: remeshing required for node %d, den %e, required_dens %e\n", inode, currDensity, proposedDensity);
            }
        } else {
            // evaluate the required size
            proposedDensity = minIndicatorDensity +
                              ( indicatorVal - minIndicatorLimit ) * ( maxIndicatorDensity - minIndicatorDensity ) / ( maxIndicatorLimit - minIndicatorLimit );
            //proposedDensity = min (currDensity, proposedDensity);
            this->nodalDensities.at(inode) = proposedDensity;

            if ( proposedDensity < ( currDensity * this->remeshingDensityRatioToggle ) ) {
                OOFEM_LOG_DEBUG("DirectEI: remeshing required for node %d, den %e, required_dens %e\n", inode, currDensity, proposedDensity);
                this->currStrategy  = RemeshingFromCurrentState_RS;
            }
        }
    }

#ifdef __PARALLEL_MODE
    // exchange strategies between nodes to ensure consistency
    int myStrategy = this->currStrategy, globalStrategy;
    MPI_Allreduce(& myStrategy, & globalStrategy, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    this->currStrategy = ( RemeshingStrategy ) globalStrategy;
#endif

    // remember time stamp
    stateCounter = tStep->giveSolutionStateCounter();
    return 1;
}


double
DirectErrorIndicatorRC :: giveRequiredDofManDensity(int num, TimeStep *tStep, int relative)
{
    this->estimateMeshDensities(tStep);
    if ( relative ) {
        double currDens;
        currDens = this->giveDofManDensity(num);
        return this->nodalDensities.at(num) / currDens;
    } else {
        return this->nodalDensities.at(num);
    }
}


void
DirectErrorIndicatorRC :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, minIndicatorLimit, _IFT_DirectErrorIndicatorRC_minlim);
    IR_GIVE_FIELD(ir, maxIndicatorLimit, _IFT_DirectErrorIndicatorRC_maxlim);
    IR_GIVE_FIELD(ir, minIndicatorDensity, _IFT_DirectErrorIndicatorRC_mindens);
    IR_GIVE_FIELD(ir, maxIndicatorDensity, _IFT_DirectErrorIndicatorRC_maxdens);
    IR_GIVE_FIELD(ir, zeroIndicatorDensity, _IFT_DirectErrorIndicatorRC_defdens);

    remeshingDensityRatioToggle = 0.80;
    IR_GIVE_OPTIONAL_FIELD(ir, remeshingDensityRatioToggle, _IFT_DirectErrorIndicatorRC_remeshingdensityratio);

#ifdef __PARALLEL_MODE
    EngngModel *emodel = domain->giveEngngModel();
    commBuff = new CommunicatorBuff(emodel->giveNumberOfProcesses(), CBT_dynamic);
    communicator = new NodeCommunicator(emodel, commBuff, emodel->giveRank(),
                                        emodel->giveNumberOfProcesses());
#endif
}

RemeshingStrategy
DirectErrorIndicatorRC :: giveRemeshingStrategy(TimeStep *tStep)
{
    this->estimateMeshDensities(tStep);
    return this->currStrategy;
}

void
DirectErrorIndicatorRC :: reinitialize()
{
    stateCounter = -1;
#ifdef __PARALLEL_MODE
    dofManDensityExchangeFlag  = true;
    initCommMap = true;
#endif
}


void
DirectErrorIndicatorRC :: setDomain(Domain *d)
{
    RemeshingCriteria :: setDomain(d);
#ifdef __PARALLEL_MODE
    dofManDensityExchangeFlag = true;
    initCommMap = true;
#endif
}


#ifdef __PARALLEL_MODE
void
DirectErrorIndicatorRC :: exchangeDofManDensities()
{
    Domain *domain = this->giveDomain();

    if ( this->dofManDensityExchangeFlag ) {
        ///@todo Compute local shared dofman densities
        sharedDofManDensities.clear();
        int size = domain->giveNumberOfDofManagers();
        for ( int i = 1; i <= size; i++ ) {
            if ( domain->giveDofManager(i)->giveParallelMode() == DofManager_shared ) {
                sharedDofManDensities [ i ] = this->giveLocalDofManDensity(i);
            }
        }

        // exchange them
        communicator->packAllData(this, & DirectErrorIndicatorRC :: packSharedDofManLocalDensities);
        communicator->initExchange(999);
        communicator->unpackAllData(this, & DirectErrorIndicatorRC :: unpackSharedDofManLocalDensities);
        communicator->finishExchange();

        this->dofManDensityExchangeFlag = false;
    } // if (this->dofManDensityExchangeFlag)
}

int
DirectErrorIndicatorRC :: packSharedDofManLocalDensities(ProcessCommunicator &processComm)
{
    int result = 1;
    ProcessCommunicatorBuff *pcbuff = processComm.giveProcessCommunicatorBuff();
    const IntArray &toSendMap = processComm.giveToSendMap();

    for ( int inode : toSendMap ) {
        result &= pcbuff->write(this->sharedDofManDensities [ inode ]);
    }

    return result;
}

int
DirectErrorIndicatorRC :: unpackSharedDofManLocalDensities(ProcessCommunicator &processComm)
{
    int result = 1;
    const IntArray &toRecvMap = processComm.giveToRecvMap();
    ProcessCommunicatorBuff *pcbuff = processComm.giveProcessCommunicatorBuff();
    double value;

    for ( int inode : toRecvMap ) {
        result &= pcbuff->read(value);
        this->sharedDofManDensities [ inode ] = max(value, this->sharedDofManDensities [ inode ]);
 #ifdef __VERBOSE_PARALLEL
        OOFEM_LOG_INFO("unpackSharedDofManLocalDensities: node %d[%d], value %f\n", inode, domain->giveDofManager( inode )->giveGlobalNumber(), this->sharedDofManDensities [ inode ]);
 #endif
    }

    return result;
}




void
DirectErrorIndicatorRC :: exchangeDofManIndicatorVals(TimeStep *tStep)
{
    Domain *domain = this->giveDomain();

    ///@todo Compute local shared dofman indicator values
    sharedDofManIndicatorVals.clear();
    int size = domain->giveNumberOfDofManagers();
    for ( int i = 1; i <= size; i++ ) {
        if ( domain->giveDofManager(i)->giveParallelMode() == DofManager_shared ) {
            sharedDofManIndicatorVals [ i ] = this->giveLocalDofManIndicator(i, tStep);
        }
    }

    // exchange them
    communicator->packAllData(this, & DirectErrorIndicatorRC :: packSharedDofManLocalIndicatorVals);
    communicator->initExchange(999);
    communicator->unpackAllData(this, & DirectErrorIndicatorRC :: unpackSharedDofManLocalIndicatorVals);
    communicator->finishExchange();
}

int
DirectErrorIndicatorRC :: packSharedDofManLocalIndicatorVals(ProcessCommunicator &processComm)
{
    int result = 1;
    ProcessCommunicatorBuff *pcbuff = processComm.giveProcessCommunicatorBuff();
    const IntArray &toSendMap = processComm.giveToSendMap();

    for ( int inode : toSendMap ) {
        result &= pcbuff->write(this->sharedDofManIndicatorVals [ inode ]);
    }

    return result;
}

int
DirectErrorIndicatorRC :: unpackSharedDofManLocalIndicatorVals(ProcessCommunicator &processComm)
{
    int result = 1;
    const IntArray &toRecvMap = processComm.giveToRecvMap();
    ProcessCommunicatorBuff *pcbuff = processComm.giveProcessCommunicatorBuff();

    for ( int inode : toRecvMap ) {
        double value;
        result &= pcbuff->read(value);
        this->sharedDofManIndicatorVals [ inode ] = max(value, this->sharedDofManIndicatorVals [ inode ]);
 #ifdef __VERBOSE_PARALLEL
        OOFEM_LOG_INFO("unpackSharedDofManLocalIndicatorVals: node %d[%d], value %f\n", inode, domain->giveDofManager( inode )->giveGlobalNumber(), this->sharedDofManIndicatorVals [ inode ]);
 #endif
    }

    return result;
}


#endif
} // end namespace oofem
