// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef geometrygenerator_h
#define geometrygenerator_h

#include "math/floatarray.h"
#include "input/element.h"
#include "dofman/node.h"

#define _IFT_GeometryGenerator_numOfParticles "gg_nop"
#define _IFT_GeometryGenerator_numOfIterations "gg_noi"
#define _IFT_GeometryGenerator_numOfItOnePar "gg_noiop"
#define _IFT_GeometryGenerator_particleRadius "gg_rp"
//...

namespace oofem {

/**
 * Generate random geometry of particles and links for CQ simulation.
 * 
 */
class GeometryGenerator 
{
protected:
    // global
    int nop; // number of particles
    int nol; // number of links
    std::vector<FloatArray> Particles;
    std::vector<IntArray> Links;

    // particleGenerator
    double ParticleRadius;  // minimal distance of two particles
    int maxNumOfParticles;  // maximal number of generated particles
    int maxNumOfIterations; // maximal number of iterations during generation
    int maxNumOfItOnePar;   // maximal number of generation of one particle

public:
    GeometryGenerator();
    virtual ~GeometryGenerator();

    void initializeParticleGenerator(InputRecord &ir);
    void generateParticles();
    void loadParticles();

    bool CheckDistances(double R, FloatArray coords, int n);


    void initializeLinkGenerator(InputRecord &ir);
    void generateLinks();
    void loadLinks();

    virtual const char *giveClassName() const { return "QCFullsolveddomain"; }
};
} // end namespace oofem
#endif // geometrygenerator_h
