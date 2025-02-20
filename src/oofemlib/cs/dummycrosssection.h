// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dummycrosssection_h
#define dummycrosssection_h

#include "cs/crosssection.h"
#include "material/materialmode.h"
#include "material/matresponsemode.h"
#include "material/material.h"
#include "input/internalstatetype.h"
#include "input/internalstatevaluetype.h"
#include "utility/dictionary.h"
#include "input/crosssectextension.h"
#include "math/gausspoint.h"

///@name Input fields for CrossSection
//@{
#define _IFT_DummyCrossSection_Name "dummycs"
#define _IFT_DummyCrossSection_material "mat"
//@}

namespace oofem {

/**
 * Class representing dummy cross section, that forwards all requests to material model.
 *
 */
class OOFEM_EXPORT DummyCrossSection : public CrossSection
{
protected:
    int matNumber = 0;
public:
    DummyCrossSection(int n, Domain *d);
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool isCharacteristicMtrxSymmetric(MatResponseMode rMode) const override;
    int packUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *ip) override;
    int unpackAndUpdateUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *ip) override;
    int estimatePackSize(DataStream &buff, GaussPoint *ip) override;
    Material *giveMaterial(IntegrationPoint *ip) const override;
    const char *giveClassName() const override { return "DummyCrossSection"; }
    const char *giveInputRecordName() const override { return _IFT_DummyCrossSection_Name; }

};
} // end namespace oofem
#endif // dummycrosssection_h
