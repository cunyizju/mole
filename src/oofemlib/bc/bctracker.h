// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef bctracker_h
#define bctracker_h

#include <list>
#include <vector>

namespace oofem {
class Domain;

/**
 * This class keeps track of applied boundary conditions on individual entities.
 * Some of the BCs can be applied via sets, in this case its BC and associated set
 * that keeps track which element or node is subjected to it.
 * In some applications, however, one needs the list of BCs applied on the component.
 * Cration and management of this relation is the task of this class.
 * Note: only element list is managed at present
 **/

class BCTracker {
 public:
  /// Helper class storing a sigle record for component (element, node, etc)
  struct Entry {
    /// bc number
    int bcNumber;
    /// boundari ID if required
    int boundaryId;

    Entry (int bc, int bid) {bcNumber=bc; boundaryId = bid;}
  };
  typedef std::list<Entry> entryListType;
  
 private:
  /// list keeping element entries
  std::vector<entryListType> elemList;
  /// Domain link
  Domain *domain;
  
 public:
  BCTracker (Domain *d);

  void initialize ();
  const entryListType& getElementRecords (int elem);
};
  
} // end namespace oofem
#endif


  
