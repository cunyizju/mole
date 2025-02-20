// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intelline1phf_h
#define intelline1phf_h

#include "sm/Elements/Interfaces/structuralinterfaceelementphf.h"

#define _IFT_IntElLine1PhF_Name "intelline1phf"
#define _IFT_IntElLine1PhF_axisymmode "axisymmode"

namespace oofem {
class FEI2dLineLin;

/**
 * 
 * @author Jim Brouzoulis
 */
class IntElLine1PhF : public StructuralInterfaceElementPhF
{
protected:
    static FEI2dLineLin interp;
    /// Flag controlling axisymmetric mode (integration over unit circumferential angle)
    bool axisymmode = false;

public:
    IntElLine1PhF(int n, Domain * d);

    FEInterpolation *giveInterpolation() const override;

    //int computeNumberOfDofs() override { return 8; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    double computeAreaAround(GaussPoint *gp) override;
    void computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    FloatArrayF<2> computeCovarBaseVectorAt(GaussPoint *gp) const;

    int testElementExtension(ElementExtension ext) override { return 0; }

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_IntElLine1PhF_Name; }
    const char *giveClassName() const override { return "IntElLine1PhF"; }
    void initializeFrom(InputRecord &ir) override;

    //void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, TimeStep *tStep) override;
    void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, const double damage, TimeStep *tStep) override;

    void giveStiffnessMatrix_Eng(FloatMatrix &answer, MatResponseMode rMode, IntegrationPoint *ip, TimeStep *tStep) override
    {
        answer = this->giveInterfaceCrossSection()->give2dStiffnessMatrix_Eng(rMode, ip, tStep);
    }

    void giveDofManDofIDMask_u(IntArray &answer) override;
    void giveDofManDofIDMask_d(IntArray &answer) override;
    void getLocationArray_u( IntArray &answer) override;
    void getLocationArray_d( IntArray &answer) override;
    void computeCovarBaseVectorsAt(GaussPoint *gp, FloatMatrix &G) override;

protected:
    void computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeGaussPoints() override;

    Element_Geometry_Type giveGeometryType() const override { return EGT_quad_1_interface; };
};
} // end namespace oofem
#endif
