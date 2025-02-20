// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dofmanvalfield_h
#define dofmanvalfield_h

#include "fields/field.h"
#include "input/elementgeometrytype.h"
#include "error/error.h"
#include "engng/engngm.h"
#include "input/domain.h"
#include <vector>
#include "input/internalstatetype.h"
#include "math/intarray.h"
#include "material/materialmappingalgorithm.h"
#include "mapping/mmashapefunctprojection.h"
#include "input/cltypes.h"
    
namespace oofem {
class Domain;

/**
 * Class representing field defined by nodal values associated to given domain.
 * Field represent the spatial distribution of certain variable.
 * The implementation allows to set individual dofMan values;
 * However, in the current implementation does not allow to specify values for different time steps.
 * It is also possible to create a separate, virtual domain by adding particular
 * nodes and elements with nodal values.
 */
class OOFEM_EXPORT DofManValueField : public Field
{
protected:
    /// Associated domain (need its elements to interpolate)
    Domain *domain;
    /// Array of dofman values
    std::vector< FloatArray >dmanvallist;
    /// Pointer to engineering model
    std::unique_ptr< EngngModel >eModel;
    /// Pointer to single cross-section;
    std::unique_ptr< CrossSection >crossSect;

public:
    /**
     * Constructor. Creates an empty field of given type associated to given domain.
     */
    DofManValueField(FieldType ft, Domain *d);

    /**
     * Constructor. Creates a virtual empty domain which needs to be populated
     * by nodes, elements and nodal values.
     */

    DofManValueField(FieldType ft, int nNodes, int nElements, const std::string engngModel,  const std::string domainDofsDefaults);

    virtual ~DofManValueField() { }

    /**
     * Add node to the domain
     */
    void addNode(int i, const FloatArray &coords);

    /**
     * Add element to the domain
     */
    void addElement(int i, const char *name, const IntArray &nodes);

    int evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep) override;

    /**
     * Evaluates the field at given DofManager. This potentially can be resolved quickly, as
     * receiver data may be described using values at dof managers. Here an additional issue
     * exists: one needs to make sure, that passed dman is from the same domain, so that its
     * number can be used to perform suggested quick evaluation.
     *
     * If this is not the case (the field is described differently),
     * the response can be evaluated using dofman coordinates in a standard way.
     * @param[out] answer Evaluated field for dman.
     * @param dman Reference to dof manager.
     * @param mode Mode of value (total, velocity,...).
     * @param tStep TimInternalStateType valID, IntArray regions, TimeStep *tStepe step to evaluate for.
     * @return Zero if ok, nonzero Error code (0-ok, 1-failed)
     */
    int evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep) override;

    /**
     * Sets the value associated to given dofManager
     */
    void setDofManValue(int dofMan, FloatArray value);

    /**
     * Obtain coordinates of a node
     */
    const FloatArray &getNodeCoordinates(int i);

    void saveContext(DataStream &stream) override;
    void restoreContext(DataStream &stream) override;

    const char *giveClassName() const override {
        return "DofManValueField";
    }
};
} // end namespace oofem
#endif // dofmanvalfield_h
