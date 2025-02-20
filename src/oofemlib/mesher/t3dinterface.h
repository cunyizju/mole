// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef t3dinterface_h
#define t3dinterface_h

#include "mesher/mesherinterface.h"
#include "input/element.h"

#define _IFT_T3DInterface_Name "t3d"

namespace oofem {
#define BMF_FILENAME "t3d.bmf"

class TimeStep;

/**
 * This class represents the interface to t3d mesh generation package.
 * This interface is primarily responsible for two main tasks:
 * - to create input mesher file, containing all information including the mesh density information
 *   based on information from remeshing criteria.
 * - possibly to launch the mesher and transform its output to oofem input (using t3d2oofem)
 */
class OOFEM_EXPORT T3DInterface : public MesherInterface
{
public:
    /// Constructor
    T3DInterface(Domain * d) : MesherInterface(d) { }
    /// Destructor
    virtual ~T3DInterface() { }

    returnCode createMesh(TimeStep *tStep, int domainNumber, int domainSerNum, Domain **dNew) override;
    int t3d_2_OOFEM(const char *t3dOutFile, Domain **dNew);
   // Used by HTS elemnt to mesh one element in order to export the HTS element into vtk
    int createInput(Element *e, char *t3dInFile);
    int createVTKExportMesh(const char *t3dOutFile,std::vector<FloatArray> &nodeCoords, std::vector<IntArray> &cellNodes, IntArray &cellTypes );
   // udes in Quasicontinuum    
    int createQCInterpolationMesh(const char *t3dOutFile,std::vector<FloatArray> &nodeCoords, std::vector<IntArray> &cellNodes, IntArray &cellTypes );

protected:
    /// Creates the mesher input, containing the required mesh density information.
    int createInput(Domain *d, TimeStep *tStep);
};
} // end namespace oofem
#endif // t3dinterface_h
