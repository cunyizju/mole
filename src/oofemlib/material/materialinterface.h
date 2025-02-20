// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef materialinterface_h
#define materialinterface_h

#include "input/femcmpnn.h"

namespace oofem {
/**
 * Abstract base class representing (moving) material interfaces.
 * Its typical use to model moving interface (such as free surface)
 * in a fixed-grid methods (as typically used in CFD).
 * The basic tasks are representation of interface and its updating.
 */
class OOFEM_EXPORT MaterialInterface : public FEMComponent
{
public:
    /**
     * Constructor. Takes two two arguments. Creates
     * MaterialInterface instance with given number and belonging to given domain.
     * @param n Component number in particular domain. For instance, can represent
     * node number in particular domain.
     * @param d Domain to which component belongs to.
     */
    MaterialInterface(int n, Domain * d) : FEMComponent(n, d) { }

    const char *giveInputRecordName() const override { return nullptr; }

    /**
     *  Initializes receiver
     */
    virtual void initialize() { }
    /**
     * Updates the position of interface according to state reached in given solution step.
     */
    virtual void updatePosition(TimeStep *tStep) = 0;
    /**
     * Updates element state after equilibrium in time step has been reached.
     * All temporary history variables,
     * which now describe equilibrium state should be copied into equilibrium ones.
     * The existing internal state is used for update.
     */
    virtual void updateYourself(TimeStep *tStep) = 0;
    /**
     * Computes critical time step induced by receiver integration algorithm
     */
    virtual double computeCriticalTimeStep(TimeStep *tStep) = 0;

    /**
     * Returns relative material contents at given point. Usually only one material is presented in given point,
     * but some smoothing may be applied close to material interface to make transition smooth
     */
    virtual void giveMaterialMixtureAt(FloatArray &answer, FloatArray &position) = 0;
    /**
     * Returns volumetric (or other based measure) of relative material contents in given element.
     */
    virtual void giveElementMaterialMixture(FloatArray &answer, int ielem) = 0;
    /** Returns scalar value representation of material Interface at given point. For visualization */
    virtual double giveNodalScalarRepresentation(int) = 0;
};
} // end namespace oofem
#endif // materialinterface_h
