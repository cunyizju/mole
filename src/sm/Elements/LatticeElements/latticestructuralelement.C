// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/LatticeElements/latticestructuralelement.h"

namespace oofem {
LatticeStructuralElement :: LatticeStructuralElement(int n, Domain *aDomain) : StructuralElement(n, aDomain)
{ }

void
LatticeStructuralElement :: initializeFrom(InputRecord &ir)
{
    StructuralElement :: initializeFrom(ir);
}

void
LatticeStructuralElement :: printOutputAt(FILE *file, TimeStep *tStep)
{
    StructuralElement :: printOutputAt(file, tStep);

    /// FIXME: This output should just be moved to the elements themselves. But, they don't exist yet? / Mikael
    FloatArray forces;
    if ( this->giveClassName() == std::string("LatticeBeam3d") ) {
        this->giveInternalForcesVector(forces, tStep, 0);
        fprintf(file, "LatticeBeam forces = %e %e %e %e %e %e.\n", forces.at(7), forces.at(8), forces.at(9), forces.at(10), forces.at(11), forces.at(12) );
    } else if ( this->giveClassName() == std::string("LatticeBeam3dBoundary") ) {
        this->giveInternalForcesVector(forces, tStep, 0);
        fprintf(file, "LatticeBeam3dBoundary forces = %e %e %e %e %e %e.\n", forces.at(7), forces.at(8), forces.at(9), forces.at(10), forces.at(11), forces.at(12) );
    }
}

} // end namespace oofem
