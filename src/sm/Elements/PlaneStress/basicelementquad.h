// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef basicelementquad_h
#define basicelementquad_h

#include "sm/Elements/structural2delement.h"
#define _IFT_BasicElementQuad_Name "basicelementquad"

namespace oofem {
class FEI2dQuadLin;

/**
 * This class implements a 'basic' quadratic four node plane-stress
 * finite element in the xy-plane. Each node has 2 degrees of freedom.
 * 
 * Based on the basicelement
 *
 * /@author Johannes Främby
 */
class BasicElementQuad : public PlaneStressElement
{
    /**
     * All of the following methods need to be implemented by the element 
     * (if not stated otherwise).
     */
    
protected:
    static FEI2dQuadLin interp;           

public:
    /// Constructor
    BasicElementQuad(int n, Domain * d);    
    /// Destructor.
    virtual ~BasicElementQuad() { }         

    FEInterpolation *giveInterpolation() const override;    
    const char *giveInputRecordName() const override { return _IFT_BasicElementQuad_Name; }
    const char *giveClassName() const override { return "BasicElementQuad"; }

protected:
    
    // - Support for computing the mass matrix needed for dynamic simulations
    int giveNumberOfIPForMassMtrxIntegration() override { return 4; }    


};
} // end namespace oofem
#endif // basicelementquad_h
