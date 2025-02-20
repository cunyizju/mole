// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef nonlocmatstiffinterface_h
#define nonlocmatstiffinterface_h

#include "utility/interface.h"
#include "material/nonlocalmaterialext.h"

namespace oofem {
class SparseMtrx;
class GaussPoint;
class TimeStep;
class UnknownNumberingScheme;

/**
 * Class Nonlocal Material Stiffness Interface. This is only abstract class.
 * This interface allows material model to add those services required to
 * compute and assemble nonlocal contribution to stiffness matrix.
 */
class OOFEM_EXPORT NonlocalMaterialStiffnessInterface : public Interface
{
public:
    /// Constructor.
    NonlocalMaterialStiffnessInterface() : Interface() { }

    /// Computes and adds IP contributions to destination matrix.
    virtual void NonlocalMaterialStiffnessInterface_addIPContribution(SparseMtrx &dest, const UnknownNumberingScheme &s,
                                                                      GaussPoint *gp, TimeStep *tStep) = 0;
    /**
     * Returns integration list of receiver. Contains localIntegrationRecord structures, containing
     * references to integration points and their weights that influence to nonlocal average in
     * receiver's associated integration point.
     */
    virtual std :: vector< localIntegrationRecord > *NonlocalMaterialStiffnessInterface_giveIntegrationDomainList(GaussPoint *gp) = 0;
};
} // end namespace oofem
#endif // nonlocmatstiffinterface_h
