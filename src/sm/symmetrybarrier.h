// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef symmetrybarrier_h
#define symmetrybarrier_h

#include "material/nonlocalbarrier.h"
#include "math/intarray.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"

///@name Input fields for SymmetryBarrier
//@{
#define _IFT_SymmetryBarrier_Name "symmetrybarrier"
#define _IFT_SymmetryBarrier_origin "origin"
#define _IFT_SymmetryBarrier_normals "normals"
#define _IFT_SymmetryBarrier_activemask "activemask"
//@}

namespace oofem {
/**
 * Implementation of symmetry nonlocal barrier.
 * It allows to specify up to three planes (orthogonal ones) of symmetry
 * It then modifies the integration weights of source points to take into account
 * symmetry of the averaged field.
 */
class SymmetryBarrier : public NonlocalBarrier
{
protected:
    FloatArray origin;
    FloatArray normals;
    IntArray mask;
    FloatMatrix lcs;

public:
    /**
     * Constructor. Creates an element with number n belonging to domain aDomain.
     * @param n Element's number
     * @param d Pointer to the domain to which element belongs.
     */
    SymmetryBarrier(int n, Domain * d);
    /// Destructor.
    virtual ~SymmetryBarrier();

  void applyConstraint(const double cl, const FloatArray &c1, const FloatArray &c2, double &weight,
                         bool &shieldFlag, const NonlocalMaterialExtensionInterface &nei) override;
    double calculateMinimumDistanceFromBoundary(const FloatArray &coords) override { return 1.e10; }
    void initializeFrom(InputRecord &ir) override;

    const char *giveInputRecordName() const override { return _IFT_SymmetryBarrier_Name; }
    const char *giveClassName() const override { return "SymmetryBarrier"; }
};
} // end namespace oofem
#endif // symmetrybarrier_h
