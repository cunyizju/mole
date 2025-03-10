// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef nonlocalbarrier_h
#define nonlocalbarrier_h

#include "input/femcmpnn.h"
#include "material/nonlocalmaterialext.h"

namespace oofem {
/**
 * Abstract base class for all nonlocal barriers. The purpose of this class is to
 * model barrier for nonlocal averaging process (visibility criterion, symmetry condition).
 * Usually, the given remote integration point influences to the source point
 * nonlocal average if the averaging function at source point and evaluated for
 * remote point has nonzero value. The barrier allows to exclude additional points,
 * which may be close enough, but due to several reasons there is no influence
 * between these points (for example, they can be  separated by a notch).
 *
 * @see NonlocalMaterialStatusExtensionInterface class.
 */
class OOFEM_EXPORT NonlocalBarrier : public FEMComponent
{
public:
    /**
     * Constructor. Creates an element with number n belonging to domain aDomain.
     * @param n Element's number
     * @param aDomain Pointer to the domain to which element belongs.
     */
    NonlocalBarrier(int n, Domain * aDomain);
    /// Destructor.
    virtual ~NonlocalBarrier() { }

    /*
     * Abstract method returning true if the barrier is activated
     * by interaction of two given points. In this case the nonlocal influence
     * is not considered. Otherwise returns false.
     * @param c1 Coordinates of first point.
     * @param c2 Coordinates of second point.
     * @return True if barrier is activated, false otherwise.
     */
    // virtual bool isActivated (const FloatArray& c1, const FloatArray& c2) = 0;

    /**
     * Abstract method modifying the integration weight between master (c1) and source (c2) point.
     * @param cl Characteristic length of nonlocal model.
     * @param c1 Coordinates of master point.
     * @param c2 Coordinates of source point.
     * @param weight Original integration weight; on output modified weight.
     * @param[out] shieldFlag Set to true if shielding is activated.
     * @param nei The element with the non local material extension.
     */
    virtual void applyConstraint(const double cl, const FloatArray &c1, const FloatArray &c2, double &weight,
                                 bool &shieldFlag, const NonlocalMaterialExtensionInterface &nei) = 0;

    /**
     * Abstract method calculating the minimum distance of the Gauss Point
     * from the nonlocal boundaries
     * @param coords Coordinates of the Gauss Point
     * @param maxPossibleDistance Distance from the boundary beyond which the nonlocal radius(as it is interpreted in each weight function) becomes equal to the user-defined
     * @return the minimum value of the minimum distance from nonlocal boundary and maxPossibleDistance
     */
    virtual double calculateMinimumDistanceFromBoundary(const FloatArray &coords) = 0;
};
} // end namespace oofem
#endif // nonlocalbarrier_h
