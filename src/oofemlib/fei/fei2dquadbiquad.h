// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei2dquadbiquad_h
#define fei2dquadbiquad_h

#include "fei/fei2dquadquad.h"

namespace oofem {
/**
 * Class representing a 2d quadrilateral with bi-quadratic interpolation based on isoparametric coordinates.
 * Local Node Numbering
 *       ^ eta
 *       |
 * (4)--(7)--(3)
 *  |         |
 *  |         |
 * (8)  (9)  (6)-->ksi
 *  |         |
 *  |         |
 * (1)--(5)--(2)
 * Everything regarding edges can be directly inherited by FEI2dQuadQuad.
 * @note Untested.
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI2dQuadBiQuad : public FEI2dQuadQuad
{
public:
    FEI2dQuadBiQuad(int ind1, int ind2) : FEI2dQuadQuad(ind1, ind2) { }

    static FloatArrayF<9> evalN(const FloatArrayF<2> &lcoords);
    static FloatMatrixF<2,9> evaldNdxi(const FloatArrayF<2> &lcoords);
    std::pair<double, FloatMatrixF<2,9>> evaldNdx(const FloatArrayF<2> &lcoords, const FEICellGeometry &cellgeo) const;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo)  const override;
    void evaldNdxi(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int giveNumberOfNodes() const override { return 9; }
    std::unique_ptr<IntegrationRule> giveIntegrationRule(int order) const override;
};
} // end namespace oofem
#endif // fei2dquadbiquad_h
