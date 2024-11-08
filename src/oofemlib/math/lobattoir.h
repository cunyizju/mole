/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
