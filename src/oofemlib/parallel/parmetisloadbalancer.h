// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef parmetisloadbalancer_h
#define parmetisloadbalancer_h

#include "input/loadbalancer.h"
#include "math/intarray.h"

#include <parmetis.h>
#include <vector>

#define _IFT_ParmetisLoadBalancer_Name "parmetis"

namespace oofem {
/**
 * End-of-data marker, used to identify end of data stream received.
 * The value should not conflict with any globnum id.
 */
 #define PARMETISLB_END_DATA -1
 #define SHARED_DOFMAN_PARTITIONS_TAG 9998

/**
 * ParMetis load balancer.
 */
class OOFEM_EXPORT ParmetisLoadBalancer : public LoadBalancer
{
protected:
    /// Element numbering maps.
    IntArray gToLMap, lToGMap;
    idx_t *elmdist;
    int myGlobNumOffset;
    /// Partition weights (user input).
    real_t *tpwgts;
    /// Array of DofManMode(s).
    IntArray dofManState;
    /// Array of dof man partitions.
    std :: vector< IntArray >dofManPartitions;
    /// Partition vector of the locally-stored elements.
    IntArray elementPart;

public:
    ParmetisLoadBalancer(Domain * d);
    virtual ~ParmetisLoadBalancer();

    void calculateLoadTransfer() override;

 #if 1
    DofManMode giveDofManState(int idofman) override;
    IntArray *giveDofManPartitions(int idofman) override;
    int giveElementPartition(int ielem) override;
 #endif
protected:
    void handleMasterSlaveDofManLinks();

    void initGlobalParmetisElementNumbering();
    int  giveLocalElementNumber(int globnum) { return gToLMap.at(globnum - myGlobNumOffset); }
    int  giveGlobalElementNumber(int locnum) { return lToGMap.at(locnum); }

    /**
     * Label local partition nodes (the nodes that are local or shared).
     * Labeling consist of assigning corresponding id that characterize the
     * status of local dof manager after balancing the load. Labeling determines
     * which of local nodes remain local, or became local on other partition,
     * or became shared, etc.
     */
    void labelDofManagers();
    int  determineDofManState(int idofman, int myrank, int npart, IntArray *dofManPartitions);

    int packSharedDmanPartitions(ProcessCommunicator &pc);
    int unpackSharedDmanPartitions(ProcessCommunicator &pc);
    void addSharedDofmanPartitions(int _locnum, IntArray _partitions);

    const char *giveClassName() const override { return "ParmetisLoadBalancer"; }
};
} // end namespace oofem

#endif // parmetisloadbalancer_h
