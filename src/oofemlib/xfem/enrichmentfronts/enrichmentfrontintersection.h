// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "xfem/enrichmentfronts/enrichmentfront.h"
#include "math/floatarray.h"

#ifndef ENRICHMENTFRONTINTERSECTION_H_
 #define ENRICHMENTFRONTINTERSECTION_H_

 #define _IFT_EnrFrontIntersection_Name "enrfrontintersection"
 #define _IFT_EnrFrontIntersection_Tangent "tangent"

namespace oofem {
class XfemManager;
class DofManager;
class FloatArray;
class InputRecord;
class DynamicInputRecord;
class LinElBranchFunction;

/**
 * EnrFrontIntersection
 *
 * An enrichment front capable of handling crack intersections.
 *
 * @author Erik Svenning
 * @date Apr 23, 2014
 */
class EnrFrontIntersection : public EnrichmentFront
{
public:
    EnrFrontIntersection();
    virtual ~EnrFrontIntersection();

    void MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan,  const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo) override;

    int giveNumEnrichments(const DofManager &iDMan) const override;
    int giveMaxNumEnrichments() const override { return 1; }

    // Evaluate the enrichment function and its derivative in front nodes.
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const override;
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const override;
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const override;

    const char *giveClassName() const override { return "EnrFrontIntersection"; }
    const char *giveInputRecordName() const override { return _IFT_EnrFrontIntersection_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    double giveSupportRadius() const override { return 0.0; }
    bool propagationIsAllowed() const override { return false; }

    void setTangent(FloatArray iTangent) { mTangent = std :: move(iTangent); }

protected:
    FloatArray mTangent;
};
} /* namespace oofem */

#endif /* ENRICHMENTFRONTINTERSECTION_H_ */
