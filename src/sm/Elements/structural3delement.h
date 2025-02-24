// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structural3delement_h
#define structural3delement_h

#include "sm/Elements/nlstructuralelement.h"


#define _IFT_Structural3DElement_materialCoordinateSystem "matcs" ///< [optional] Support for material directions based on element orientation.

namespace oofem {
class GaussPoint;
class FloatMatrix;
class FloatArray;
class IntArray;

/**
 * Base class 3D elements.
 *
 * @author Jim Brouzoulis
 * @author Mikael Öhman
 */
class Structural3DElement : public NLStructuralElement
{
protected:
    bool matRotation;

public:
    /**
     * Constructor. Creates element with given number, belonging to given domain.
     * @param n Element number.
     * @param d Domain to which new material will belong.
     */
    Structural3DElement(int n, Domain *d);
    /// Destructor.
    virtual ~Structural3DElement() { }

    void initializeFrom(InputRecord &ir) override;

    MaterialMode giveMaterialMode() override;
    int computeNumberOfDofs() override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    double computeVolumeAround(GaussPoint *gp) override;

    double giveCharacteristicLength(const FloatArray &normalToCrackPlane) override;

    void giveMaterialOrientationAt(FloatArray &x, FloatArray &y, FloatArray &z, const FloatArray &lcoords);
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;

    void computeInitialStressMatrix(FloatMatrix &answer, TimeStep *tStep) override;

protected:
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int lowerIndx = 1, int upperIndx = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeGaussPoints() override;

    // Edge support
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    double computeEdgeVolumeAround(GaussPoint *gp, int iEdge) override;
    int computeLoadLEToLRotationMatrix(FloatMatrix &answer, int iEdge, GaussPoint *gp) override;

    int testElementExtension(ElementExtension ext) override
    { return ( ( ( ext == Element_EdgeLoadSupport ) || ( ext == Element_SurfaceLoadSupport ) ) ? 1 : 0 ); }

    void computeSurfaceNMatrixAt(FloatMatrix &answer, int iSurf, GaussPoint *gp);
    void giveSurfaceDofMapping(IntArray &answer, int) const override;
    double computeSurfaceVolumeAround(GaussPoint *gp, int) override;
    int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int, GaussPoint *gp) override;

private:
    double dnx(int i, int arg2);
};
} // end namespace oofem
#endif // structural3delement_h
