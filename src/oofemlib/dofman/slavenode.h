// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef slavenode_h
#define slavenode_h

#include "dofman/node.h"

///@name Input fields for SlaveNode
//@{
#define _IFT_SlaveNode_Name "slavenode"
#define _IFT_SlaveNode_masterDofManagers "masterdofman"
#define _IFT_SlaveNode_weights "weights"
//@}

namespace oofem {
/**
 * Class implementing slave node connected to other nodes (masters) using predetermined weights.
 * Hanging node possess no degrees of freedom - all values are interpolated from corresponding master dofs.
 *
 * The contributions of hanging node are localized directly to master related equations.
 * The node can not have its own boundary or initial conditions,
 * they are determined completely from master dof conditions except for dofs of master type.
 * @see{HangingNode}
 */
class OOFEM_EXPORT SlaveNode : public Node
{
protected:
    /// Master nodes for all dofs.
    IntArray masterDofManagers;
    /// Common dof weights for each master node.
    FloatArray masterWeights;

public:
    /**
     * Constructor. Creates a hanging node with number n, belonging to aDomain.
     * @param n Node number in domain aDomain.
     * @param aDomain Domain to which node belongs.
     */
    SlaveNode(int n, Domain * aDomain) : Node(n, aDomain) { }
    /// Destructor.
    virtual ~SlaveNode(void) { }

    void initializeFrom(InputRecord &ir) override;
    void postInitialize() override;
    bool isDofTypeCompatible(dofType type) const override { return ( type == DT_master || type == DT_slave ); }
    void updateLocalNumbering(EntityRenumberingFunctor &f) override;

    const char *giveClassName() const override { return "SlaveNode"; }
    const char *giveInputRecordName() const override { return _IFT_SlaveNode_Name; }
};
} // end namespace oofem
#endif // slavenode_h
