// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef emptycs_h
#define emptycs_h

#include "cs/crosssection.h"

#define _IFT_EmptyCS_Name "emptycs"

namespace oofem {
/**
 * Empty cross section model which doesn't have any material models.
 */
class OOFEM_EXPORT EmptyCS : public CrossSection
{
public:
    /**
     * Constructor. Creates cross section with number n belonging to domain d.
     * @param n Cross section number.
     * @param d Domain for cross section.
     */
    EmptyCS(int n, Domain * d);

    Material *giveMaterial(IntegrationPoint *ip) const override;
    int packUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *ip) override { return 1; }
    int unpackAndUpdateUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *ip) override { return 1; }
    int estimatePackSize(DataStream &buff, GaussPoint *ip) override { return 0; }

    const char *giveClassName() const override { return "EmptyCS"; }
    const char *giveInputRecordName() const override { return _IFT_EmptyCS_Name; }
};
} // end namespace oofem
#endif // emptycs_h
