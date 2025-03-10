// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef slavedof_h
#define slavedof_h

#include "dofman/dof.h"

namespace oofem {
/**
 * Class representing "slave" degree of freedom. This dof is generally linked to some master dofs
 * with various weights - contributions - combination (link slave-slave is allowed).
 * The slave dof is fully dependent upon master dofs, therefore it's equation number is undefined.
 * It can have own boundary conditions - it is not yet implemented.
 */
class OOFEM_EXPORT SlaveDof : public Dof
{
protected:
    /// Count of master DofManagers.
    int countOfMasterDofs;
    /// Count of master DofManagers.
    int countOfPrimaryMasterDofs;
    /// Array of master  DofManagers.
    IntArray masterDofMans;
    /// Array of master dofIDs.
    IntArray dofIDs;
    /// Vector of master contribution coefficients.
    FloatArray masterContribution;

public:
    /**
     * Constructor. Creates slave dof with number n, belonging to aNode dof manager.
     * @param aNode Node receiver will belong to.
     * @param id DofID of slave dof.
     */
    SlaveDof(DofManager * aNode, DofIDItem id = Undef);
    /// Destructor.
    virtual ~SlaveDof(void) { }

    dofType giveDofType() override { return DT_slave; }

    void initialize(const IntArray &masterNodes, const IntArray &mstrDofID, const FloatArray &mstrContribution);
    int giveNumberOfPrimaryMasterDofs() override;
    void giveMasterDofManArray(IntArray &answer) override;
    void giveUnknowns(FloatArray &masterUnknowns, ValueModeType mode, TimeStep *tStep) override;
    void giveUnknowns(FloatArray &masterUnknowns, PrimaryField &field, ValueModeType mode, TimeStep *tStep) override;
    void computeDofTransformation(FloatArray &primaryMasterContribs) override;
    void giveEquationNumbers(IntArray &masterEqNumbers, const UnknownNumberingScheme &s) override;
    void giveDofIDs(IntArray &masterDofIDs) override;

    /**
     * Returns the value of the unknown associated with the receiver at given time step.
     * Slave simply asks vector of corresponding master dofs and own transformation
     * vector and returns result as dot product of these vectors. Standard element
     * services have to transform global unknown vector transform into their local c.s
     * before using it (when computing strain vector by @f$\epsilon = B\cdot r @f$, for example,
     * where B is element geometrical matrix). This transformation should contain also
     * nodal to global coordinate system transformation. So, this specialized
     * standard method for unknown query returns the corresponding master DOF value.
     * @see MasterDof::giveUnknown
     */
    double giveUnknown(ValueModeType mode, TimeStep *tStep) override;
    double giveUnknown(PrimaryField &field, ValueModeType mode, TimeStep *tStep) override;

    /**
     * Returns equation number corresponding to receiver.
     * Rigid Arm Slave have equation number undefined.
     * Usually single dof in node connected using rigid arm is
     * contributing to several master dofs (displacement to displacement and rotations in master).
     * @return Prints error message and exits.
     */
    int __giveEquationNumber() const override;
    /**
     * Returns equation number corresponding to receiver.
     * Rigid Arm Slave have equation number undefined.
     * Usually single dof in node connected using rigid arm is
     * contributing to several master dofs (displacement to displacement and rotations in master).
     * @return Prints error message and exits.
     */
    int __givePrescribedEquationNumber() override;
    /**
     * Asks new equation number. Empty function (master is assumed to receive same message).
     */
    int askNewEquationNumber(TimeStep *tStep) override { return 1; }

    /**
     * Returns boundary condition of dof if it is prescribed.
     * HangingDof can not be subjected to BC, it is only mapping to master
     * @return NULL if no BC applied, otherwise pointer to corresponding BC.
     */
    bool hasBc(TimeStep *tStep) override { return false; }

    /**
     * Returns initial condition of dof if it is prescribed.
     * HangingDof can not be subjected to IC, it is only mapping to master
     * @see MasterDof::hasIc
     */
    bool hasIc() override { return false; }

    /**
     * RigidArmSlaveDof can not be subjected to IC - it is only mapping to master.
     * @see MasterDof::hasIc
     */
    bool hasIcOn(ValueModeType) override { return false; }

    int giveBcId() override { return 0; }
    int giveIcId() override { return 0; }

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    const char *giveClassName() const override { return "SlaveDof"; }

    void updateLocalNumbering(EntityRenumberingFunctor &f) override;

protected:
    inline Dof *giveMasterDof(int i);
};
} // end namespace oofem
#endif // slavedof_h
