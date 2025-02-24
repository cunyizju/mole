// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef problemcomm_h
#define problemcomm_h

#include "parallel/communicator.h"
#include "error/error.h"

namespace oofem {
class EngngModel;

/**
 * Class representing communicator for engng model.
 * It is assumed to be an attribute of an engineering model.
 * The communicator provides all services for communication with
 * associated remote partitions. It manages several process communicators.
 */
class OOFEM_EXPORT ProblemCommunicator : public Communicator
{
protected:
    bool initialized;

public:
    /**
     * Constructor.
     * @param emodel Associated engineering model.
     * @param b Buffer object.
     * @param rank Rank of associated partition.
     * @param size Number of collaborating processes.
     */
    ProblemCommunicator(EngngModel * emodel, CommunicatorBuff * b, int rank, int size);
    /// Destructor
    virtual ~ProblemCommunicator();

    /**
     * Service for setting up the communication patterns with other remote process.
     * Sets up the toSend and toRecv attributes in associated process communicators.
     * @param emodel Associated engineering model.
     * @param excludeSelfCommFlag If set to true, the communication map of receiver with itself will be forced to be empty, otherwise it will be assembled.
     * @param forceReinit Forces reinitilaization.
     */
    virtual void setUpCommunicationMaps(EngngModel *emodel, bool excludeSelfCommFlag, bool forceReinit = false) = 0;
    /**
     * Assigns given map to given process communicator.
     * Sorts map according to global entity (dofmanagers or element) numbers to ensure, that local and
     * corresponding remote process have the identical map with same ordering. This will ensure proper packing/unpacking
     * order. The corresponding processCommunicator buffer takes care about resizing itself accordingly to hold all
     * outgoing/incoming data using engngModel->estimateMaxPackSize service.
     * @param processComm Domain comm which send map will be set.
     * @param map Send map.
     */
    virtual int setProcessCommunicatorToSendArry(ProcessCommunicator *processComm, IntArray &map) = 0;
    /**
     * Assigns given map to given process communicator.
     * Sorts map according to global entity (dofmanagers or element) numbers to ensure, that local and
     * corresponding remote process have the identical map with same ordering. This will ensure proper packing/unpacking
     * order. The corresponding processCommunicator buffer takes care about resizing itself  accordingly to hold all
     * outgoing/incoming data using engngModel->estimateMaxPackSize service.
     * @param processComm Process comm which received map will be set.
     * @param map Received map.
     */
    virtual int setProcessCommunicatorToRecvArry(ProcessCommunicator *processComm, IntArray &map) = 0;

protected:
    /**
     * Sorts given communication map, containing local DofManager numbers according to their
     * corresponding global numbers. It could not be sorted by standard techniques, because
     * it is necessary to ask DofMAnager form domain and determine its global Number.
     * @param map Map to sort.
     * @param cmp Comparison function must return a negative value if first argument is less than the second,
     * zero if the arguments are equal, and a positive number otherwise.
     */
    void sortCommMap( IntArray &map, int ( ProblemCommunicator :: *cmp )( int, int ) );
    /// Implementation of quicksort algorithm.
    void quickSortCommMap( IntArray &map, int l, int r, int ( ProblemCommunicator :: *cmp )( int, int ) );
    /// Partitioning used in quicksort.
    int quickSortPartition( IntArray &map, int l, int r, int ( ProblemCommunicator :: *cmp )( int, int ) );

public:
    /// Global dofManager number comparison function.
    int DofManCmp(int, int);
    /// Global element comparison function.
    int ElemCmp(int, int);
};

class OOFEM_EXPORT NodeCommunicator : public ProblemCommunicator
{
public:
    NodeCommunicator(EngngModel * emodel, CommunicatorBuff * b, int rank, int size);
    virtual ~NodeCommunicator() {}

    void setUpCommunicationMaps(EngngModel *emodel, bool excludeSelfCommFlag, bool forceReinit = false) override;
    int setProcessCommunicatorToSendArry(ProcessCommunicator *processComm, IntArray &map) override;
    int setProcessCommunicatorToRecvArry(ProcessCommunicator *processComm, IntArray &map) override;
};

class OOFEM_EXPORT ElementCommunicator : public ProblemCommunicator
{
public:
    ElementCommunicator(EngngModel * emodel, CommunicatorBuff * b, int rank, int size);
    virtual ~ElementCommunicator() {}

    void setUpCommunicationMaps(EngngModel *emodel, bool excludeSelfCommFlag, bool forceReinit = false) override;
    int setProcessCommunicatorToSendArry(ProcessCommunicator *processComm, IntArray &map) override;
    int setProcessCommunicatorToRecvArry(ProcessCommunicator *processComm, IntArray &map) override;
};

} // end namespace oofem

#endif // problemcomm_h
