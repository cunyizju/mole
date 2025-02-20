// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef particle_h
#define particle_h

#include "dofman/node.h"

///@name Input fields for Particle
//@{
#define _IFT_Particle_Name "particle"
#define _IFT_Particle_rad "rad"
//@}

namespace oofem {
/**
 * Class implementing spherical particles as special nodes having a certain radius.
 * Such particles are used by the cohesive particle model.
 * @author Milan Jirasek
 */
class Particle : public Node
{
protected:
    /// Particle radius (only spherical particles considered).
    double radius;

public:
    /**
     * Constructor. Creates a particle with number n, belonging to aDomain.
     * @param n Particle number in domain aDomain.
     * @param aDomain Domain to which the particle belongs.
     */
    Particle(int n, Domain * aDomain);
    /**
     * Destructor.
     */
    virtual ~Particle(void) { }

    void initializeFrom(InputRecord &ir) override;
    const char *giveInputRecordName() const override { return _IFT_Particle_Name; }
    const char *giveClassName() const override { return "Particle"; }

    /// Returns the radius of the particle.
    double giveRadius() const { return radius; }
};
} // end namespace oofem
#endif
