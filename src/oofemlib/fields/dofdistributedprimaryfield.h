// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dofdistributedprimaryfield_h
#define dofdistributedprimaryfield_h

#include "fields/primaryfield.h"

namespace oofem {
/**
 * Class representing field of primary variables, which are typically allocated on nodes.
 * 
 * The class also handles the basic time integration, making it suitable for quasistatic problems.
 * VM_Total is @$ x = x_n @$
 * VM_Intermediate is @$ x = \alpha x_n + (1-\alpha)x_{n-1} @$
 * VM_Incremental is @$ \delta x = x_n - x_{n-1} @$
 * VM_Velocity @$ \dot x = (x_n - x_{n-1}) / \delta t @$
 * Accelerations are not considered.
 * 
 * Note that the physical unit(s) of x stems from the dof type. E.g. a viscous flow problem may have x be the velocity, 
 * and VM_Velocity is thus a fluid acceleration.
 * 
 * Problem domains that require other types of time integration should overload this class.
 */
class OOFEM_EXPORT DofDistributedPrimaryField : public PrimaryField
{
private:
    double alpha;

public:
    /**
     * Constructor. Creates a field of given type associated to given domain.
     * Not using pointer to domain, because this will prevent the use of PrimaryField as an
     * EngngModel attribute. This is because the domain does not exists when
     * PrimaryField is created (this is when EngngModel is created).
     * @param a Engineering model which field belongs to.
     * @param idomain Index of domain for field.
     * @param ft Type of stored field.
     * @param nHist Number of old time steps to store (minimum 1).
     * @param alpha Parameter for computing the interpolated intermediate value.
     */
    DofDistributedPrimaryField(EngngModel * a, int idomain, FieldType ft, int nHist=2, double alpha=1.0);
    virtual ~DofDistributedPrimaryField();

    void initialize(ValueModeType mode, TimeStep *tStep, FloatArray &answer, const UnknownNumberingScheme &s) override;

    double giveUnknownValue(Dof *dof, ValueModeType mode, TimeStep *tStep) override;

    void update(ValueModeType mode, TimeStep *tStep, const FloatArray &vectorToStore, const UnknownNumberingScheme &s) override;

    void applyDefaultInitialCondition() override;
    void applyInitialCondition(InitialCondition &ic);

    void applyBoundaryCondition(TimeStep *tStep) override;
    void applyBoundaryCondition(BoundaryCondition &bc, TimeStep *tStep);

    FloatArray *giveSolutionVector(TimeStep *tStep) override { OOFEM_ERROR("DEPRECATED"); return nullptr; }

    void setInitialGuess(DofManager &dman, TimeStep *tStep, TimeStep *prev);
    void advanceSolution(TimeStep *tStep) override;



    void saveContext(DataStream &stream) override { }
    void restoreContext(DataStream &stream) override { }
};
} // end namespace oofem
#endif // dofdistributedprimaryfield_h
