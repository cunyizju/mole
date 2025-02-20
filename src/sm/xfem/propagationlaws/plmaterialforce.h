// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef PLMATERIALFORCE_H_
#define PLMATERIALFORCE_H_

#include "xfem/propagationlaw.h"

#include <memory>

#define _IFT_PLMaterialForce_Name "propagationlawmaterialforce"
#define _IFT_PLMaterialForce_Radius "radius" ///< Radius of region for domain integral
#define _IFT_PLMaterialForce_IncLength "incrementlength" ///< Increment length per time step
#define _IFT_PLMaterialForce_CrackPropThreshold "gc" ///<  Threshold for crack propagation

namespace oofem {

class Domain;
class EnrichmentDomain;
class DynamicInputRecord;
class MaterialForceEvaluator;


/**
 * Propagation law that propagates the crack in
 * the direction of the material force.
 *
 * @author Erik Svenning
 * @date Nov 14, 2014
 */
class OOFEM_EXPORT PLMaterialForce : public PropagationLaw
{
public:
    PLMaterialForce();
    virtual ~PLMaterialForce();

    const char *giveClassName() const override { return "PLMaterialForce"; }
    const char *giveInputRecordName() const override { return _IFT_PLMaterialForce_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool hasPropagation() const override { return mIncrementLength > 0.; } ///@todo Could this be done smarter? / Mikael
    bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) override;

    void setRadius(const double &iRadius) {mRadius = iRadius;}
    void setIncrementLength(const double &iIncrementLength) {mIncrementLength = iIncrementLength;}
    void setCrackPropThreshold(const double &iCrackPropThreshold) {mCrackPropThreshold = iCrackPropThreshold;}

protected:
    double mRadius, mIncrementLength, mCrackPropThreshold;

    std :: unique_ptr< MaterialForceEvaluator > mpMaterialForceEvaluator;
};

} /* namespace oofem */

#endif /* PLMATERIALFORCE_H_ */
