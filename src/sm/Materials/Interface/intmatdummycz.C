// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "intmatdummycz.h"
#include "engng/classfactory.h"
#include "math/gausspoint.h"

namespace oofem {
REGISTER_Material(IntMatDummyCZ);

IntMatDummyCZ :: IntMatDummyCZ(int n, Domain *d) : StructuralInterfaceMaterial(n, d) {}

FloatArrayF<3>
IntMatDummyCZ :: giveFirstPKTraction_3d(const FloatArrayF<3> &jump, const FloatMatrixF<3,3> &F, GaussPoint *gp, TimeStep *tStep) const
{
    StructuralInterfaceMaterialStatus *status = static_cast< StructuralInterfaceMaterialStatus * >( this->giveStatus(gp) );

    status->letTempJumpBe(jump);

    return {0., 0., 0.};
}

FloatMatrixF<3,3> IntMatDummyCZ :: give3dStiffnessMatrix_dTdj(MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) const
{
    return zero<3,3>();
}

void IntMatDummyCZ :: initializeFrom(InputRecord &ir)
{
    StructuralInterfaceMaterial :: initializeFrom(ir);
}

void IntMatDummyCZ :: giveInputRecord(DynamicInputRecord &input)
{
    StructuralInterfaceMaterial :: giveInputRecord(input);
}

void IntMatDummyCZ :: printYourself()
{
    printf("I am a IntMatDummyCZ.\n");
}

} /* namespace oofem */
