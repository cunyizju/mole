// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef rigidarmnode_h
#define rigidarmnode_h

#include "dofman/node.h"

///@name Input fields for RigidArmNode
//@{
#define _IFT_RigidArmNode_Name "rigidarmnode"
#define _IFT_RigidArmNode_master "master"
//@}

namespace oofem {
class FloatArray;
class IntArray;

/**
 * Class implementing node connected to other node (master) using rigid arm in finite element mesh.
 * The Rigid arm node supports not only slave dofs mapped to master throug rigid arm connection
 * but also some dofs can be primary dofs (specified by masterDofMask attribute)
 * The primary DOFs can have their own BCs, ICs.
 *
 * The introduction of rigid arm connected nodes allows to avoid very stiff elements used
 * for modeling the rigid-arm connection. The rigid arm node maps its dofs to master dofs
 * using simple transformations (small rotations are assumed). Therefore, the contribution
 * to rigid arm node are localized directly to master related equations.
 * The rigid arm node can not have its own boundary or initial conditions on linked DOFs,
 * they are determined completely from master dof conditions.
 *
 * Updated by bp: The local coordinate system in slave is supported in current implementation, 
 * the coordinate system in master and slave can be different. 
 * If no lcs is set, global one is assumed.the global cs applies.
 *
 * On the other hand, rigid arm node can be loaded independently of master.
 * The transformation for DOFs and load is not orthogonal - the inverse transformation can
 * not be constructed by transposition. Because of time consuming inversion, methods
 * can generally compute both transformations for dofs as well as loads.
 */
class OOFEM_EXPORT RigidArmNode : public Node
{
protected:
    ///
    IntArray masterMask;
    /// Number of master DofManager (Node)
    int masterDofMngr;
    /// Pointer to master Node
    Node *masterNode;

private:
    void allocAuxArrays();
    void deallocAuxArrays();

public:
    /**
     * Constructor. Creates a rigid-arm node with number n, belonging to aDomain.
     * @param n Node number in domain aDomain.
     * @param aDomain Domain to which node belongs.
     */
    RigidArmNode(int n, Domain * aDomain);
    /// Destructor.
    virtual ~RigidArmNode(void) { }

    void initializeFrom(InputRecord &ir) override;
    void postInitialize() override;
    void updateLocalNumbering(EntityRenumberingFunctor &f) override;
    int checkConsistency() override;
    /**
     * Compute vector of master contribution coefficients - SUMA of contributions == 1.0
     */
    void computeMasterContribution(std::map< DofIDItem, IntArray > &masterDofID, 
                                   std::map< DofIDItem, FloatArray > &masterContribution);

    const char *giveClassName() const override { return "RigidArmNode"; }
    const char *giveInputRecordName() const override { return _IFT_RigidArmNode_Name; }
    bool isDofTypeCompatible(dofType type) const override { return ( type == DT_master || type == DT_slave ); }
};
} // end namespace oofem
#endif // rigidarmnode_h
