// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef activedof_h
#define activedof_h

#include "dofman/dof.h"

namespace oofem {
class ActiveBoundaryCondition;

/**
 * Class representing "slave" degree of freedom with an active boundary condition.
 * It is similar to SlaveDof, but its actual value is controlled by an active boundary condition.
 * The code is simple, the functions just pass on the evaluation to the corresponding active boundary condition.
 */
class OOFEM_EXPORT ActiveDof : public Dof
{
protected:
    /// Corresponding equation number (positive value) or prescribed equation number (negative value).
    int equationNumber;
    /// Boundary condition number.
    int bc;
    /// Active boundary condition number.
    ActiveBoundaryCondition *activeBC;

public:
    /**
     * Constructor. Creates slave dof with number n, belonging to aNode dof manager.
     * @param aNode Node receiver will belong to.
     * @param id DofID of slave dof.
     * @param bc Boundary condition dof belongs to.
     */
    ActiveDof(DofManager * aNode, DofIDItem id = Undef, int bc = 0);
    /// Destructor.
    virtual ~ActiveDof() { }

    void initialize(int cntOfMstrDfMngr, const IntArray &masterNodes, const IntArray *mstrDofID, const FloatArray &mstrContribution);
    int giveNumberOfPrimaryMasterDofs() override;
    bool isPrimaryDof() override;
    int giveNumberOfMasterDofs();
    void giveMasterDofManArray(IntArray &answer) override;
    void giveUnknowns(FloatArray &masterUnknowns, ValueModeType mode, TimeStep *tStep) override;
    void giveUnknowns(FloatArray &masterUnknowns, PrimaryField &field, ValueModeType mode, TimeStep *tStep) override;
    void computeDofTransformation(FloatArray &primaryMasterContribs) override;
    void giveEquationNumbers(IntArray &masterEqNumbers, const UnknownNumberingScheme &s) override;
    void giveDofIDs(IntArray &masterDofIDs) override;

    double giveUnknown(ValueModeType mode, TimeStep *tStep) override;
    double giveUnknown(PrimaryField &field, ValueModeType mode, TimeStep *tStep) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    dofType giveDofType() override { return DT_active; }
    const char *giveClassName() const override { return "ActiveDof"; }

    void updateLocalNumbering(EntityRenumberingFunctor &f) override;

    int __giveEquationNumber() const override;
    int __givePrescribedEquationNumber() override;
    int askNewEquationNumber(TimeStep *tStep) override;
    bool hasBc(TimeStep *tStep) override;
    int giveBcId() override;
    void setBcId(int bcId) override;
    double giveBcValue(ValueModeType mode, TimeStep *tStep) override;

    bool hasIcOn(ValueModeType type) override;
    InitialCondition *giveIc() override;
    bool hasIc() override;
    int giveIcId() override;

    ActiveBoundaryCondition *giveActiveBoundaryCondition();

protected:
    inline Dof *giveMasterDof(int i);
};
} // end namespace oofem
#endif // slavedof_h
