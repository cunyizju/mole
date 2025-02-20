// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfront.h"
#include "xfem/tipinfo.h"
#include "input/domain.h"
#include "xfem/xfemmanager.h"
#include "mesher/spatiallocalizer.h"
#include "input/element.h"

namespace oofem {
void EnrichmentFront :: MarkTipElementNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan,  const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo)
{
    mTipInfo = iTipInfo;

    Domain &d = * ( ixFemMan.giveDomain() );

    Element *el = d.giveSpatialLocalizer()->giveElementContainingPoint(mTipInfo.mGlobalCoord);

    if ( el != NULL ) {
        const IntArray &elNodes = el->giveDofManArray();

        for ( int i = 1; i <= elNodes.giveSize(); i++ ) {
            if ( ioNodeEnrMarkerMap [ elNodes.at(i) ] == NodeEnr_START_TIP || ioNodeEnrMarkerMap [ elNodes.at(i) ] == NodeEnr_END_TIP ) {
                ioNodeEnrMarkerMap [ elNodes.at(i) ] = NodeEnr_START_AND_END_TIP;
            } else   {
                if ( mTipInfo.mTipIndex == 0 ) {
                    ioNodeEnrMarkerMap [ elNodes.at(i) ] = NodeEnr_START_TIP;
                }

                if ( mTipInfo.mTipIndex == 1 ) {
                    ioNodeEnrMarkerMap [ elNodes.at(i) ] = NodeEnr_END_TIP;
                }
            }
        }
    }
}

void EnrichmentFront :: computeCrackTangent(FloatArray &oTangent, FloatArray &oNormal, bool &oFlipTangent, const EfInput &iEfInput) const
{
    oTangent = iEfInput.mLocalTangDir;

    if ( oTangent.dotProduct(mTipInfo.mTangDir) < 0.0 ) {
        oTangent.times(-1.0);
        oFlipTangent = true;
    } else   {
        oFlipTangent = false;
    }

    oNormal = {
        -oTangent.at(2), oTangent.at(1)
    };
}
} // end namespace oofem
