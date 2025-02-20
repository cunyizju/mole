// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef basicelement_h
#define basicelement_h

#include "sm/Elements/structural2delement.h"
#define _IFT_BasicElement_Name "basicelement"

namespace oofem {
class FEI2dTrLin;

/**
 * This class implements a 'basic' triangular three-node plane-stress
 * finite element in the xy-plane. Each node has 2 degrees of freedom.
 * 
 * The current implementation is intended to serve as a simple prototype element in 
 * order to provide new users with a simple example of how a standard element can 
 * be implemented in OOFEM.
 * 
 * This element is a simplified version of the more general element TrPlaneStress2d,
 * which contains many additional (and optional) features. See 'trplanstrss.h' for
 * more info.
 * 
 * The Elements essentially only provides the interpolator, which is used by the 
 * base class PlaneStressElement for constructing the N- and B-matrices.
 *
 * @author Jim Brouzoulis 
 */
class BasicElement : public PlaneStressElement
{
protected:
    static FEI2dTrLin interp;

public:
    /// Constructor
    BasicElement(int n, Domain * d);
    /// Destructor.
    virtual ~BasicElement() { }

    FEInterpolation *giveInterpolation() const override;

    // Necessary for reading input files:
    const char *giveInputRecordName() const override { return _IFT_BasicElement_Name; }
    // Necessary for debug messages:
    const char *giveClassName() const override { return "BasicElement"; }

protected:

    // Support for computing the mass matrix needed for dynamic simulations
    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }
};
} // end namespace oofem
#endif // basicelement_h
