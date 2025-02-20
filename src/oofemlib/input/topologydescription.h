// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef topologydescription_h
#define topologydescription_h

#include "error/error.h"
#include "oofemcfg.h"

namespace oofem {
class SpatialLocalizer;
class TimeStep;
class Domain;
class DataReader;

/**
 * Determines the state of the evolving topology.
 */
enum TopologyState {
    TS_OK,             ///< Indicates that everything is OK with respect to topology.
    TS_NeedsRemeshing, ///< Indicates that the topology has reached a need for remeshing, as the case with merging surfaces.
};

/**
 * Abstract class for topology description.
 * The topology is coupled to a given domain. The main workload for a topology description is to be able to reproduce a finite element mesh from the results.
 *
 * @author Mikael Öhman
 */
class OOFEM_EXPORT TopologyDescription
{
protected:
    /// Domain which topology belongs to.
    Domain *d;

public:
    TopologyDescription(Domain * d) {
        this->d = d;
    }
    virtual ~TopologyDescription() { }
    /**
     * Instanciates itself.
     */
    virtual bool instanciateYourself(DataReader &dr) = 0;

    /**
     * Updates the topology from the FE solution.
     * @param tStep Active time step.
     */
    virtual TopologyState updateYourself(TimeStep *tStep) = 0;

    /**
     * File output of the current state of the topology description.
     * This is not handled by the export modules, since each type of representation can differ.
     * @param tStep Active time step.
     */
    virtual void doOutput(TimeStep *tStep)
    { OOFEM_ERROR( "Not implemented" ); }

    /**
     * Generates the FE components from the bare mesh.
     * Does not map fields or internal variables.
     * @todo Placing it in a new domain is probably preferable.
     */
    virtual void replaceFEMesh() // (Domain *& newDomain)
    { OOFEM_ERROR( "Not implemented" ); }

    /**
     * Changes the connected domain of receiver.
     * @note{Does not delete any existing objects.}
     */
    virtual void setDomain(Domain *newDomain) { d = newDomain; }

    /**
     * Gives the name of the class.
     */
    virtual const char *giveClassName() const = 0;

    /// Returns string for prepending output (used by error reporting macros).
    std :: string errorInfo(const char *func) const {
        return std::string(this->giveClassName()) + "::" + func;
    }
};
} // end namespace oofem
#endif // topologydescription_h
