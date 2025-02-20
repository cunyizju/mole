// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef INCLUSIONEI_H_
#define INCLUSIONEI_H_

#define _IFT_Inclusion_Name "inclusion"
#define _IFT_Inclusion_CrossSection "crosssection"

#include "xfem/enrichmentitem.h"
#include "xfem/hybridei.h"

namespace oofem {
/**
 * EnrichmentItem describing an inclusion
 * @author Erik Svenning (among others)
 * @date Sep 9, 2014
 */
class OOFEM_EXPORT Inclusion : public HybridEI
{
protected:
    CrossSection *mpCrossSection;
public:
    Inclusion(int n, XfemManager *xm, Domain *aDomain);
    virtual ~Inclusion();

    // Returns true if the enrichment item can assign
    // a different material to any Gauss point.
    bool canModifyMaterial() const override { return true; }

    // Returns true if the enrichment item assigns a different material to the Gauss point
    bool isMaterialModified(GaussPoint &iGP, Element &iEl, CrossSection * &opCS) const override;

    const char *giveClassName() const override { return "Inclusion"; }
    const char *giveInputRecordName() const override { return _IFT_Inclusion_Name; }
    void initializeFrom(InputRecord &ir) override;
    CrossSection *giveCrossSection() { return mpCrossSection; }
};
} /* namespace oofem */

#endif /* INCLUSIONEI_H_ */
