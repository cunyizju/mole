// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef cohsur3d_h
#define cohsur3d_h

#include "sm/Elements/structuralelement.h"

///@name Input fields for CohSur3d
//@{
#define _IFT_CohesiveSurface3d_Name "cohsur3d"
#define _IFT_CohSur3d_kx "kx"
#define _IFT_CohSur3d_ky "ky"
#define _IFT_CohSur3d_kz "kz"
#define _IFT_CohSur3d_area "area"
//@}

namespace oofem {
/**
 * This class implements a cohesive surface element used by the
 * cohesive particle model.
 *
 * @author Milan Jirasek
 */
class CohesiveSurface3d : public StructuralElement
{
protected:
    double area, length;
    FloatArray center; ///< Coordinates of the center of the cohesive surface.
    FloatMatrix lcs; ///< Matrix defining the local coordinate system.

    ///@name Shift constants of periodic particles (near boundary of periodic cell).
    //@{
    int kx, ky, kz;
    double kxa, kyb, kzc;
    //@}

public:
    CohesiveSurface3d(int n, Domain * d);
    virtual ~CohesiveSurface3d() { }

    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui) override;
    double computeVolumeAround(GaussPoint *gp) override;
    int computeNumberOfDofs() override { return 6 * giveNumberOfNodes(); }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    double giveLength();
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override { }
    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;

    // definition & identification
    const char *giveClassName() const override { return "CohesiveSurface3d"; }
    const char *giveInputRecordName() const override { return _IFT_CohesiveSurface3d_Name; }

    // input and output
    void initializeFrom(InputRecord &ir) override;



protected:
    void computeGaussPoints() override;
    void evaluateCenter();
    void evaluateLocalCoordinateSystem();

    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    MaterialMode giveMaterialMode() override { return _3dInterface; }
};
} // namespace oofem
#endif
