// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef SRC_SM_XFEM_NUCLEATIONCRITERIA_NCPRINCIPALSTRAIN_H_
#define SRC_SM_XFEM_NUCLEATIONCRITERIA_NCPRINCIPALSTRAIN_H_

#define _IFT_NCPrincipalStrain_Name "ncprincipalstrain"
#define _IFT_NCPrincipalStrain_StrainThreshold "strainthreshold"
#define _IFT_NCPrincipalStrain_IncrementLength "incrementlength"
#define _IFT_NCPrincipalStrain_PropStrainThreshold "propagationstrainthreshold"
#define _IFT_NCPrincipalStrain_InitialCrackLength "initialcracklength"
#define _IFT_NCPrincipalStrain_CrossSectionIndex "csindex"

#include "xfem/nucleationcriterion.h"
#include <memory>

namespace oofem {

class NCPrincipalStrain : public NucleationCriterion
{
public:
    NCPrincipalStrain(Domain *ipDomain);
    virtual ~NCPrincipalStrain();

    std::vector<std::unique_ptr<EnrichmentItem>> nucleateEnrichmentItems() override;

    void initializeFrom(InputRecord &ir) override;

    void appendInputRecords(DynamicDataReader &oDR) override;

    const char *giveClassName() const override { return "NCPrincipalStrain"; }
    const char *giveInputRecordName() const override { return _IFT_NCPrincipalStrain_Name; }

protected:
    double mStrainThreshold;
    double mInitialCrackLength;
    double mIncrementLength;
    double mPropStrainThreshold;

    /// If the initiated crack should cut exactly one element.
    bool mCutOneEl;

    /// Index of the cross section that the nucleation criterion applies to.
    int mCrossSectionInd;
};

} /* namespace oofem */

#endif /* SRC_SM_XFEM_NUCLEATIONCRITERIA_NCPRINCIPALSTRAIN_H_ */
