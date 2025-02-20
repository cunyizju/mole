// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ENRICHMENTFRONTREDUCEFRONT_H_
#define ENRICHMENTFRONTREDUCEFRONT_H_

#include "oofemcfg.h"
#include <vector>
#include "input/inputrecord.h"
#include "xfem/enrichmentfronts/enrichmentfront.h"
#include "xfem/tipinfo.h"

#define _IFT_EnrFrontReduceFront_Name "enrfrontreducefront"

namespace oofem {
class XfemManager;
class DofManager;
class FloatArray;
class InputRecord;
class DynamicInputRecord;

/**
 * EnrFrontReduceFront
 * @author Erik Svenning
 * @date Feb 14, 2014
 */
class OOFEM_EXPORT EnrFrontReduceFront : public EnrichmentFront
{
public:
    EnrFrontReduceFront() {};
    virtual ~EnrFrontReduceFront() {};

    void MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan, const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo) override;

    // No special tip enrichments are applied with this model,
    // it only modifies the set of nodes subject to bulk enrichment.
    int giveNumEnrichments(const DofManager &iDMan) const override { return 0; }
    int giveMaxNumEnrichments() const override { return 0; }

    // Evaluate the enrichment function and its derivative in front nodes.
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const override {}
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const override {}
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const override {}

    const char *giveClassName() const override { return "EnrFrontReduceFront"; }
    const char *giveInputRecordName() const override { return _IFT_EnrFrontReduceFront_Name; }

    void initializeFrom(InputRecord &ir) override { }
    void giveInputRecord(DynamicInputRecord &input) override;

    double giveSupportRadius() const override { return 0.0; }
};
} // end namespace oofem

#endif /* ENRICHMENTFRONTREDUCEFRONT_H_ */
