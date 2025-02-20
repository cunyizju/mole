// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef elementdofman_h
#define elementdofman_h

#include "dofman/dofmanager.h"

namespace oofem {
class Domain;
class Dof;
class NodalLoad;
class TimeStep;
class FloatArray;
class IntArray;
class Element;

/**
 * Class implementing internal element dof manager having some DOFs.
 * It possess degrees of freedom (see base class DofManager).
 * This class is usually attribute only of a single element, as its DOFs are internal element degrees of freedom.
 */
class OOFEM_EXPORT ElementDofManager : public DofManager
{
private:
    Element *element;

public:
    /**
     * Constructor.
     * @param n Element dof manager number.
     * @param aDomain Domain which receiver belongs to.
     * @param elem Element to which receiver belongs.
     */
    ElementDofManager(int n, Domain * aDomain, Element * elem);
    /// Destructor.
    virtual ~ElementDofManager();

    void initializeFrom(InputRecord &ir) override;
    void printYourself() override;
    const char *giveClassName() const override { return "ElementDofManager"; }
    const char *giveInputRecordName() const override { return ""; } // Note: Can't be created in input files.

    bool isDofTypeCompatible(dofType type) const override { return ( type == DT_master || type == DT_simpleSlave ); }
};
} // end namespace oofem
#endif // elementdofman_h
