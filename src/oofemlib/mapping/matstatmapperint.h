// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef MATSTATMAPPERINT_H_
#define MATSTATMAPPERINT_H_

#include "material/materialmappingalgorithm.h"

#include <memory>

namespace oofem {
class MaterialStatus;
class StructuralInterfaceMaterialStatus;
class GaussPoint;
class Domain;
class TimeStep;
class Set;

/**
 * An interface class for MaterialStatus. The purpose is to allow
 * mapping of state variables (Gauss point variables) in a
 * generic way. This is useful when the mesh changes, e.g. due to
 * propagating cracks or in adaptive analysis.
 *
 * To add adaptivity capability to a new material model, only
 * copyStateVariables() and addStateVariables need to be overloaded
 * in the corresponding material status class. Hence, mapping
 * functionality can be implemented for new material models with
 * minimum effort.
 *
 * @note bp: this is however renundant to existing MaterialModelMapperInterface (MMI_map, MMI_update),
 * which is perhaps more flexible (several mapping algorithms, etc).
 *
 * @author Erik Svenning
 *  Created on: Nov 6, 2013
 */
class OOFEM_EXPORT MaterialStatusMapperInterface
{
protected:
    std::unique_ptr< MaterialMappingAlgorithm > mpMaterialMapper;

public:
    MaterialStatusMapperInterface();
    virtual ~MaterialStatusMapperInterface() {}

    virtual void copyStateVariables(const MaterialStatus &iStatus) = 0;
    virtual void addStateVariables(const MaterialStatus &iStatus) = 0;
    //    virtual void callCopyStateVariables(MaterialStatusMapperInterface &oStatus) = 0;


    /**
     * Maps all internal state variables from
     * the old domain to the given gp status.
     * @param iGP Integration point belonging to the new domain.
     * @param iOldDom Old domain.
     * @param iTStep Time step.
     * @return Nonzero if o.k.
     */
    virtual int MSMI_map(const GaussPoint &iGP, const Domain &iOldDom, Set &sourceSet, const TimeStep &iTStep, MaterialStatus &oStatus);

    virtual int MSMI_map_cz(const GaussPoint &iGP, const Domain &iOldDom, Set &sourceSet, const TimeStep &iTStep, MaterialStatus &oStatus);
    /**
     * Updates the internal state variables from previously mapped values.
     * @param iGP Integration point belonging to the new domain.
     * @param iTStep Time step.
     * @return Nonzero if o.k.
     */
    virtual int MSMI_update(const GaussPoint &iGP, const TimeStep &iTStep);
    /**
     * Finishes the mapping for given time step. Used to perform cleanup.
     */
    virtual int MSMI_finish(const TimeStep &iTStep);
};
} /* namespace oofem */
#endif /* MATSTATMAPPERINT_H_ */
