// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fei1dquad_h
#define fei1dquad_h

#include "fei/feinterpol1d.h"

namespace oofem {
/**
 * Class representing a 1d Hermitian cubic isoparametric interpolation.
 * @author Mikael Öhman
 */
class OOFEM_EXPORT FEI1dHermite : public FEInterpolation1d
{
protected:
    int cindx;

public:
    FEI1dHermite(int coordIndx) : FEInterpolation1d(2), cindx(coordIndx) { }

    integrationDomain giveIntegrationDomain() const override { return _Line; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }

    double giveLength(const FEICellGeometry &cellgeo) const override;

    std::pair<double, FloatMatrixF<1,4>> evaldNdx(double ksi, const FEICellGeometry &cellgeo) const;
    FloatMatrixF<1,4> evald2Ndx2(double ksi, const FEICellGeometry &cellgeo) const;

    void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void evald2Ndx2(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    int  global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;
    double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo) const override;

    int giveNumberOfNodes() const override { return 2; }
};
} // end namespace oofem
#endif
