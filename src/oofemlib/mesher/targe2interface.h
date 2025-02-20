// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef targe2interface_h
#define targe2interface_h

#include "mesher/mesherinterface.h"

#define _IFT_Targe2Interface_Name "targe2"

namespace oofem {
class TimeStep;

/**
 * This class represents the interface to Targe2 mesh generation package.
 * This interface is primarily responsible for two main tasks:
 * - to create input mesher file, containing all information including the mesh density information
 *   based on information from remeshing criteria.
 * - possibly to launch the mesher and transform its output to oofem input (using targe2oofem)
 */
class OOFEM_EXPORT Targe2Interface : public MesherInterface
{
public:
    /// Constructor
    Targe2Interface(Domain * d) : MesherInterface(d) { }
    /// Destructor
    virtual ~Targe2Interface() { }

    returnCode createMesh(TimeStep *tStep, int domainNumber, int domainSerNum, Domain **dNew) override;

protected:
    /// Creates the mesher input, containing the required mesh density informations.
    int createInput(Domain *d, TimeStep *tStep);
};
} // end namespace oofem
#endif // targe2interface_h
