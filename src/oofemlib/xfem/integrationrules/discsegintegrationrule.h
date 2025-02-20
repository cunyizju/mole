// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef DISCSEGINTEGRATIONRULE_H_
#define DISCSEGINTEGRATIONRULE_H_

#include "math/gaussintegrationrule.h"

#include "input/geometry.h"

namespace oofem {
/**
 * DiscontinuousSegmentIntegrationRule provides integration over a
 * discontinuous boundary segment.
 *
 * @author Erik Svenning
 * @date Mar 14, 2014
 */
class OOFEM_EXPORT DiscontinuousSegmentIntegrationRule : public GaussIntegrationRule
{
protected:
    std :: vector< Line >mSegments;

    /// Start and end points of the boundary segment.
//    FloatArray mXS, mXE;

public:
    DiscontinuousSegmentIntegrationRule(int n, Element *e, const std :: vector< Line > &iSegments);
    virtual ~DiscontinuousSegmentIntegrationRule();

    int SetUpPointsOnLine(int iNumPointsPerSeg, MaterialMode mode) override;
};
} /* namespace oofem */

#endif /* DISCSEGINTEGRATIONRULE_H_ */
