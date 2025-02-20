// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef lobattoir_h
#define lobattoir_h

#include "math/integrationrule.h"

#define _IFT_LobattoIntegrationRule_Name "lobatto"

namespace oofem {
/**
 * Class representing Lobatto-quadrature integration rule.
 * The number of integration points and their coordinates and integration weights depends on
 * integration rule type (rule for integration in 1d, 2d, 3d) and required  accuracy.
 */
class OOFEM_EXPORT LobattoIntegrationRule : public IntegrationRule
{
public:
    /**
     * Constructor.
     * @param n Number associated with receiver
     * @param e Reference to engineering model.
     * @param startIndx First component, for which rule applies.
     * @param endIndx Last component, for which rule applies.
     * @param dynamic Flag indicating that receiver can change.
     */
    LobattoIntegrationRule(int n, Element * e, int startIndx, int endIndx, bool dynamic);
    LobattoIntegrationRule(int n, Element * e);
    /// Destructor
    virtual ~LobattoIntegrationRule();

    const char *giveClassName() const override { return "LobattoIntegrationRule"; }
    IntegrationRuleType giveIntegrationRuleType() const override { return IRT_Lobatto; }
    void initializeFrom(InputRecord &ir) override { }

    int getRequiredNumberOfIntegrationPoints(integrationDomain dType, int approxOrder) override;

    //@todo These 3 integration rules have not been verified but the code is identical
    // to that in GaussIntegrationRule, only the point coords and weights differ /JB
    int SetUpPointsOnLine(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnSquare(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnCube(int nPoints, MaterialMode mode) override;
    static void giveLineCoordsAndWeights(int nPoints, FloatArray &coords_xi, FloatArray &weights);
    int SetUpPointsOnTriangle(int nPoints, MaterialMode mode) override;
};
} // end namespace oofem
#endif // lobattoir_h
