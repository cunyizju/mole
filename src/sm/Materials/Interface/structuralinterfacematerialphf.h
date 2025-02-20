// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef structuralinterfacematerialphf_h
#define structuralinterfacematerialphf_h

#include "sm/Materials/Interface/structuralinterfacematerial.h"
#include "math/floatmatrixf.h"

namespace oofem {
class GaussPoint;

/**
 *
 * @author Jim Brouzoulis
 */
class StructuralInterfaceMaterialPhF : public StructuralInterfaceMaterial
{
    struct Tangents {
        FloatMatrixF<3,3> jj;
        FloatArrayF<3> jd;
        FloatArrayF<3> dj;
        double dd; 
    };

public:
    StructuralInterfaceMaterialPhF(int n, Domain * d);

    //virtual FloatArrayF<1> giveEngTraction_1d(const FloatArrayF<1> &jump, double damage, GaussPoint *gp, TimeStep *tStep);
    virtual FloatArrayF<2> giveEngTraction_2d(const FloatArrayF<2> &jump, double damage, GaussPoint *gp, TimeStep *tStep) const;
    virtual FloatArrayF<3> giveEngTraction_3d(const FloatArrayF<3> &jump, double damage, GaussPoint *gp, TimeStep *tStep) const
    { OOFEM_ERROR("not implemented "); };

    virtual double giveDrivingForce(GaussPoint *gp) const { OOFEM_ERROR("not implemented "); return -1; }
    virtual double giveDrivingForcePrime(GaussPoint *gp) const { OOFEM_ERROR("not implemented "); return -1; }

    // Df/Dd
    //virtual void giveDrivingForceTangent(FloatMatrix &answer,  MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    virtual void giveTangents(FloatMatrix &jj, FloatMatrix &jd, FloatMatrix &dj, FloatMatrix &dd, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const
    { OOFEM_ERROR("not implemented "); };

    const char *giveClassName() const override { return "StructuralInterfaceMaterialPhF"; }
};
} // end namespace oofem
#endif // StructuralInterfaceMaterialPhF_h
