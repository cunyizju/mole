// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/domain.h"
#include "math/intarray.h"
#include "input/element.h"
#include "utility/set.h"
#include "bc/load.h"
#include "bc/bodyload.h"
#include "bc/boundaryload.h"
#include "bc/nodalload.h"
#include "bc/activebc.h"
#include "solvers/timestep.h"

namespace oofem {

BCTracker :: BCTracker(Domain* d) {
  this->domain = d;
}


void
BCTracker::initialize() {
  this->elemList.clear();
  this->elemList.resize(domain->giveNumberOfElements());

  int nbc = domain->giveNumberOfBoundaryConditions();
  for ( int ibc = 1; ibc <= nbc; ++ibc ) {
    GeneralBoundaryCondition *bc = domain->giveBc(ibc);
    ActiveBoundaryCondition *abc;
    Load *load;

    if ( ( abc = dynamic_cast< ActiveBoundaryCondition * >(bc) ) ) {
      continue;
    } else if ( bc->giveSetNumber() && ( load = dynamic_cast< Load * >(bc) )) {
      BodyLoad *bodyLoad;
      BoundaryLoad *bLoad;
      Set *set = domain->giveSet( bc->giveSetNumber() );

      if ( ( bodyLoad = dynamic_cast< BodyLoad * >(bc) ) ) { // Body load:
        const IntArray &elements = set->giveElementList();
        for ( int ielem = 1; ielem <= elements.giveSize(); ++ielem ) {
          Entry entry (ibc, 0);
          this->elemList[elements.at(ielem)-1].push_back(entry);
        }
      } else if ( ( bLoad = dynamic_cast< BoundaryLoad * >(bc) ) ) { // Boundary load:
        const IntArray &boundaries = set->giveBoundaryList();
        for ( int ibnd = 1; ibnd <= boundaries.giveSize() / 2; ++ibnd ) {
          int eid = boundaries.at(ibnd * 2 - 1) ;
          int bid = boundaries.at(ibnd * 2);
          Entry entry (ibc, bid);
          this->elemList[eid-1].push_back(entry);
        }

        ///@todo Should we have a seperate entry for edge loads? Just sticking to the general "boundaryload" for now.
        const IntArray &edgeBoundaries = set->giveEdgeList();
        for ( int ibnd = 1; ibnd <= edgeBoundaries.giveSize() / 2; ++ibnd ) {
          int eid = edgeBoundaries.at(ibnd * 2 - 1) ;
          int bid = edgeBoundaries.at(ibnd * 2);
          Entry entry(ibc, bid);
          this->elemList[eid-1].push_back(entry);
        }
      }
    }
  }// end loop over BCs
}
  
const BCTracker::entryListType&
BCTracker::getElementRecords (int elem) {
  if (this->elemList.empty()) {
    this->initialize();
  }
  return this->elemList.at(elem-1);
}

} // end namespace oofem
