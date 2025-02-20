// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mesherinterface_h
#define mesherinterface_h

#include "oofemcfg.h"
#include "input/inputrecord.h"

namespace oofem {
class Domain;
class TimeStep;

/**
 * The base class representing the interface to mesh generation package.
 * This interface is primarily responsible for two main tasks:
 * - to create input mesher file, containing all information including the mesh density informations
 *   based on informations from remeshing criteria.
 * - possibly to launch the mesher and transform its output to oofem input
 */
class OOFEM_EXPORT MesherInterface
{
protected:
    Domain *domain;
public:
    enum returnCode { MI_OK, MI_NEEDS_EXTERNAL_ACTION, MI_FAILED };
    /// Constructor
    MesherInterface(Domain * d) {
        domain = d;
    }
    /// Destructor
    virtual ~MesherInterface() { }

    /**
     * Runs the mesh generation, mesh will be written to corresponding domain din file.
     * @param tStep Time step.
     * @param domainNumber New domain number.
     * @param domainSerNum New domain serial number.
     * @param dNew Newly allocated domain, representing new mesh or set to NULL if external generation has to be performed.
     */
    virtual returnCode createMesh(TimeStep *tStep, int domainNumber, int domainSerNum, Domain **dNew) = 0;
    /**
     * Initializes receiver according to object description stored in input record.
     * This function is called immediately after creating object using
     * constructor. Input record can be imagined as data record in component database
     * belonging to receiver. Receiver may use value-name extracting functions
     * to extract particular field from record.
     */
    virtual void initializeFrom(InputRecord &ir) { }
};
} // end namespace oofem
#endif // mesherinterface_h
