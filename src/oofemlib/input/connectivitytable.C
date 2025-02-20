// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/connectivitytable.h"
#include "input/domain.h"
#include "input/element.h"
#include "dofman/dofmanager.h"
#include "math/intarray.h"

namespace oofem {

  ConnectivityTable::ConnectivityTable(Domain * d) : domain(d), nodalConnectivity(), nodalConnectivityFlag(0)
{
  #ifdef _OPENMP
    omp_init_lock(&initLock);
#endif
}

void
ConnectivityTable :: reset()
{
    nodalConnectivityFlag = 0;
}

void
ConnectivityTable :: instanciateConnectivityTable()
//
// assembles table of nodal connectivities
//
{
    int ndofMan = domain->giveNumberOfDofManagers();
    int nelems = domain->giveNumberOfElements();
    IntArray dofManConnectivity(ndofMan);

    if ( nodalConnectivityFlag ) {
        return;                     // already initialized
    }
#ifdef _OPENMP
    omp_set_lock(&initLock); // if not initialized yet; one thread can proceed with init; others have to wait until init completed
    if ( this->nodalConnectivityFlag ) {
        omp_unset_lock(&initLock); 
        return;
    }
#endif   
//    OOFEM_LOG_INFO("ConnectivityTable: initializing\n");

    for ( auto &elem : domain->giveElements() ) {
        int nnodes = elem->giveNumberOfDofManagers();
        for ( int j = 1; j <= nnodes; j++ ) {
            int jnode = elem->giveDofManager(j)->giveNumber();
            dofManConnectivity.at(jnode)++;
        }
    }

    // allocate Nodal connectivity table for domain
    nodalConnectivity.resize(ndofMan);
    for ( int i = 0; i < ndofMan; i++ ) {
        nodalConnectivity[i].resize( dofManConnectivity[i] );
    }

    // build Nodal connectivity table for domain
    dofManConnectivity.zero();

    for ( int i = 1; i <= nelems; i++ ) {
        Element *ielem = domain->giveElement(i);
        int nnodes = ielem->giveNumberOfDofManagers();
        for ( int j = 1; j <= nnodes; j++ ) {
            int jnode = ielem->giveDofManager(j)->giveNumber();
            nodalConnectivity[jnode-1].at( ++dofManConnectivity.at(jnode) ) = i;
        }
    }

    nodalConnectivityFlag = 1;
    
 #ifdef _OPENMP
    omp_unset_lock(&initLock);
#endif
 
}

const IntArray *
ConnectivityTable :: giveDofManConnectivityArray(int dofman)
{
    if ( nodalConnectivityFlag == 0 ) {
        this->instanciateConnectivityTable();
    }

    return &this->nodalConnectivity[dofman-1];
}


void
ConnectivityTable :: giveElementNeighbourList(IntArray &answer, IntArray &elemList)
{
    if ( nodalConnectivityFlag == 0 ) {
        this->instanciateConnectivityTable();
    }

    answer.resize(0);

    for ( auto &el_num : elemList ) {
        Element *ielem = domain->giveElement( el_num );
        int nnode = ielem->giveNumberOfDofManagers();
        for ( int j = 1; j <= nnode; j++ ) {
            int jnode = ielem->giveDofManager(j)->giveNumber();
            for ( auto &val : this->nodalConnectivity[jnode-1] ) {
                answer.insertSortedOnce( val );
            }
        }
    }
}


void
ConnectivityTable :: giveNodeNeighbourList(IntArray &answer, IntArray &nodeList)
{
    int nnodes = nodeList.giveSize();
    if ( nodalConnectivityFlag == 0 ) {
        this->instanciateConnectivityTable();
    }

    answer.resize(0);

    for ( int i = 1; i <= nnodes; i++ ) {
        int inode = nodeList.at(i);
        for ( auto &val : this->nodalConnectivity[inode-1] ) {
            answer.insertSortedOnce( val );
        }
    }
}
} // end namespace oofem
