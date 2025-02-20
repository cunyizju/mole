// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef beambaseelement_h
#define beambaseelement_h

#include "sm/Elements/structuralelement.h"
#include "sm/CrossSections/fiberedcs.h"
#include "sm/Materials/Elasticity/winklermodel.h"
#include "dofman/dofmanager.h"

namespace oofem {

/**
 * This class implements a base beam intented to be a base class for
 * beams based on lagrangian interpolation, where exact end forces can
 * be recovered.
 */
 class BeamBaseElement : public StructuralElement
{
protected:

 public:
    BeamBaseElement (int n, Domain *d);
    virtual ~BeamBaseElement();

protected:
    /** Computes element end force vector from applied loading in local coordinate system
     * @param answer computed end force vector due to non-nodal loading
     * @param tStep solution step
     * @param mode determines response mode
     */
    virtual void computeLocalForceLoadVector(FloatArray &answer, TimeStep *tStep, ValueModeType mode);

};
} // end namespace oofem
#endif // beambaseelement_h
