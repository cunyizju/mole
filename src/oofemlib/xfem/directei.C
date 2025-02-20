// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "xfem/directei.h"
#include "input/geometry.h"

namespace oofem {
DirectEI :: DirectEI(int n, XfemManager *xm, Domain *aDomain) :
    GeometryBasedEI(n, xm, aDomain)
{}

DirectEI :: ~DirectEI()
{}

void DirectEI :: evalLevelSetNormal(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const
{
    mpBasicGeometry->computeNormalSignDist(oLevelSet, iGlobalCoord);
}

void DirectEI :: evalLevelSetTangential(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const
{
    double arcPos = 0.0;
    mpBasicGeometry->computeTangentialSignDist(oLevelSet, iGlobalCoord, arcPos);
}

void DirectEI :: evalGradLevelSetNormal(FloatArray &oGradLevelSet, const FloatArray &iGlobalCoord, const FloatMatrix &idNdX, const IntArray &iNodeInd) const
{
    double arcPos = 0.0, tangSignDist = 0.0;
    mpBasicGeometry->computeTangentialSignDist(tangSignDist, iGlobalCoord, arcPos);

    FloatArray tangent;
    mpBasicGeometry->giveTangent(tangent, arcPos);

    oGradLevelSet = {
        -tangent [ 1 ], tangent [ 0 ]
    };
}
} /* namespace oofem */
