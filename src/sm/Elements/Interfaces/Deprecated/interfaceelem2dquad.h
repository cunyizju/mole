// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef interfaceelem2dquad_h
#define interfaceelem2dquad_h

#include "sm/Elements/structuralelement.h"

#define _IFT_InterfaceElem2dQuad_Name "interface2dquad"
#define _IFT_InterfaceElem2dQuad_axisymmode "axisymmode"

namespace oofem {
class FEI2dLineQuad;

/**
 * This class implements a two dimensional interface element.
 * Even if geometry approx is quadratic, the element is assumed straight
 * If not straight, the rotation matrix depends on actual integration point
 * and stiffness and strain computations should be modified.
 */
class InterfaceElem2dQuad : public StructuralElement
{
protected:
    static FEI2dLineQuad interp;
    /// Flag controlling axisymmetric mode (integration over unit circumferential angle)
    bool axisymmode;

public:
    InterfaceElem2dQuad(int n, Domain * d);
    virtual ~InterfaceElem2dQuad() { }

    FEInterpolation *giveInterpolation() const override;

    int computeNumberOfDofs() override { return 12; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    double computeVolumeAround(GaussPoint *gp) override;


    int testElementExtension(ElementExtension ext) override { return 0; }

    Interface *giveInterface(InterfaceType) override { return NULL; }



    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_InterfaceElem2dQuad_Name; }
    const char *giveClassName() const override { return "InterfaceElem2dQuad"; }
    void initializeFrom(InputRecord &ir) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    MaterialMode giveMaterialMode() override { return _2dInterface; }

protected:
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override { }
    void computeGaussPoints() override;

    bool computeGtoLRotationMatrix(FloatMatrix &answer) override;
};
} // end namespace oofem
#endif // interfaceelem2dquad_h
