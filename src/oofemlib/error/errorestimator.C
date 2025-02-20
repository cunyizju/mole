// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "error/errorestimator.h"
#include "mesher/remeshingcrit.h"
#include "input/inputrecord.h"

namespace oofem {
ErrorEstimator :: ErrorEstimator(int n, Domain *d) : FEMComponent(n, d)
{
    skippedNelems = 0;
    regionSkipMap.clear();
}

ErrorEstimator :: ~ErrorEstimator()
{
}

void
ErrorEstimator :: setDomain(Domain *d)
{
    FEMComponent :: setDomain(d);
    this->giveRemeshingCrit()->setDomain(d);
}



void
ErrorEstimator :: initializeFrom(InputRecord &ir)
{
    regionSkipMap.clear();
    IR_GIVE_OPTIONAL_FIELD(ir, regionSkipMap, _IFT_ErrorEstimator_regionskipmap);
    this->IStype = IST_StressTensor;
    int val = ( int ) this->IStype;
    IR_GIVE_OPTIONAL_FIELD(ir, val, _IFT_ErrorEstimator_IStype);
    this->IStype = ( InternalStateType ) val;
}

void ErrorEstimator :: reinitialize()
{
    this->rc->reinitialize();
}

bool ErrorEstimator :: skipRegion(int reg)
{
    if ( reg <= regionSkipMap.giveSize() ) {
        return regionSkipMap.at(reg) > 0;
    } else {
        return false;
    }
}
} // end namespace oofem
