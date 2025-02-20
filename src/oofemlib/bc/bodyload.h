// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef bodyload_h
#define bodyload_h

#include "bc/load.h"
#include "bc/bcgeomtype.h"

namespace oofem {
/**
 * Class implementing element body load, acting over whole element volume (e.g., the dead weight).
 * Body load is usually attribute of one or more elements.
 *
 * This base body load class only defines the common services common to all derived classes.
 * Derived classes need to implement services declared by base Load class.
 */
class OOFEM_EXPORT BodyLoad : public Load
{
public:
    /**
     * Constructor. Creates Body Load object with given number, belonging to given domain.
     * @param i Load number
     * @param d Domain to which new load belongs.
     */
    BodyLoad(int i, Domain * d) : Load(i, d) { }

    /**
     * Returns receiver's load geometry type.
     * @return BodyLoadBGT.
     */
    bcGeomType giveBCGeoType() const override { return BodyLoadBGT; }
};
} // end namespace oofem
#endif // bodyload_h
