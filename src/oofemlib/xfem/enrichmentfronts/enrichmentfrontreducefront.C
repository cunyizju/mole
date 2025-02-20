// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfrontreducefront.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"
#include "xfem/xfemmanager.h"
#include "input/domain.h"
#include "input/connectivitytable.h"
#include "mesher/spatiallocalizer.h"
#include "input/element.h"

namespace oofem {
REGISTER_EnrichmentFront(EnrFrontReduceFront)

void EnrFrontReduceFront :: MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan, const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo)
{
    mTipInfo = iTipInfo;

    // Remove nodes touched by the crack tip
    Domain &d = * ( ixFemMan.giveDomain() );

    Element *el = d.giveSpatialLocalizer()->giveElementContainingPoint(iTipInfo.mGlobalCoord);

    if ( el != NULL ) {
        const IntArray &elNodes = el->giveDofManArray();

        for ( int i = 1; i <= elNodes.giveSize(); i++ ) {
            ioNodeEnrMarkerMap [ elNodes.at(i) ] = NodeEnr_NONE;
        }
    }
}

void EnrFrontReduceFront :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);
}
} // end namespace oofem
