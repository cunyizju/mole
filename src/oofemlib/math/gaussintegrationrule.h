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

#ifndef gaussintegrationrule_h
#define gaussintegrationrule_h

#include "math/integrationrule.h"

#define _IFT_GaussIntegrationRule_Name "gauss"

namespace oofem {
class Element;

/**
 * Class representing Gaussian-quadrature integration rule.
 * The number of integration points and their coordinates and integration weights depends on
 * integration rule type (rule for integration in 1d, 2d, 3d) and required accuracy.
 * The positions and weights are determined by the minimum required of points to integrate a polynomial exactly (while the points are strictly within the domain)
 *
 * Tasks:
 * - Returning number of integration points used
 * - Returning requested integration point
 * - Updating itself
 * - Saving and restoring context
 *
 * @see GaussPoint
 */
class OOFEM_EXPORT GaussIntegrationRule : public IntegrationRule
{
public:
    /**
     * Constructor.
     * @param n Number associated with receiver.
     * @param e Element associated with receiver.
     * @param startIndx First component, for which rule applies.
     * @param endIndx Last component, for which rule applies.
     * @param dynamic Flag indicating that receiver can change.
     */
    GaussIntegrationRule(int n, Element * e, int startIndx, int endIndx, bool dynamic = false);
    GaussIntegrationRule(int n, Element * e);
    /// Destructor
    virtual ~GaussIntegrationRule();

    const char *giveClassName() const override { return "GaussIntegrationRule"; }
    IntegrationRuleType giveIntegrationRuleType() const override { return IRT_Gauss; }
    void initializeFrom(InputRecord &ir) override { }

    int getRequiredNumberOfIntegrationPoints(integrationDomain dType, int approxOrder) override;

    int SetUpPointsOnLine(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnTriangle(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnSquare(int nPoints, MaterialMode mode) override;
    int SetUpPointsOn3dDegShell(int nPointsXY, int nPointsZ, MaterialMode mode) override;
    int SetUpPointsOn3dDegShellLayers(int nPointsXY, int nPointsZ, MaterialMode mode, const FloatArray &layerThickness) override;
    int SetUpPointsOnCubeLayers(int nPoints1, int nPoints2, int nPointsDepth, MaterialMode mode, const FloatArray &layerThickness) override;
    int SetUpPointsOnCube(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnTetrahedra(int nPoints, MaterialMode mode) override;
    int SetUpPointsOnWedge(int nPointsTri, int nPointsDepth, MaterialMode mode) override;
    int SetUpPointsOnWedgeLayers(int nPointsTri, int nPointsDepth, MaterialMode mode, const FloatArray &layerThickness) override;
    int SetUpPointsOn2DEmbeddedLine(int nPoints, MaterialMode mode, const FloatArray &coord0, const FloatArray &coord1) override;

    static void giveTetCoordsAndWeights(int nPoints, FloatArray &coords_xi1, FloatArray &coords_xi2, FloatArray &coords_xi3, FloatArray &weights);
    static void giveTriCoordsAndWeights(int nPoints, FloatArray &coords_xi1, FloatArray &coords_xi2, FloatArray &weights);
    static void giveLineCoordsAndWeights(int nPoints, FloatArray &coords_xi, FloatArray &weights);
};
} // end namespace oofem
#endif // gaussintegrationrule_h
