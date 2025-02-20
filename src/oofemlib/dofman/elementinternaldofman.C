// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dofman/elementinternaldofman.h"
#include "dofman/dof.h"
#include "math/intarray.h"
#include "utility/verbose.h"

namespace oofem {
ElementDofManager :: ElementDofManager(int n, Domain *aDomain, Element *elem) :
    DofManager(n, aDomain)
{
    this->element = elem;
}


ElementDofManager :: ~ElementDofManager()
{ }


void ElementDofManager :: initializeFrom(InputRecord &ir)
// Gets from the source line from the data file all the data of the receiver.
{
#  ifdef VERBOSE
    // VERBOSE_PRINT1("Instanciating node ",number)
#  endif

    DofManager :: initializeFrom(ir);
}


void ElementDofManager :: printYourself()
// Prints the receiver on screen.
{
    printf("InternalElementDofManager %d \n", number);
    for ( Dof *dof: *this ) {
        dof->printYourself();
    }

    loadArray.printYourself();
    printf("\n");
}
} // end namespace oofem
