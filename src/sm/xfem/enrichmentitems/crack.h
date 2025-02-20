// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef CRACK_H_
#define CRACK_H_

#include "xfem/enrichmentitem.h"
#include "xfem/hybridei.h"

#define _IFT_Crack_Name "crack"

namespace oofem {
class XfemManager;
class Domain;
class InputRecord;
class GaussPoint;
class GnuplotExportModule;

/**
 * Crack.
 * @author Erik Svenning
 * @date Feb 14, 2014
 */
class OOFEM_EXPORT Crack : public HybridEI
{
public:
    Crack(int n, XfemManager *xm, Domain *aDomain);

    const char *giveClassName() const override { return "Crack"; }
    const char *giveInputRecordName() const override { return _IFT_Crack_Name; }
    void initializeFrom(InputRecord &ir) override;

    void AppendCohesiveZoneGaussPoint(GaussPoint *ipGP);
    void ClearCohesiveZoneGaussPoints() {mCohesiveZoneGaussPoints.clear(); mCohesiveZoneArcPositions.clear();}

    void callGnuplotExportModule(GnuplotExportModule &iExpMod, TimeStep *tStep) override;

    const std :: vector< GaussPoint * > &giveCohesiveZoneGaussPoints() const { return mCohesiveZoneGaussPoints; }
    const std :: vector< double > &giveCohesiveZoneArcPositions() const { return mCohesiveZoneArcPositions; }

    void computeCrackIntersectionPoints(Crack &iCrack, std :: vector< FloatArray > &oIntersectionPoints, std :: vector< double > &oArcPositions);
    void computeArcPoints(const std :: vector< FloatArray > &iIntersectionPoints, std :: vector< double > &oArcPositions);
    double computeLength();
    int giveDofPoolSize() const override;

protected:
    /**
     * Array of pointers to the Gauss points related to the
     * cohesive zone. The array is used for data extraction
     * and visualization only. The reason for keeping an array
     * of pointers here is as follows: the cohesive zone Gauss
     * points are created in the XFEMElementInterface, that
     * (of course) only keeps track of GPs in that element.
     * However, for visualization it is very valuable to be able
     * to plot cohesive zone data (e.g. damage or crack opening)
     * vs the arc length coordinate of the crack. This must be
     * accomplished at the level of the EnrichmentItem, because
     * here we know about the geometry of the crack.
     */
    std :: vector< GaussPoint * >mCohesiveZoneGaussPoints;
    std :: vector< double >mCohesiveZoneArcPositions;
};
} // end namespace oofem

#endif /* CRACK_H_ */
