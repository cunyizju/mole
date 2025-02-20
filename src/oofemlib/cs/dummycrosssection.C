// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "cs/dummycrosssection.h"
#include "input/dynamicinputrecord.h"
#include "material/material.h"
#include "math/gaussintegrationrule.h"
#include "engng/classfactory.h"


namespace oofem {
REGISTER_CrossSection(DummyCrossSection);

DummyCrossSection :: DummyCrossSection(int n, Domain* d) : CrossSection(n, d)
{
}

void
DummyCrossSection :: initializeFrom(InputRecord &ir)
{
    CrossSection :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, this->matNumber, _IFT_DummyCrossSection_material);
}


void
DummyCrossSection :: giveInputRecord(DynamicInputRecord &input)
{
    CrossSection :: giveInputRecord(input);

    input.setField(this->matNumber, _IFT_DummyCrossSection_material);
}


bool
DummyCrossSection :: isCharacteristicMtrxSymmetric(MatResponseMode rMode) const
{
    return this->domain->giveMaterial(this->matNumber)->isCharacteristicMtrxSymmetric(rMode);
}

Material *
DummyCrossSection :: giveMaterial(IntegrationPoint *ip) const
{
    return this->domain->giveMaterial(this->matNumber);
}
int
DummyCrossSection :: packUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *gp)
{
    return this->domain->giveMaterial(this->matNumber)->packUnknowns(buff, tStep, gp);
}

int
DummyCrossSection :: unpackAndUpdateUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *gp)
{
    return this->domain->giveMaterial(this->matNumber)->unpackAndUpdateUnknowns(buff, tStep, gp);
}

int
DummyCrossSection :: estimatePackSize(DataStream &buff, GaussPoint *gp)
{
    return this->domain->giveMaterial(this->matNumber)->estimatePackSize(buff, gp);
}
} // end namespace oofem
