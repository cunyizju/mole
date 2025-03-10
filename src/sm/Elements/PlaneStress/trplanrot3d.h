// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef trplanrot3d_h
#define trplanrot3d_h

#include "sm/Elements/PlaneStress/trplanrot.h"

#define _IFT_TrPlaneStrRot3d_Name "trplanestrrot3d"

namespace oofem {
#ifndef __CHARTENSOR
 #define __CHARTENSOR
enum CharTensor {
    LocalStrainTensor,
    GlobalStrainTensor,
    LocalCurvatureTensor,
    GlobalCurvatureTensor,

    LocalForceTensor,
    GlobalForceTensor,
    LocalMomentTensor,
    GlobalMomentTensor
};
#endif

/**
 * This class represent triangular plane stress element with rotational degree of freedom around normal
 * that can be arbitrary oriented in space, in contrast to base TrPlaneStrRot element that is
 * defined in xy-plane.
 *
 * Each node has 3 degrees of freedom.
 *
 * @author Ladislav Svoboda
 * @date 2010-5-25
 */
class TrPlaneStrRot3d : public TrPlaneStrRot
{
protected:
    /**
     * Transformation Matrix form GtoL(3,3) is stored
     * at the element level for computation efficiency
     */
    FloatMatrix GtoLRotationMatrix;

    void giveLocalCoordinates(FloatArray &answer, const FloatArray &global);
    void giveNodeCoordinates(FloatArray &x, FloatArray &y) override;

    double computeVolumeAround(GaussPoint *gp) override;
    void giveCharacteristicTensor(FloatMatrix &answer, CharTensor type, GaussPoint *gp, TimeStep *tStep);
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
    int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;
    void computeBodyLoadVectorAt(FloatArray &answer, Load *forLoad, TimeStep *tStep, ValueModeType mode) override;

    friend class TR_SHELL01;
    /**
     * @name Surface load support
     */
    //@{
    void computeSurfaceNMatrixAt(FloatMatrix &answer, int iSurf, GaussPoint *gp);
    void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;
    double computeSurfaceVolumeAround(GaussPoint *gp, int iSurf) override;
    int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int iSurf, GaussPoint *gp) override;
    //@}

public:
    TrPlaneStrRot3d(int n, Domain * d);
    virtual ~TrPlaneStrRot3d() { }

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_TrPlaneStrRot3d_Name; }
    const char *giveClassName() const override { return "TrPlaneStrRot3d"; }

    int computeNumberOfDofs() override { return 9; }
    int computeNumberOfGlobalDofs() override { return 18; }
    void giveDofManDofIDMask(int inode, IntArray &) const override;

    const FloatMatrix *computeGtoLRotationMatrix();
    bool computeGtoLRotationMatrix(FloatMatrix &answer) override;
    int testElementExtension(ElementExtension ext) override
    { return ( ( ext == Element_SurfaceLoadSupport ) ? 1 : 0 ); }

    void printOutputAt(FILE *file, TimeStep *tStep) override;
};
} // end namespace oofem
#endif //  trplanrot3d_h
