// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef boudary_h
#define boudary_h

#include "bc/generalboundarycondition.h"
#include "math/floatarray.h"
#include "bc/bctype.h"
#include "input/valuemodetype.h"

/**
 * @name Dirichlet boundary condition.
 *
 */
//@{
#define _IFT_BoundaryCondition_Name "boundarycondition"
#define _IFT_BoundaryCondition_PrescribedValue "prescribedvalue" ///< [rn,optional] Prescribed value of all DOFs
#define _IFT_BoundaryCondition_PrescribedValue_d "d" ///< [rn,optional] Alternative input field
#define _IFT_BoundaryCondition_values "values" ///< [ra,optional] Vector of prescribed values for each respective DOF.
//@}

namespace oofem {
class TimeStep;
class Dof;

/**
 * Class implementing Dirichlet boundary condition on DOF (primary boundary condition).
 * This boundary condition is usually attribute of one or more degrees of freedom (DOF).
 *
 * The type of unknown (physical meaning)
 * is fully determined by corresponding DOF, to which given BC is
 * associated. The previous implementation uses the 'prescribedValueDictionary' to
 * store the unknowns types, but this makes sense, when BC is associated to node,
 * but when associated to BC, the physical meaning of unknown is determined by DOF.
 *
 * Boundary condition can change its value in time using its inherited TimeFunction.
 * It can also switch itself on or off depending on nonzero value of
 * isImposedTimeFunction load time function. Please note, that previous option must be
 * supported by particular engineering model (because equation renumbering is necessary,
 * and for incremental solution schemes DOFs unknown dictionaries must be used). See
 * particular engineering model documentation for details.
 *
 * The services provided include
 * - Returning a component, i.e., the prescribed value of an unknown
 *   (displacement, velocity, temperature, etc). Determined using
 *   ValueModeType (determines whether
 *   for example total value or its change from previous step is requested) - method give.
 * - Returning a value of isImposedTimeFunction, indicating whether b.c. is imposed or not
 *   in given time (nonzero indicates imposed BC) - method isImposed.
 */
class OOFEM_EXPORT BoundaryCondition : public GeneralBoundaryCondition
{
protected:
    /// Prescribed values for each resp. dof
    FloatArray values;

public:
    /**
     * Constructor. Creates boundary condition with given number, belonging to given domain.
     * @param i Boundary condition number.
     * @param d Domain to which new object will belongs.
     */
    BoundaryCondition(int i, Domain * d) : GeneralBoundaryCondition(i, d)
    { }
    /// Destructor
    virtual ~BoundaryCondition() { }

    /**
     * Returns the value of a prescribed unknown, respecting requested mode for given time.
     * Its physical meaning is determined by corresponding DOF.
     * This function should only be used if the BC is imposed.
     * @see isImposed
     * @param dof Determines the dof subjected to receiver BC.
     * @param mode Unknown char type (if total or incremental value is returned).
     * @param tStep Time step to give value for.
     * @return Prescribed value of unknown or zero if not prescribed.
     */
    virtual double give(Dof *dof, ValueModeType mode, TimeStep *tStep);
    virtual double give(Dof *dof, ValueModeType mode, double time);

    /**
     * Set prescribed value at the input record string of receiver
     * @param s prescribed value
     * @todo This function isn't as meaningful anymore. Possibly keep it if we change it to a vector. No inheriting b.c.s can overload this in a meaningful way.
     */
    void setPrescribedValue(double s);

    // Overloaded methods:
    bcType giveType() const override { return DirichletBT; }
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    void scale(double s) override;
    const char *giveClassName() const override { return "BoundaryCondition"; }
    const char *giveInputRecordName() const override { return _IFT_BoundaryCondition_Name; }

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};
} // end namespace oofem
#endif // boudary_h
