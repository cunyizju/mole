// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef delaunay_h
#define delaunay_h

#include "oofemcfg.h"

#include <vector>

namespace oofem {
class FloatArray;
class Triangle;

/**
 * O(n4) algorithm, only for testing purposes.
 * Yes, but 4th order in n. For the xfem element subdivision, n does not increase when the mesh is refined.
 * Time will tell if it is too slow ... /ES
 * @author chamrova
 * @author Erik Svenning
 */
class OOFEM_EXPORT Delaunay
{
public:
    Delaunay() : mTol(1.0e-12) { }

    bool colinear(const FloatArray &iP1, const FloatArray &iP2, const FloatArray &iP3) const;
    void printTriangles(std :: vector< Triangle > &triangles);
    bool isInsideCC(const FloatArray &iP, const FloatArray &iP1, const FloatArray &iP2, const FloatArray &iP3) const;
    void triangulate(const std :: vector< FloatArray > &iVertices, std :: vector< Triangle > &oTriangles) const;

private:
    const double mTol; /// Tolerance used when checking if points are colinear.
};
} // end namespace oofem
#endif  // delaunay_h
