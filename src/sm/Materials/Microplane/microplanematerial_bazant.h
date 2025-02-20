// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef microplanematerial_bazant_h
#define microplanematerial_bazant_h

#include "sm/Materials/Structural/structuralms.h"
#include "microplanematerial.h"


namespace oofem {
/**
 * Abstract base class for all microplane models according to Bazant's approach.
 * Micro strains on microplane are described using magnitude of normal strain,
 * volumetric normal component and by two orthogonal shear components
 * (m and l direction) in microplane.
 */
class MicroplaneMaterial_Bazant : public MicroplaneMaterial
{
public:
    /**
     * Constructor. Creates Abstract Bazant's Microplane Material belonging
     * to domain d, with number n.
     * @param n Material number.
     * @param d Domain to which newly created material belongs.
     */
    MicroplaneMaterial_Bazant(int n, Domain * d);

    FloatArrayF<6> giveRealStressVector_3d(const FloatArrayF<6> &strain, GaussPoint *gp,
                                           TimeStep *tStep) const override;

    /**
    * Computes stress on given microplane (volumetric, deviatoric normal stresses and shead stresses)
    */
    virtual MicroplaneState giveRealMicroplaneStressVector(GaussPoint *gp, int mnumber, const MicroplaneState &strain, TimeStep *tStep) const = 0;

    /**
     * Updates the volumetric stress component after computing real stress microplane vectors.
     */
    virtual void updateVolumetricStressTo(GaussPoint *gp, int mnumber, double sigv) const = 0;

    const char *giveClassName() const override { return "MicroplaneMaterial_Bazant"; }
};
} // end namespace oofem
#endif // microplanematerial_bazant_h
