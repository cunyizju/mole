// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "particle.h"
#include "engng/classfactory.h"
#include "error/error.h"
#include "math/floatmatrix.h"

namespace oofem {
REGISTER_DofManager(Particle);


Particle :: Particle(int n, Domain *aDomain) : Node(n, aDomain)
{ }


void
Particle :: initializeFrom(InputRecord &ir)
{
    Node :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, radius, _IFT_Particle_rad);
    if ( radius < 0.0 ) {
        throw ValueInputException(ir, _IFT_Particle_rad, "must be positive");
    }
}
} // namespace oofem
