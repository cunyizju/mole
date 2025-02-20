// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li



#include "input/domain.h"
#include "mixedpressurematerialextensioninterface.h"
#include "math/gausspoint.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "../sm/Materials/Structural/structuralmaterial.h"

#include <list>


namespace oofem {
// constructor
MixedPressureMaterialExtensionInterface :: MixedPressureMaterialExtensionInterface(Domain *d)  : Interface()
{
    dom = d;
}




void
MixedPressureMaterialExtensionInterface :: giveRealStressVector(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep)
{
    ///@todo Move this to StructuralCrossSection ?
    MaterialMode mode = gp->giveMaterialMode();
    if ( mode == _3dMat ) {
        this->giveRealStressVector_3d(answer, gp, reducedStrain, pressure, tStep);
    } else if ( mode == _PlaneStrain ) {
        this->giveRealStressVector_PlaneStrain(answer, gp, reducedStrain, pressure, tStep);
    } else {
        OOFEM_ERROR("Unknown material mode for the mixed u-p formulation");
    }
}



void
MixedPressureMaterialExtensionInterface ::  giveDeviatoricConstitutiveMatrix(FloatMatrix &answer, MatResponseMode rmode, GaussPoint *gp, TimeStep *tStep)
{
    ///@todo Move this to StructuralCrossSection ?
    MaterialMode mode = gp->giveMaterialMode();
    if ( mode == _3dMat ) {
        this->giveDeviatoric3dMaterialStiffnessMatrix(answer, rmode, gp, tStep);
    } else if ( mode == _PlaneStrain ) {
        this->giveDeviatoricPlaneStrainStiffMtrx(answer, rmode, gp, tStep);
    } else {
        OOFEM_ERROR("Unknown material mode for the mixed u-p formulation");
    }
}




void
MixedPressureMaterialExtensionInterface :: giveRealStressVector_PlaneStrain(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep)
{
    FloatArray vE, vS;
    StructuralMaterial :: giveFullSymVectorForm(vE, reducedStrain, _PlaneStrain);
    this->giveRealStressVector_3d(vS, gp, vE, pressure, tStep);
    StructuralMaterial :: giveReducedSymVectorForm(answer, vS, _PlaneStrain);
}
}
