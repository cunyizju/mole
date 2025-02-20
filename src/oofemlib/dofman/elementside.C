// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dofman/elementside.h"
#include "dofman/dof.h"
#include "math/floatmatrix.h"
#include "math/intarray.h"
#include "utility/verbose.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_DofManager(ElementSide);

ElementSide :: ElementSide(int n, Domain *aDomain) :
    DofManager(n, aDomain)
{ }


ElementSide :: ~ElementSide()
{ }


void ElementSide :: initializeFrom(InputRecord &ir)
// Gets from the source line from the data file all the data of the receiver.
{
#  ifdef VERBOSE
    // VERBOSE_PRINT1("Instanciating node ",number)
#  endif

    DofManager :: initializeFrom(ir);
}


void ElementSide :: printYourself()
// Prints the receiver on screen.
{
    printf("Element side %d \n", number);
    for ( Dof *dof: *this ) {
        dof->printYourself();
    }

    loadArray.printYourself();
    printf("\n");
}


void ElementSide :: computeTransformation(FloatMatrix &answer, const IntArray *map)
{
    //

    // computes transformation of receiver from global cs to nodal (user-defined) cs.
    // Note: implementation rely on D_u, D_v and D_w (R_u, R_v, R_w) order in cltypes.h
    // file. Do not change their order and do not insert any values between these values.
    //
    //

    int size;

    if ( map == NULL ) {
        size = this->giveNumberOfDofs();
    } else {
        size = map->giveSize();
    }

    // response for all local dofs is computed

    answer.resize(size, size);
    answer.zero();
    answer.beUnitMatrix();
}
} // end namespace oofem
