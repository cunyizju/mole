// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef patchintegrationrule_h
#define patchintegrationrule_h

#include "math/gaussintegrationrule.h"

namespace oofem {
class FEI2dTrLin;
class FEI3dTrQuad;
class Triangle;

/**
 * PatchIntegrationRule provides integration over a triangle patch.
 * Input to the constructor is:
 *  -int n:         number of quadrature points per triangle.
 *  -Element *e:    parent element pointer
 *  -iTriangles:    array of triangles describing the subdivision of the element.
 *
 * @author Erik Svenning (Major modifications)
 *
 */
class OOFEM_EXPORT PatchIntegrationRule : public GaussIntegrationRule
{
protected:
    std :: vector< Triangle >mTriangles;

    // Interpolation used to distribute quadrature points
    // in each triangle of the patch.
    static FEI2dTrLin mTriInterp;
    static FEI3dTrQuad mTriInterpQuad;


public:
    /// Constructor.
    PatchIntegrationRule(int n, Element *e, const std :: vector< Triangle > &iTriangles);
    /// Destructor.
    virtual ~PatchIntegrationRule();

    const char *giveClassName() const override { return "PatchIntegrationRule"; }

    // TODO: Give this function a better name.
    // Note: the fact that this function is inherited complicates name change.
    int SetUpPointsOnTriangle(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnWedge(int nPointsTri, int nPointsDepth, MaterialMode mode) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};
} // end namespace oofem
#endif // patchintegrationrule_h
