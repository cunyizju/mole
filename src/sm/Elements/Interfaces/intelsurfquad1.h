// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intelsurfquad1_h
#define intelsurfquad1_h

#include "sm/Elements/Interfaces/structuralinterfaceelement.h"
#include "fei/fei3dquadlin.h"
#include "math/floatarrayf.h"
#include "math/floatmatrixf.h"

#define _IFT_IntElSurfQuad1_Name "intelsurfquad1"

namespace oofem {
/**
 * This class implements 3d triangular surface interface element with linear interpolation.
 */
class IntElSurfQuad1 : public StructuralInterfaceElement
{
protected:
    static FEI3dQuadLin interpolation;

public:
    IntElSurfQuad1(int n, Domain *d);

    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;
    bool computeLocalCoordinates(FloatArray &answer, const FloatArray &gcoords) override;
    virtual void computeCovarBaseVectorsAt(IntegrationPoint *ip, FloatArray &G1, FloatArray &G2);
    //bool computeGtoLRotationMatrix(FloatMatrix &answer) override;
    void computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;

    int computeNumberOfDofs() override { return 24; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    double computeAreaAround(IntegrationPoint *ip) override;

    void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, TimeStep *tStep) override
    {
        answer = this->giveInterfaceCrossSection()->giveEngTraction_3d(jump, gp, tStep);
    }

    void giveStiffnessMatrix_Eng(FloatMatrix &answer, MatResponseMode rMode, IntegrationPoint *ip, TimeStep *tStep) override
    {
        answer = this->giveInterfaceCrossSection()->give3dStiffnessMatrix_Eng(rMode, ip, tStep);
    }

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_IntElSurfQuad1_Name; }
    void initializeFrom(InputRecord &ir) override;
    Element_Geometry_Type giveGeometryType() const override { return EGT_quad_1_interface; }



protected:
    void computeNmatrixAt(GaussPoint *ip, FloatMatrix &answer) override;
    void computeGaussPoints() override;

};
} // end namespace oofem
#endif // intelsurfquad1_h
