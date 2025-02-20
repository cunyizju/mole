// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef hangingnode_h
#define hangingnode_h

#include "dofman/node.h"

///@name Input fields for HangingNode
//@{
#define _IFT_HangingNode_Name "hangingnode"
#define _IFT_HangingNode_masterElement "masterelement"
#define _IFT_HangingNode_masterRegion "masterregion"
//@}

namespace oofem {
/**
 * Class implementing hanging node connected to other nodes (masters) using interpolation.
 * Hanging node possess no degrees of freedom - all values are interpolated from corresponding master dofs.
 *
 * The introduction of hanging nodes allows, for example, to include reinforcing bar elements inside
 * arbitrary FE-mesh of concrete specimen or facilitates the local refinement of FE-mesh.
 *
 * The contributions of hanging node are localized directly to master related equations.
 * The hanging node can not have its own boundary or initial conditions,
 * they are determined completely from master dof conditions.
 * The local coordinate system in slave is not supported in current implementation, the global cs applies.
 * On the other hand, hanging node can be loaded independently of master.
 *
 * If no master element number is supplied (or negative) then it will locate it using the global coordinates.
 * and if no master region number is supplied (or zero), it will look for elements in all regions.
 */
class OOFEM_EXPORT HangingNode : public Node
{
protected:
    /// Number of the master element.
    int masterElement;
    /// Region of the master element (used for automatic detection).
    int masterRegion;

public:
    /**
     * Constructor. Creates a hanging node with number n, belonging to aDomain.
     * @param n Node number in domain aDomain.
     * @param aDomain Domain to which node belongs.
     */
    HangingNode(int n, Domain * aDomain);
    /// Destructor.
    virtual ~HangingNode(void) { }

    void initializeFrom(InputRecord &ir) override;
    void postInitialize() override;
    int checkConsistency() override;
    bool isDofTypeCompatible(dofType type) const override { return ( type == DT_master || type == DT_slave ); }

    const char *giveClassName() const override { return "HangingNode"; }
    const char *giveInputRecordName() const override { return _IFT_HangingNode_Name; }
};
} // end namespace oofem
#endif // hangingnode_h
