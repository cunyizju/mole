// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef generalbc_h
#define generalbc_h

#include "input/femcmpnn.h"
#include "math/intarray.h"
#include "bc/bcvaltype.h"
#include "bc/bcgeomtype.h"
#include "bc/bctype.h"

///@name Input fields for a general boundary condition
//@{
#define _IFT_GeneralBoundaryCondition_timeFunct "loadtimefunction"
#define _IFT_GeneralBoundaryCondition_valType "valtype"
#define _IFT_GeneralBoundaryCondition_dofs "dofs"
#define _IFT_GeneralBoundaryCondition_isImposedTimeFunct "isimposedtimefunction"
#define _IFT_GeneralBoundaryCondition_set "set"
//@}

namespace oofem {

class Function;
class DofManager;

/**
 * Abstract base class for all boundary conditions of problem.
 * Boundary condition is an attribute of the domain (it belongs to).
 * General BC is also attribute of several elements, nodes, Dofs and so on,
 * which are subjected to boundary condition.
 *
 * This base class only declares itself as a base class of all boundary conditions,
 * and declares only very basic services. This base class introduces
 * 'TimeFunction' as an attribute of each boundary condition.
 * 'TimeFunction' represent time variation, its value is dependent on time step.
 * The value (or the components) of a boundary condition (load) will be
 * the product of its value by the value of
 * the associated load time function at given time step.
 * The meaning of boundary condition components is dependent on particular boundary condition type,
 * and should be defined in derived classes documentation.
 *
 * This base class introduces also two general services for requesting  boundary condition physical meaning and
 * boundary condition geometrical character (pointwise, acting on element body or edge and so on).
 *
 * Derived classes should represent the base classes for particular boundary condition type (like
 * force load, or boundary condition prescribed directly on some dof) and should declare
 * the basic common interface.
 */
class OOFEM_EXPORT GeneralBoundaryCondition : public FEMComponent
{
protected:
    /// Associated load time function.
    int timeFunction;
    /// Physical meaning of BC value.
    bcValType valType;
    /// Dofs that b.c. is applied to (relevant for Dirichlet type b.c.s).
    IntArray dofs;
    /**
     * Zero by default - the BC is than always imposed. Otherwise the number of associated
     * load time function. If the load time function returns aero value, the BC is inactive.
     */
    int isImposedTimeFunction;
    /// Set number for boundary condition to be applied to.
    int set;

public:
    /**
     * Constructor. Creates boundary condition with given number, belonging to given domain.
     * @param n Boundary condition number.
     * @param d Domain to which new object will belongs.
     */
    GeneralBoundaryCondition(int n, Domain * d);
    /// Destructor.
    virtual ~GeneralBoundaryCondition() { }

    /**
     * Gives the set number which boundary condition is applied to.
     */
    int giveSetNumber() { return set; }

    /// Gives the number of internal dof managers.
    virtual int giveNumberOfInternalDofManagers() { return 0; }
    /// Gives an internal dof manager from receiver.
    virtual DofManager *giveInternalDofManager(int i) { return nullptr; }


    /**
     * @return Associated load time function of receiver.
     */
    Function *giveTimeFunction();

    /**
     * Gets the activity time function number.
     */
    int getIsImposedTimeFunctionNumber(){ return this->isImposedTimeFunction; }


    /**
     * Sets the activity time function of receiver.
     * @param funcIndx Index of new time function.
     */
    void setIsImposedTimeFunctionNumber(int funcIndx){ this->isImposedTimeFunction = funcIndx; }

    /**
     * Returns receiver load type. It distinguish particular boundary conditions according to
     * their "physical" meaning (like StructuralTemperatureLoadLT, StructuralLoadLT).
     * Derived classes should always overload, default implementation returns UnknownLT value.
     * See cltypes.h file for details.
     */
    virtual bcValType giveBCValType() const { return this->valType; }

    /**
     * Returns nonzero if receiver representing BC is imposed at given time, otherwise returns zero.
     * @param tStep Time step representing time when receiver is tested.
     * @return True if imposed for given time, false otherwise.
     */
    virtual bool isImposed(TimeStep *tStep);

    /**
     * Array with default dofs which b.c. acts on (only Dirichlet type b.c.s are afflicted).
     * @return Array with dof IDs.
     */
    virtual const IntArray &giveDofIDs() const { return dofs; }
    /**
     * @return Type of boundary condition. It allows to distinguish BC according its
     * mathematical meaning, ie. like Dirichlet, Neumann, or Newton type.
     */
    virtual bcType giveType() const { return UnknownBT; }
    /**
     * Returns geometry character of boundary condition. For available values see cltypes.h file.
     * Derived classes should always overload, default implementation returns UnknownLoadGT value.
     */
    virtual bcGeomType giveBCGeoType() const { return UnknownBGT; }

    /**
     * Scales the receiver according to given value. Typically used in nondimensional analysis to scale down BCs and ICs.
     * @param s Scale factor.
     */
    virtual void scale(double s) { }

    /// Performs post initialization steps.
    virtual void postInitialize() { }

    // Overloaded methods:
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};
} // end namespace oofem
#endif // generalbc_h
