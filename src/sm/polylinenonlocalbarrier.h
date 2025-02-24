// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef polylinenonlocalbarrier_h
#define polylinenonlocalbarrier_h

#include "material/nonlocalbarrier.h"
#include "math/intarray.h"

///@name Input fields for PolylineNonlocalBarrier
//@{
#define _IFT_PolylineNonlocalBarrier_Name "polylinebarrier"
#define _IFT_PolylineNonlocalBarrier_vertexnodes "vertexnodes"
#define _IFT_PolylineNonlocalBarrier_xcoordindx "xcoordindx"
#define _IFT_PolylineNonlocalBarrier_ycoordindx "ycoordindx"
//@}

namespace oofem {
/**
 * Implementation of polyline nonlocal barrier.
 * It is a composite one-dimensional cell consisting of one or more connected lines.
 * The polyline is defined by an ordered list of n+1 vertices (nodes),
 * where n is the number of lines in the polyline.
 * Each pair of points (i,i+1) defines a line.
 *
 * The purpose of this class is to
 * model barrier for nonlocal averaging process (visibility criterion).
 * Usually, the given remote integration point influences to the source point
 * nonlocal average if the averaging function at source point and evaluated for
 * remote point has nonzero value. The barrier allows to exclude additional points,
 * which may be close enough, but due to several reasons there is no influence
 * between these points (for example, they can be  separated by a notch).
 */
class PolylineNonlocalBarrier : public NonlocalBarrier
{
protected:
    /// Local x-coordinate index.
    int localXCoordIndx;
    /// Local y-coordinate index.
    int localYCoordIndx;
    /// List of polyline vertices.
    IntArray vertexNodes;

public:
    /**
     * Constructor. Creates an element with number n belonging to domain aDomain.
     * @param n Barrier's number
     * @param aDomain Pointer to the domain to which element belongs.
     */
    PolylineNonlocalBarrier(int n, Domain * aDomain);
    /// Virtual destructor.
    virtual ~PolylineNonlocalBarrier();

    virtual bool isActivated(const FloatArray &c1, const FloatArray &c2);

  void applyConstraint(const double cl, const FloatArray &c1, const FloatArray &c2, double &weight,
                         bool &shieldFlag, const NonlocalMaterialExtensionInterface &nei) override;

    void initializeFrom(InputRecord &ir) override;

    double calculateMinimumDistanceFromBoundary(const FloatArray &coords) override;

    /**
     * This function computes the length of the normal to the line defined by 2 vertices that passes through
     * the given Gauss Point. If the intersection point lies outside the line segment the minimum of the
     * distances between the Gauss Point and the two vertices is returned
     * @param coordsA Coordinates of the ith vertex of the polyline
     * @param coordsB Coordinates of the (i+1)th vertex of the polyline
     * @param coordsGP Coordinates of the Gauss Point whose nonlocal interactions domain is modified based on the distance based averaging
     */
    double giveDistancePointLine(const FloatArray &coordsA, const FloatArray &coordsB, const FloatArray &coordsGP);

    const char *giveInputRecordName() const override { return _IFT_PolylineNonlocalBarrier_Name; }
    const char *giveClassName() const override { return "PolylineNonlocalBarrier"; }
};
} // end namespace oofem
#endif // polylinenonlocalbarrier_h
