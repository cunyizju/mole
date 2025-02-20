// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intelline1_h
#define intelline1_h

#include "sm/Elements/Interfaces/structuralinterfaceelement.h"
#include "math/floatmatrixf.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"

#define _IFT_IntElLine1_Name "intelline1"
#define _IFT_IntElLine1_axisymmode "axisymmode"

namespace oofem {
class FEI2dLineLin;

/**
 * This class implements a two dimensional interface element.
 * Even if geometry approx is quadratic, the element is assumed straight
 * If not straight, the rotation matrix depends on actual integration point
 * and stiffness and strain computations should be modified.
 * 
 * @author Jim Brouzoulis
 * @author Borek Patzak
 */
class IntElLine1 : public StructuralInterfaceElement, public NodalAveragingRecoveryModelInterface
{
protected:
    static FEI2dLineLin interp;
    /// Flag controlling axisymmetric mode (integration over unit circumferential angle)
    bool axisymmode = false;

public:
    IntElLine1(int n, Domain * d);

    FEInterpolation *giveInterpolation() const override;

    int computeNumberOfDofs() override { return 8; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    double computeAreaAround(GaussPoint *gp) override;
    void computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    virtual FloatArrayF<2> computeCovarBaseVectorAt(GaussPoint *gp) const;

    int testElementExtension(ElementExtension ext) override { return 0; }

    Interface *giveInterface(InterfaceType interface) override;
    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                    InternalStateType type, TimeStep *tStep) override;
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_IntElLine1_Name; }
    const char *giveClassName() const override { return "IntElLine1"; }
    void initializeFrom(InputRecord &ir) override;

    void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, TimeStep *tStep) override
    {
        answer = this->giveInterfaceCrossSection()->giveEngTraction_2d(jump, gp, tStep);
    }

    void giveStiffnessMatrix_Eng(FloatMatrix &answer, MatResponseMode rMode, IntegrationPoint *ip, TimeStep *tStep) override
    {
        answer = this->giveInterfaceCrossSection()->give2dStiffnessMatrix_Eng(rMode, ip, tStep);
    }



protected:
    void computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeGaussPoints() override;

    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }
};
} // end namespace oofem
#endif
