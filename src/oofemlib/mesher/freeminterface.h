// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef freeminterface_h
#define freeminterface_h

#include "mesher/mesherinterface.h"
#include "math/floatarray.h"

#define _IFT_FreemInterface_Name "freem"

namespace oofem {
class TimeStep;

/**
 * This class represents the interface to freem mesh generation package.
 * This interface is primarily responsible for two main tasks:
 * - To create input mesher file, containing all information including the mesh density information
 *   based on the remeshing criteria.
 * - Possibly to launch the mesher and transform its output to oofem input.
 */
class OOFEM_EXPORT FreemInterface : public MesherInterface
{
public:
    /// Constructor
    FreemInterface(Domain * d) : MesherInterface(d) { }
    /// Destructor
    virtual ~FreemInterface() { }

    returnCode createMesh(TimeStep *tStep, int domainNumber, int domainSerNum, Domain **dNew) override;

protected:
    /// Creates the mesher input, containing the required mesh density information.
    int createInput(Domain *d, TimeStep *tStep);
    /// Service for smoothing the densities for freem.
    void smoothNodalDensities(Domain *d, FloatArray &nodalDensities, TimeStep *tStep);
};
} // end namespace oofem
#endif // freeminterface_h
