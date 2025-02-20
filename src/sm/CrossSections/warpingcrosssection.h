// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef warpingcrosssection_h
#define warpingcrosssection_h

#include "simplecrosssection.h"

///@name Input fields for SimpleCrossSection
//@{
#define _IFT_WarpingCrossSection_Name "warpingcs"
#define _IFT_WarpingCrossSection_WarpingNodeNumber "warpingnode"
//@}

namespace oofem {
/**
 * description of warping cross section...
 */
class OOFEM_EXPORT WarpingCrossSection : public SimpleCrossSection
{
protected:
    int WarpingNodeNumber;     // number of the 4rd node

public:
    WarpingCrossSection(int n, Domain *d) : SimpleCrossSection(n, d), WarpingNodeNumber(0) { }

    void initializeFrom(InputRecord &ir) override;

    // identification and auxiliary functions

    const char *giveClassName() const override {
        return "WarpingCrossSection";
    }

    const char *giveInputRecordName() const override {
        return _IFT_WarpingCrossSection_Name;
    }

    int giveWarpingNodeNumber() const {
        return this->WarpingNodeNumber;
    }
};
} // end namespace oofem
#endif // warpingcrosssection_h
