// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ENRICHMENTFRONTCOHESIVEBRANCHFUNCONEEL_H_
#define ENRICHMENTFRONTCOHESIVEBRANCHFUNCONEEL_H_

#define _IFT_EnrFrontCohesiveBranchFuncOneEl_Name "enrfrontcohesivebranchfunconeel"

#include "oofemcfg.h"
#include <vector>
#include "input/inputrecord.h"
#include "xfem/enrichmentfronts/enrichmentfront.h"
#include "xfem/tipinfo.h"
#include "xfem/enrichmentfunction.h"


namespace oofem {
class XfemManager;
class DofManager;
class FloatArray;
class InputRecord;
class DynamicInputRecord;
class LinElBranchFunction;

/**
 * EnrFrontCohesiveBranchFuncOneEl
 * @author Erik Svenning
 * @date Nov 28, 2014
 */
class OOFEM_EXPORT EnrFrontCohesiveBranchFuncOneEl : public EnrichmentFront
{
public:
    EnrFrontCohesiveBranchFuncOneEl();
    virtual ~EnrFrontCohesiveBranchFuncOneEl();

    void MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan,  const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo) override;

    int giveNumEnrichments(const DofManager &iDMan) const override;
    int giveMaxNumEnrichments() const override { return 1; }

    // Evaluate the enrichment function and its derivative in front nodes.
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const override;
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const override;
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const override;

    const char *giveClassName() const override { return "EnrFrontCohesiveBranchFuncOneEl"; }
    const char *giveInputRecordName() const override { return _IFT_EnrFrontCohesiveBranchFuncOneEl_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    double giveSupportRadius() const override { return 0.0; }

private:
    CohesiveBranchFunction mpBranchFunc;
};

} /* namespace oofem */

#endif /* ENRICHMENTFRONTCOHESIVEBRANCHFUNCONEEL_H_ */
