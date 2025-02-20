// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfrontextend.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"
#include "xfem/xfemmanager.h"
#include "input/domain.h"
#include "input/connectivitytable.h"
#include "input/element.h"

namespace oofem {
REGISTER_EnrichmentFront(EnrFrontExtend)

void EnrFrontExtend :: MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan, const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo)
{
    mTipInfo = iTipInfo;
    // Extend the set of enriched nodes as follows:
    // If any node of the neighboring elements is enriched, the current node is also enriched.

    Domain &d = * ( ixFemMan.giveDomain() );

    // Loop over all nodes
    int nNodes = d.giveNumberOfDofManagers();

    std :: vector< int >newEnrNodes;
    for ( int i = 1; i <= nNodes; i++ ) {
        // Check if the node is already enriched
        bool alreadyEnr = false;
        auto resMarker = ioNodeEnrMarkerMap.find(i);
        if ( resMarker != ioNodeEnrMarkerMap.end() ) {
            alreadyEnr = true;
        }

#if defined ( ENABLE_XFEM_CPP11 )
        auto begin      = newEnrNodes.begin();
        auto end        = newEnrNodes.end();
#else
        std :: vector< int > :: const_iterator begin  = newEnrNodes.begin();
        std :: vector< int > :: const_iterator end    = newEnrNodes.end();
#endif
        if ( std :: binary_search(begin, end, i) ) {
            alreadyEnr = true;
        }


        if ( !alreadyEnr ) {
            bool goOn = true;

            // Loop over neighbors
            const IntArray &neigh = * ( d.giveConnectivityTable()->giveDofManConnectivityArray(i) );
            for ( int j = 1; j <= neigh.giveSize(); j++ ) {
                if ( !goOn ) {
                    break;
                }

                Element &el = * ( d.giveElement( neigh.at(j) ) );

                // Loop over neighbor element nodes
                for ( int k = 1; k <= el.giveNumberOfDofManagers(); k++ ) {
                    int kGlob = el.giveDofManager(k)->giveGlobalNumber();
                    auto res = iLevelSetNormalDirMap.find(kGlob);
                    if ( res != iLevelSetNormalDirMap.end() && res->second < 0.0 ) {
                        newEnrNodes.push_back(i);
                        goOn = false;
                        break;
                    }
                }
            }
        }
    }


    // Mark the new nodes to be enriched
    for ( int i = 0; i < int ( newEnrNodes.size() ); i++ ) {
        ioNodeEnrMarkerMap [ newEnrNodes [ i ] ] = NodeEnr_BULK;
    }
}

void EnrFrontExtend :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);
}
} // end namespace oofem
