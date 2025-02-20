// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dofman/slavenode.h"
#include "dofman/slavedof.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/entityrenumberingscheme.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_DofManager(SlaveNode);

void SlaveNode :: initializeFrom(InputRecord &ir)
{
    Node :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, masterDofManagers, _IFT_SlaveNode_masterDofManagers);
    IR_GIVE_OPTIONAL_FIELD(ir, masterWeights, _IFT_SlaveNode_weights);

    if ( masterWeights.giveSize() == 0 ) {
        masterWeights.resize( masterDofManagers.giveSize() );
        masterWeights.add( 1 / ( double ) masterDofManagers.giveSize() );
    } else if ( masterDofManagers.giveSize() != masterWeights.giveSize() ) {
        throw ValueInputException(ir, _IFT_SlaveNode_weights, "master dof managers and weights size mismatch.");
    }
}


void SlaveNode :: postInitialize()
{
    Node :: postInitialize();

    // initialize slave dofs (inside check of consistency of receiver and master dof)
    for ( Dof *dof: *this ) {
        SlaveDof *sdof = dynamic_cast< SlaveDof * >(dof);
        if ( sdof ) {
            sdof->initialize(masterDofManagers, IntArray(), masterWeights);
        }
    }
    // clean up
    this->masterWeights.clear();
}


void SlaveNode :: updateLocalNumbering(EntityRenumberingFunctor &f)
{
    for ( int i = 1; i <= masterDofManagers.giveSize(); ++i ) {
        masterDofManagers.at(i) = f(masterDofManagers.at(i), ERS_DofManager);
    }

    DofManager :: updateLocalNumbering(f);
}
} // end namespace oofem
