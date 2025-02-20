// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef PROPAGATIONLAW_H_
#define PROPAGATIONLAW_H_

#include "oofemcfg.h"
#include "input/inputrecord.h"

#define _IFT_PLDoNothing_Name "propagationlawdonothing"

#define _IFT_PLCrackPrescribedDir_Name "propagationlawcrackprescribeddir"
#define _IFT_PLCrackPrescribedDir_Dir "angle" // Angle in degrees
#define _IFT_PLCrackPrescribedDir_IncLength "incrementlength" // Increment per time step

#define _IFT_PLnodeRadius_Name "propagationlawnoderadius"
#define _IFT_PLnodeRadius_Radius "radius" 


namespace oofem {
class Domain;
class EnrichmentDomain;
class DynamicInputRecord;
class EnrichmentFront;
struct TipPropagation;

/**
 * Updates the geometry of evolving XFEM interfaces.
 * @author Erik Svenning
 */

class OOFEM_EXPORT PropagationLaw
{
public:
    PropagationLaw();
    virtual ~PropagationLaw();

    virtual const char *giveClassName() const = 0;
    virtual const char *giveInputRecordName() const = 0;

    virtual void initializeFrom(InputRecord &ir) = 0;
    virtual void giveInputRecord(DynamicInputRecord &input) = 0;

    virtual bool hasPropagation() const = 0;
    virtual bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) = 0;
};

/**
 * Dummy propagation law that does nothing.
 * @author Erik Svenning
 */
class OOFEM_EXPORT PLDoNothing : public PropagationLaw
{
public:
    PLDoNothing() { }
    virtual ~PLDoNothing() { }

    const char *giveClassName() const override { return "PLDoNothing"; }
    const char *giveInputRecordName() const override { return _IFT_PLDoNothing_Name; }

    void initializeFrom(InputRecord &ir) override { }
    void giveInputRecord(DynamicInputRecord &input) override;

    bool hasPropagation() const override { return false; }
    bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) override { return false; }
};

/**
 * Propagation law that propagates the crack in a predefined direction.
 * @author Erik Svenning
 */
class OOFEM_EXPORT PLCrackPrescribedDir : public PropagationLaw
{
public:
    PLCrackPrescribedDir() : mAngle(0.0), mIncrementLength(0.0) { }
    virtual ~PLCrackPrescribedDir() { }

    const char *giveClassName() const override { return "PLCrackPrescribedDir"; }
    const char *giveInputRecordName() const override { return _IFT_PLCrackPrescribedDir_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool hasPropagation() const override { return mIncrementLength > 0.; }
    bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) override;

protected:
    double mAngle, mIncrementLength;
};

/**
 * Propagation law that propagates a delamination in a predefined radius from an element.
 * @author Johannes Främby
 */
class OOFEM_EXPORT PLnodeRadius : public PropagationLaw
{
public:
    PLnodeRadius() : mRadius(0.0) { }
    virtual ~PLnodeRadius() { }

    const char *giveClassName() const override { return "PLnodeRadius"; }
    const char *giveInputRecordName() const override { return _IFT_PLnodeRadius_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool hasPropagation() const override { return mRadius > 0.; }
    bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) override;

protected:
    double mRadius;
};
} // end namespace oofem

#endif /* PROPAGATIONLAW_H_ */
