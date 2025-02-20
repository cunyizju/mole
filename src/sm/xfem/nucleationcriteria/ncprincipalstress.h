// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef SRC_SM_XFEM_NUCLEATIONCRITERIA_NCPRINCIPALSTRESS_H_
#define SRC_SM_XFEM_NUCLEATIONCRITERIA_NCPRINCIPALSTRESS_H_

#define _IFT_NCPrincipalStress_Name "ncprincipalstress"
#define _IFT_NCPrincipalStress_StressThreshold "stressthreshold"
#define _IFT_NCPrincipalStress_InitialCrackLength "initialcracklength"
#define _IFT_NCPrincipalStress_MatForceRadius "matforceradius"
#define _IFT_NCPrincipalStress_IncrementLength "incrementlength"
#define _IFT_NCPrincipalStress_CrackPropThreshold "crackpropthreshold"

#include "xfem/nucleationcriterion.h"

#include <memory>

namespace oofem {

class NCPrincipalStress : public NucleationCriterion
{
public:
    NCPrincipalStress(Domain *ipDomain);
    virtual ~NCPrincipalStress();

    std::vector<std::unique_ptr<EnrichmentItem>> nucleateEnrichmentItems() override;

    void initializeFrom(InputRecord &ir) override;

    void appendInputRecords(DynamicDataReader &oDR) override;

    const char *giveClassName() const override { return "NCPrincipalStress"; }
    const char *giveInputRecordName() const override {return _IFT_NCPrincipalStress_Name;};

protected:
    double mStressThreshold;
    double mInitialCrackLength;
    double mMatForceRadius;
    double mIncrementLength;
    double mCrackPropThreshold;

    /// If the initiated crack should cut exactly one element.
    bool mCutOneEl;

    /// Index of the cross section that the nucleation criterion applies to.
    int mCrossSectionInd;
};

} /* namespace oofem */

#endif /* SRC_SM_XFEM_NUCLEATIONCRITERIA_NCPRINCIPALSTRESS_H_ */
