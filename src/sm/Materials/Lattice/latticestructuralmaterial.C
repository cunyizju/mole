// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Materials/Lattice/latticestructuralmaterial.h"
#include "sm/Materials/Lattice/latticematstatus.h"
#include "input/domain.h"
#include "utility/verbose.h"
#include "sm/Materials/Structural/structuralms.h"
#include "sm/Elements/structuralelement.h"
#include "sm/Elements/nlstructuralelement.h"
#include "math/gausspoint.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/floatmatrixf.h"
#include "math/floatarrayf.h"
#include "math/mathfem.h"
#include "engng/engngm.h"
#include "fields/fieldmanager.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
LatticeStructuralMaterial :: LatticeStructuralMaterial(int n, Domain *d) : StructuralMaterial(n, d) { }


bool
LatticeStructuralMaterial :: hasMaterialModeCapability(MaterialMode mode) const
//
// returns whether receiver supports given mode
//
{
    return mode == _3dLattice || mode == _2dLattice || mode == _1dLattice;
}

void
LatticeStructuralMaterial :: giveStiffnessMatrix(FloatMatrix &answer,
                                                 MatResponseMode rMode,
                                                 GaussPoint *gp, TimeStep *tStep)
//
// Returns characteristic material stiffness matrix of the receiver
//
{
    answer = this->give3dLatticeStiffnessMatrix(rMode, gp, tStep);
}


int
LatticeStructuralMaterial :: giveIPValue(FloatArray &answer,
                                         GaussPoint *gp,
                                         InternalStateType type,
                                         TimeStep *atTime)
{
    auto status = static_cast< LatticeMaterialStatus * >( this->giveStatus(gp) );

    if ( type == IST_LatticeStress ) {
        answer = status->giveLatticeStress();
        return 1;
    } else if  ( type == IST_LatticeStrain ) {
        answer = status->giveLatticeStrain();
        return 1;
    } else {
        return StructuralMaterial :: giveIPValue(answer, gp, type, atTime);
    }
}


double
LatticeStructuralMaterial :: giveLatticeStress1d(double strain, GaussPoint *gp, TimeStep *tStep)
{
    FloatArrayF< 6 >tempStrain;
    tempStrain [ 0 ] = strain;
    auto answer = giveLatticeStress3d(tempStrain, gp, tStep);
    return answer [ { 0 } ];
}

FloatArrayF< 3 >
LatticeStructuralMaterial :: giveLatticeStress2d(const FloatArrayF< 3 > &strain, GaussPoint *gp, TimeStep *tStep)
{
    auto answer = giveLatticeStress3d(assemble< 6 >(strain, { 0, 1, 5 }), gp, tStep);
    return answer [ { 0, 1, 5 } ];
}

FloatArrayF< 6 >
LatticeStructuralMaterial :: giveLatticeStress3d(const FloatArrayF< 6 > &strain, GaussPoint *gp, TimeStep *tStep)
{
    OOFEM_ERROR("3dLattice mode not supported");
}

FloatMatrixF< 1, 1 >
LatticeStructuralMaterial :: give1dLatticeStiffnessMatrix(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const
//
// return material stiffness matrix for 1dlattice
//
{
    OOFEM_ERROR("No general implementation provided");
}

FloatMatrixF< 3, 3 >
LatticeStructuralMaterial :: give2dLatticeStiffnessMatrix(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const
//
// return material stiffness matrix for 2dlattice
//
{
    OOFEM_ERROR("No general implementation provided");
}

FloatMatrixF< 6, 6 >
LatticeStructuralMaterial :: give3dLatticeStiffnessMatrix(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const
//
// return material stiffness matrix for 2dlattice
//
{
    OOFEM_ERROR("No general implementation provided");
}
} // end namespace oofem
