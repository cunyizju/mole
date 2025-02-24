// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef randommaterialext_h
#define randommaterialext_h

#include "material/matstatus.h"
#include "utility/dictionary.h"
#include "utility/interface.h"
#include "math/intarray.h"

///@name Input fields for RandomMaterialExt
//@{
#define _IFT_RandomMaterialExt_randVariables "randvars"
#define _IFT_RandomMaterialExt_randGen "randgen"
//@}

namespace oofem {
/**
 * Abstract base class for all random constitutive model statuses.
 * Random materials can have some their constitutive constants
 * randomly generated. In this case, these "constants" are stored
 * in material statuses of corresponding integration points.
 * Usually, an instance of randomGenerator is used to generate
 * the values in integration points.
 */
class OOFEM_EXPORT RandomMaterialStatusExtensionInterface : public Interface
{
protected:
    /// Dictionary containing material model values.
    Dictionary randProperties;

public:
    /**
     * Constructor.
     */
    RandomMaterialStatusExtensionInterface() : randProperties() { }

    /// Destructor.
    virtual ~RandomMaterialStatusExtensionInterface() { }

    /**
     * Returns the value of random property, identified by a key.
     * @return False if property not available.
     */
    bool _giveProperty(int key, double &value);
    /**
     * Sets the value of random property, identified by a key.
     */
    void _setProperty(int key, double value);
};


/**
 * Abstract base class for all random materials. Materials supporting random interface
 * can store some of their constants inside integration points (in their statuses).
 * This allows, for example, to set up random variation of certain parameter while still
 * setting up only one material model within the FE model.
 * The default implementation of provided services assumes that material statuses
 * created by the material are derived from base RandomMaterialStatusExtensionInterface.
 */
class OOFEM_EXPORT RandomMaterialExtensionInterface : public Interface
{
protected:
    /// Array of randomized variables (identified by a key).
    IntArray randVariables;
    /// Array of generators id's for corresponding randomized variables.
    IntArray randomVariableGenerators;
public:
    /// Constructor.
    RandomMaterialExtensionInterface()  : Interface(), randVariables(), randomVariableGenerators()
    { }
    /// Destructor.
    virtual ~RandomMaterialExtensionInterface()
    { }

public:
    /**
     * Initializes receiver according to object description stored in input record.
     * The density of material is read into property dictionary (keyword 'd')
     * Intended to be called from material initializeFrom service.
     */
    void initializeFrom(InputRecord &ir);
    void giveInputRecord(DynamicInputRecord &ir);
    /**
     * Returns the property in associated status of given integration point if defined.
     * @returns true if property available, false otherwise
     */
    bool give(int key, GaussPoint *gp, double &value) const;

protected:

    /**
     * Sets up (generates) the variables identified in randVariables array using generators
     * given in randomVariableGenerators and stores them in given status.
     * Should be called from material CreateStatus service.
     */
    void _generateStatusVariables(GaussPoint *) const;
};
} // end namespace oofem
#endif // randommaterialext_h
