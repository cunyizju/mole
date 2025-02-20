// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contable_h
#define contable_h

#include "oofemcfg.h"

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace oofem {
class IntArray;
class Domain;

/**
 * Class representing connectivity table. Usually attribute of domain. Provides
 * selected connectivity information services for domain.
 *
 * Its tasks are
 * - Creating connectivity table - method InstanciateYourself.
 * - Returning number of Elements belonging to node.
 * - Returning j-th element belonging to node i.
 */
class OOFEM_EXPORT ConnectivityTable
{
private:
    /// Pointer to domain to which receiver belongs to.
    Domain *domain;

    /// Nodal connectivity table for domain.
    std::vector< IntArray > nodalConnectivity;
    /// Flag indicating assembled connectivity table for domain.
    int nodalConnectivityFlag;
#ifdef _OPENMP
    omp_lock_t initLock;
#endif
public:
    /**
     * Constructor. Creates new Connectivity table belonging to given domain.
     */
      ConnectivityTable(Domain * d) ;
    /// Destructor
    ~ConnectivityTable() { }
    /// reset receiver to an initial state (will force table update, when needed next time)
    void reset();

    void setDomain(Domain *ipDomain) { domain = ipDomain; }

    /**
     * Builds connectivity table. This table contains for each dofManager the list of
     * elements sharing it.
     */
    void instanciateConnectivityTable();
    /**
     * @param dofman DofManger number.
     * @return Connectivity array for dofman.
     */
    const IntArray *giveDofManConnectivityArray(int dofman);
    /**
     * Returns list of neighboring elements to given elements (they are included too).
     * Neighbor is defined as element sharing the given element node.
     * @param answer List of neighbors + given elements, every element contained only once.
     * @param elemList List of elements, which neighborhood is searched.
     */
    void giveElementNeighbourList(IntArray &answer, IntArray &elemList);
    /**
     * Returns list of elements sharing given nodes.
     * @param answer List of elements, every element contained only once.
     * @param nodeList List of nodes, which neighborhood is searched.
     */
    void giveNodeNeighbourList(IntArray &answer, IntArray &nodeList);
};
} // end namespace oofem
#endif // conTable_h
