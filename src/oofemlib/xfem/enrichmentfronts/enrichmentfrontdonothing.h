// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ENRICHMENTFRONTDONOTHING_H_
#define ENRICHMENTFRONTDONOTHING_H_

#define _IFT_EnrFrontDoNothing_Name "enrfrontdonothing"

#include "oofemcfg.h"
#include <vector>
#include "input/inputrecord.h"
#include "xfem/enrichmentfronts/enrichmentfront.h"
#include "xfem/tipinfo.h"

namespace oofem {
class XfemManager;
class DofManager;
class FloatArray;
class InputRecord;
class DynamicInputRecord;

/**
 * EnrFrontDoNothing
 * @author Erik Svenning
 * @date Feb 14, 2014
 */
class OOFEM_EXPORT EnrFrontDoNothing : public EnrichmentFront
{
public:
    EnrFrontDoNothing(int iEIindex = 0) : EnrichmentFront(iEIindex) { }
    virtual ~EnrFrontDoNothing() { }

    void MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan, const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo) override { mTipInfo = iTipInfo; }

    // No special tip enrichments are applied with this model.
    int giveNumEnrichments(const DofManager &iDMan) const override { return 0; }
    int giveMaxNumEnrichments() const override { return 0; }

    // Evaluate the enrichment function and its derivative in front nodes.
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const override { }
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const override { }
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const override { }

    const char *giveClassName() const override { return "EnrFrontDoNothing"; }
    const char *giveInputRecordName() const override { return _IFT_EnrFrontDoNothing_Name; }

    void initializeFrom(InputRecord &ir) override { }
    void giveInputRecord(DynamicInputRecord &input) override;

    double giveSupportRadius() const override { return 0.0; }
};
} // end namespace oofem

#endif /* ENRICHMENTFRONTDONOTHING_H_ */
