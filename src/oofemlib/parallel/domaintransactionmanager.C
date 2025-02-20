// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "parallel/domaintransactionmanager.h"
#include "error/error.h"
#include "dofman/dofmanager.h"
#include "input/element.h"
#include "input/domain.h"

namespace oofem {
DomainTransactionManager :: DomainTransactionManager(Domain *d)
{
    this->domain = d;
}

DomainTransactionManager :: ~DomainTransactionManager()
{
    if ( !( dofmanTransactions.empty() && elementTransactions.empty() ) ) {
        OOFEM_WARNING("uncommited transactions exist");
    }
}

void
DomainTransactionManager :: initialize()
{ }

int
DomainTransactionManager :: addDofManTransaction(DomainTransactionType dtt, int label, DofManager *obj)
{
    if ( dtt == DTT_Remove ) {
        obj = NULL;
    }

    if ( dofmanTransactions.find(label) != dofmanTransactions.end() ) {
        // local enry exist
        // delete previous record
        if ( dofmanTransactions [ label ] ) {
            delete dofmanTransactions [ label ];
        }
    }

    // set new record
    dofmanTransactions [ label ] = obj;

    return 1;
}

int
DomainTransactionManager :: addElementTransaction(DomainTransactionType dtt,  int label, Element *obj)
{
    if ( dtt == DTT_Remove ) {
        obj = NULL;
    }

    if ( elementTransactions.find(label) != elementTransactions.end() ) {
        // local enry exist
        // delete previous record
        if ( elementTransactions [ label ] ) {
            delete elementTransactions [ label ];
        }
    }

    // set new record
    elementTransactions [ label ] = obj;

    return 1;
}


DofManager *DomainTransactionManager :: giveDofManager(int label)
{
    if ( dofmanTransactions.find(label) != dofmanTransactions.end() ) {
        // if modified record exist return it
        return dofmanTransactions [ label ];
    } else {
        // no modification recorded -> return NULL
        return NULL;
    }
}


Element *DomainTransactionManager :: giveElement(int label)
{
    if ( elementTransactions.find(label) != elementTransactions.end() ) {
        // if modified record exist return it
        return elementTransactions [ label ];
    } else {
        // no modification recorded -> return NULL
        return NULL;
    }
}


int
DomainTransactionManager :: commitTransactions()
{
    return domain->commitTransactions(this);
}
} // end namespace oofem
