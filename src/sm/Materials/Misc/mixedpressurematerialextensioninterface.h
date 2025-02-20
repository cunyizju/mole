// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mixedpressurematerialextensioninterface_h
#define mixedpressurematerialextensioninterface_h

#include "utility/interface.h"
#include "material/matresponsemode.h"
#include "input/domain.h"

///@name micromorphicmaterialextensioninterface
//@{

//@}

namespace oofem {
class FloatMatrix;
class FloatArray;
class GaussPoint;
class TimeStep;



/**
 * Material interface for gradient material models.
 */
class MixedPressureMaterialExtensionInterface : public Interface
{
protected:
    Domain *dom = nullptr;

public:
    /**
     * Constructor. Creates material with given number, belonging to given domain.
     * @param d Domain to which new material will belong.
     */
    MixedPressureMaterialExtensionInterface(Domain *d);
    /// Destructor.
    virtual ~MixedPressureMaterialExtensionInterface() { }


    virtual void giveDeviatoric3dMaterialStiffnessMatrix(FloatMatrix &answer,
                                                         MatResponseMode,
                                                         GaussPoint *gp,
                                                         TimeStep *tStep)
    { OOFEM_ERROR("not implemented "); }


    virtual void giveDeviatoricPlaneStrainStiffMtrx(FloatMatrix &answer,
                                                    MatResponseMode, GaussPoint *gp,
                                                    TimeStep *tStep)
    { OOFEM_ERROR("not implemented "); }


    virtual void giveDeviatoricConstitutiveMatrix(FloatMatrix & answer,
                                                  MatResponseMode, GaussPoint * gp,
                                                  TimeStep * tStep);


    virtual void giveInverseOfBulkModulus(double &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) = 0;


    void giveRealStressVector(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep);

    virtual void giveRealStressVector_3d(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep) = 0;
    virtual void giveRealStressVector_PlaneStrain(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep);
    //    virtual void giveRealStressVector_PlaneStress(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedStrain, double pressure, TimeStep *tStep) = 0;

    virtual void giveFiniteStrainGeneralizedStressVectors(FloatArray &sigma, GaussPoint *gp, const FloatArray &devF, double pressure, TimeStep *tStep) {; }
};
}
#endif
