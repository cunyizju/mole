// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/domain.h"
#include "material/nonlocalbarrier.h"
#include "Damage/graddamagematerialextensioninterface.h"
#include "input/inputrecord.h"
#include "math/floatmatrix.h"

#include <list>


namespace oofem {
// constructor
GradientDamageMaterialExtensionInterface :: GradientDamageMaterialExtensionInterface(Domain *d)  : 
    Interface(),
    dom(d)
{
}

void
GradientDamageMaterialExtensionInterface :: giveGradientDamageStiffnessMatrix_dd_NN(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep)
{
    answer.clear();
}

void
GradientDamageMaterialExtensionInterface :: giveGradientDamageStiffnessMatrix_dd_BN(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep)
{
    answer.clear();
}
  

void
GradientDamageMaterialExtensionInterface :: initializeFrom(InputRecord &ir)
{
    // read the characteristic length
    IR_GIVE_FIELD(ir, internalLength, _IFT_GradientDamageMaterialExtensionInterface_l);
}

GradientDamageMaterialStatusExtensionInterface :: GradientDamageMaterialStatusExtensionInterface() : Interface()
{
}


void
GradientDamageMaterialStatusExtensionInterface :: initTempStatus()
{
    tempLocalDamageDrivingVariable = localDamageDrivingVariable;
    tempNonlocalDamageDrivingVariable = nonlocalDamageDrivingVariable;
    tempNonlocalDamageDrivingVariableGrad = nonlocalDamageDrivingVariableGrad;
}


void
GradientDamageMaterialStatusExtensionInterface :: updateYourself(TimeStep *tStep)
{
    localDamageDrivingVariable = tempLocalDamageDrivingVariable;
    nonlocalDamageDrivingVariable = tempNonlocalDamageDrivingVariable;
    nonlocalDamageDrivingVariableGrad = tempNonlocalDamageDrivingVariableGrad;
}
} // end namespace oofem
