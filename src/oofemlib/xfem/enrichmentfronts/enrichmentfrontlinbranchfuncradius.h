// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ENRICHMENTFRONTLINBRANCHFUNCRADIUS_H_
#define ENRICHMENTFRONTLINBRANCHFUNCRADIUS_H_

#define _IFT_EnrFrontLinearBranchFuncRadius_Name "enrfrontlinearbranchfuncradius"
#define _IFT_EnrFrontLinearBranchFuncRadius_Radius "radius"

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
 * EnrFrontLinearBranchFuncRadius
 * @author Erik Svenning
 * @date Feb 14, 2014
 */
class OOFEM_EXPORT EnrFrontLinearBranchFuncRadius : public EnrichmentFront
{
public:
    EnrFrontLinearBranchFuncRadius();
    virtual ~EnrFrontLinearBranchFuncRadius();

    void MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan, const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo) override;

    int giveNumEnrichments(const DofManager &iDMan) const override;
    int giveMaxNumEnrichments() const override { return 4; }

    // Evaluate the enrichment function and its derivative in front nodes.
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const override;
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const override;
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const override;

    const char *giveClassName() const override { return "EnrFrontLinearBranchFuncRadius"; }
    const char *giveInputRecordName() const override { return _IFT_EnrFrontLinearBranchFuncRadius_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    double giveSupportRadius() const override { return mEnrichmentRadius; }

private:
    double mEnrichmentRadius;
    LinElBranchFunction mpBranchFunc;
};
} // end namespace oofem

#endif /* ENRICHMENTFRONTLINBRANCHFUNCRADIUS_H_ */
