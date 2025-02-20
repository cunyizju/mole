// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef feticommunicator_h
#define feticommunicator_h

#include "sm/FETISolver/fetiboundarydofman.h"
#include "parallel/communicator.h"
#include "error/error.h"

#include <vector>

using namespace std;

namespace oofem {
class FETISolver;

/**
 * Class representing communicator for FETI solver.
 * It is attribute of FETI solver numerical method class running on master partition (rank equal to 0).
 * This Communicator provides necessary services for communication with
 * associated slave partitions. It manages several domain communicators, each for communication with
 * particular partition.
 */
class FETICommunicator : public Communicator
{
public:
    /// Enumeration used to define necessary communication tags, used to identify different messages send/received.
    enum { FETICommunicatorZeroTag, NumberOfBoundaryDofManagersMsg, BoundaryDofManagersRecMsg };

protected:
    /// Number of equations at master level (determined form boundary nodes).
    int numberOfEquations;
    /// List of boundary dof managers records.
    vector< FETIBoundaryDofManager >boundaryDofManList;
    /**
     * Master communication map. Not stored in corresponding domain comm, but required in order to
     * allow direct (memory) mapping instead of communication.
     */
    IntArray masterCommMap;

public:
    /**
     * Creates new communicator.
     * @param emodel Engineering model for communication.
     * @param b Communicator buffer.
     * @param rank Rank of associated partition.
     * @param size Number of collaborating processes.
     */
    FETICommunicator(EngngModel * emodel, CommunicatorBuff * b, int rank, int size);
    /// Destructor
    virtual ~FETICommunicator();

    int giveNumberOfDomainEquations() { return numberOfEquations; }

    void setUpCommunicationMaps(EngngModel *pm);
    /**
     * Returns reference to i-th boundary dof manager.
     * Available only on master partition.
     */
    FETIBoundaryDofManager *giveDofManager(int i) {
        return & boundaryDofManList [ i - 1 ];
    }
    /**
     * Returns pointer to master comm map stored in receiver.
     */
    IntArray *giveMasterCommMapPtr() { return & masterCommMap; }
};
} // end namespace oofem

#endif // feticommunicator_h
