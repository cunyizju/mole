// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "math/gausspoint.h"
#include "input/inputrecord.h"
#include "input/domain.h"
#include "material/material.h"
#include "func/function.h"
#include "material/randommaterialext.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
bool
RandomMaterialStatusExtensionInterface :: _giveProperty(int key, double &value)
{
    if ( randProperties.includes(key) ) {
        value = randProperties.at(key);
        return true;
    } else {
        return false;
    }
}

void
RandomMaterialStatusExtensionInterface :: _setProperty(int key, double value)
{
    randProperties.at(key) = value;
}


void
RandomMaterialExtensionInterface :: initializeFrom(InputRecord &ir)
{
    randVariables.clear();
    randomVariableGenerators.clear();
    IR_GIVE_OPTIONAL_FIELD(ir, randVariables, _IFT_RandomMaterialExt_randVariables);
    IR_GIVE_OPTIONAL_FIELD(ir, randomVariableGenerators, _IFT_RandomMaterialExt_randGen);

    if ( randVariables.giveSize() != randomVariableGenerators.giveSize() ) {
        throw ValueInputException(ir, _IFT_RandomMaterialExt_randVariables, "Incompatible size of randvars and randdist attrs");
    }
}


void
RandomMaterialExtensionInterface :: giveInputRecord(DynamicInputRecord &ir)
{
    ir.setField(this->randVariables, _IFT_RandomMaterialExt_randVariables);
    ir.setField(this->randomVariableGenerators, _IFT_RandomMaterialExt_randGen);
}


bool
RandomMaterialExtensionInterface :: give(int key, GaussPoint *gp, double &value) const
{
    MaterialStatus *status = static_cast< MaterialStatus * >( gp->giveMaterialStatus() );

    // check if random material status has been initialized - does not have to be if material is used as a dummy
    if  ( status->giveInterface(RandomMaterialStatusExtensionInterfaceType) == NULL ) {
        return false;
    }


    RandomMaterialStatusExtensionInterface *interface = dynamic_cast< RandomMaterialStatusExtensionInterface * >
                                                        ( gp->giveMaterialStatus()->giveInterface(RandomMaterialStatusExtensionInterfaceType) );
    return interface->_giveProperty(key, value);
}

void
RandomMaterialExtensionInterface :: _generateStatusVariables(GaussPoint *gp) const
{
    // Have to wrap it through the material to ensure that it gets an actual material status (for now at least)
    int size = randVariables.giveSize();
    double value;
    MaterialStatus *matStat = static_cast< MaterialStatus * >( gp->giveMaterialStatus() );
    RandomMaterialStatusExtensionInterface *status = static_cast< RandomMaterialStatusExtensionInterface * >
                                                     ( matStat->giveInterface(RandomMaterialStatusExtensionInterfaceType) );

    for ( int i = 1; i <= size; i++ ) {
        FloatArray globalCoordinates;
        if ( gp->giveElement()->computeGlobalCoordinates(globalCoordinates, gp->giveSubPatchCoordinates() ) ) {
            Function *f = gp->giveElement()->giveDomain()->giveFunction(randomVariableGenerators.at(i) );
            value = f->evaluate({{ "x", globalCoordinates } });
            status->_setProperty(randVariables.at(i), value);
        } else {
            OOFEM_ERROR("computeGlobalCoordinates failed");
        }
    }
}
} // end namespace oofem
