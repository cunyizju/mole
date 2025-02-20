// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ENRICHMENTFRONTEXTEND_H_
#define ENRICHMENTFRONTEXTEND_H_

#define _IFT_EnrFrontExtend_Name "enrfrontextend"

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
 * EnrFrontExtend
 * @author Erik Svenning
 * @date Feb 14, 2014
 */
class OOFEM_EXPORT EnrFrontExtend : public EnrichmentFront
{
public:
    EnrFrontExtend() { }
    virtual ~EnrFrontExtend() { }

    void MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan,  const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo) override;

    // No special tip enrichments are applied with this model,
    // it only modifies the set of nodes subject to bulk enrichment.
    int giveNumEnrichments(const DofManager &iDMan) const override { return 0; }
    int giveMaxNumEnrichments() const override { return 0; }

    // Evaluate the enrichment function and its derivative in front nodes.
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const override { }
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const override { }
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const override { }


    const char *giveClassName() const override { return "EnrFrontExtend"; }
    const char *giveInputRecordName() const override { return _IFT_EnrFrontExtend_Name; }

    void initializeFrom(InputRecord &ir) override { }
    void giveInputRecord(DynamicInputRecord &input) override;

    double giveSupportRadius() const override { return 0.0; }
};
} // end namespace oofem




#endif /* ENRICHMENTFRONTEXTEND_H_ */
