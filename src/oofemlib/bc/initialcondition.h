// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef initial_h
#define initial_h

#include "bc/generalboundarycondition.h"
#include "utility/dictionary.h"
#include "bc/bcvaltype.h"
#include "input/valuemodetype.h"

///@name Input fields for initial condition
//@{
#define _IFT_InitialCondition_Name "initialcondition"
#define _IFT_InitialCondition_conditions "conditions"
#define _IFT_InitialCondition_valType "valtype"
#define _IFT_InitialCondition_set "set"
#define _IFT_InitialCondition_dofs "dofs"
//@}

namespace oofem {
class IntArray;

/**
 * Class implementing general initial condition. Initial condition is usually attribute of
 * one or more degrees of freedom (DOFs).
 *
 * The inherited attribute 'componentArray' is not used. It is replaced with
 * the more adequate dictionary 'initialValueDictionary', which entries are
 * referenced by names rather than by indices.
 *
 * One particular DOF (with its physical meaning - for example displacement)
 * can have associated only single initial condition.
 * Initial condition therefore must represent several initial conditions for particular DOF
 * (for example velocity and acceleration of unknown can be prescribed using single
 * initial condition instance). These multiple entries are distinguished by their ValueModeType value.
 * The ValueModeType value is also used as key in initialValueDictionary.
 *
 * Initial conditions apply and should be taken into account
 * only in one particular time step, which number is determined from engineering model
 * giveNumberOfTimeStepWhenIcApply service.
 * If in this time step both boundary condition on unknown and also initial condition
 * on value of this unknown (TotalMode ValueModeType) are prescribed, then
 * always value reported by boundary condition is taken into account.
 * @see EngngModel::giveNumberOfTimeStepWhenIcApply
 */
class OOFEM_EXPORT InitialCondition : public FEMComponent
{
private:
    /// Dictionary of initial values.
    Dictionary initialValueDictionary;
    /// Physical meaning of bc value.
    bcValType valType;
    /// Set number
    int set;
    /// List of dof ids that IC is applied to
    IntArray dofIDs;

public:
    /**
     * Creates initial condition with given number, belonging to given domain.
     * @param i Initial condition number.
     * @param d Domain to which new object will belongs.
     */
    InitialCondition(int i, Domain * d) : FEMComponent(i, d), initialValueDictionary() { }
    /// Destructor.
    virtual ~InitialCondition() { }

    /**
     * Returns value of initial condition for given unknown mode (determines whether total or velocity or acceleration
     * mode of unknown is requested).
     * @param mode Characteristic mode of unknown, characteristic type depends on DOF (represent physical meaning).
     * @return Value of initial condition for given mode.
     */
    double give(ValueModeType mode);
    /**
     * Returns receiver load type. It distinguish particular boundary conditions according to
     * their "physical" meaning (like StructuralTemperatureLoadLT, StructuralLoadLT).
     * Derived classes should always overload, default implementation returns UnknownLT value.
     * See cltypes.h file for details.
     * @return Value type.
     */
    bcValType giveICValType() const { return this->valType; }
    /**
     * Tests if receiver has initial condition for specific unknown-mode.
     * @param u ValueModeType value of unknown.
     * @return Nonzero if given mode has initial condition, zero otherwise.
     */
    int hasConditionOn(int u);
    /**
     * Tests if receiver has initial condition for specific unknown-mode.
     * @param type ValueModeType of unknown.
     * @return Nonzero if given mode has initial condition, zero otherwise.
     */
    int hasConditionOn(ValueModeType type);
    /**
     * Scales the receiver value (determined by ValueModeType) by given value.
     * Typically used in nondimensional analysis to scale down BCs and ICs.
     * @param type ValueModeType of unknown.
     * @param s Scaling factor.
     */
    virtual void scale(ValueModeType type, double s);

    /**
     * Gives the set number which initial condition is applied to.
     */
    int giveSetNumber() { return set; }

    /**
     * Gives the set number which initial condition is applied to.
     */
    const IntArray &giveDofIDs() { return dofIDs; }

    void printYourself() override;

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "InitialCondition"; }
    const char *giveInputRecordName() const override { return _IFT_InitialCondition_Name; }
};
} // end namespace oofem
#endif // initial_h
