// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef matstatus_h
#define matstatus_h

#include "math/integrationpointstatus.h"

namespace oofem {
class GaussPoint;
class Dictionary;
class Domain;
class NonlocalMaterialStatusExtension;

/**
 * Abstract base class representing  a material status information.
 *
 * To provide opportunity for storing arbitrary material model related history variables
 * in integration points, associated material status class is introduced.
 * Each new material model class should be declared together with its associated status class
 * (derived from MaterialStatus class). This status can be seen as simple container,
 * storing necessary history variables and providing some access and modification methods.
 * Each integration point can contain material status. Material model should create
 * unique copy of its associated status in each integration point.
 * Because integration point is parameter of all messages to material model
 * class, material model therefore can easily access  all history variables it needs.
 *
 * Generally, two sets of internal history variables are defined inside material status.
 * One set should always refer to previously reached equilibrium state. The second set is used to
 * describe the state during iteration of equilibrium. After the new equilibrium is reached (on structural level) the
 * variables of first set are updated according to variables of second set.
 * On the other hand, if convergence of iteration is not obtained, the variables of second set (so called temp-variables)
 * are initialized according to non-temp variables and the iteration can begin for example with smaller load increment.
 * The temp and non-temp history variables allow simple iteration restart within one step.
 * The restarts to previous steps are supported, but context for these steps must be stored.
 *
 * The general services for status initialization and update, as well as services for
 * storing and restoring status context are declared. The implementation is left on derived
 * classes.
 *
 * The unique copy of material status class instance corresponding to material model is
 * created and associated with any integration point.
 *
 * Tasks:
 * This is abstract class - only basic functionality is supported like:
 * - storing and restoring status on tape
 * - printYourself()
 * - updating Yourself after a new equilibrium state has been reached.
 *
 * @note{Materials statuses are attributes of GaussPoints, they are stored in
 * MatStatus variable of GaussPoint class instance.}
 */
class OOFEM_EXPORT MaterialStatus : public IntegrationPointStatus
{
public:
    /**
     * Constructor.
     * @param g Associated integration point.
     */
    MaterialStatus(GaussPoint * g) : IntegrationPointStatus(g) { }
    /// Print receiver's output to given stream.
    void printOutputAt(FILE *file, TimeStep *tStep) const override { }

    /**
     * Initializes the temporary internal variables, describing the current state according to
     * previously reached equilibrium internal variables.
     */
    virtual void initTempStatus() { }
    /**
     * Update equilibrium history variables according to temp-variables.
     * Invoked, after new equilibrium state has been reached.
     */
    void updateYourself(TimeStep *) override { }
    /**
     * Returns the value of material model property stored in receiving status.
     * This is typically used when random variation of some material property is considered,
     * in this case the individual values are to be stored in status (they are no longer material constants)
     * Returns true if property is available in status,  false
     */
    virtual bool giveMaterialProperty(int propID, double &value) { return false; }
    /**
     * Allows to set the value of material model property to be stored in receiving status.
     * This is typically used when random variation of some material property is considered,
     * in this case the individual values are to be stored in status (they are no longer material constants)
     */
    virtual void setMaterialProperty(int propID, double value) { }
};
} // end namespace oofem
#endif // matstatus_h
