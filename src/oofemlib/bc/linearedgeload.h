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

#ifndef linearedgeload_h
#define linearedgeload_h

#include "bc/boundaryload.h"
#include "math/floatarray.h"

///@name Input fields for LinearEdgeLoad
//@{
#define _IFT_LinearEdgeLoad_Name "linearedgeload"
#define _IFT_LinearEdgeLoad_formulation "formulation"
#define _IFT_LinearEdgeLoad_startcoord "sc"
#define _IFT_LinearEdgeLoad_endcoord "ec"
//@}

namespace oofem {
class TimeStep;

/**
 * This class implements a linear boundary load (force, moment,...) that acts
 * on straight segment.
 * A boundary load is usually attribute of one or more elements.
 *
 * The boundary load describes its geometry and values (it is assumed, that user will specify
 * all necessary dofs) on element boundary using isoparametric approximation.
 * Elements can request the order of approximation (for setting up the appropriate
 * integration rule order) and the array of values (for each dof) at specific integration point
 * on the boundary.
 *
 * Elements must take care, on which boundary the load acts on (side number, ...).
 *
 * @note{This class is not restricted to structural problems. For example, in thermal analysis,
 * a boundary load load would be a  heat source.}
 */
class OOFEM_EXPORT LinearEdgeLoad : public EdgeLoad
{
protected:
    /// Coordinates of start and end point
    FloatArray startCoords, endCoords;
    FormulationType formulation;

public:
    LinearEdgeLoad(int i, Domain * d) : EdgeLoad(i, d) { }
    virtual ~LinearEdgeLoad() { }

    int giveApproxOrder() override { return 1; }
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    bcGeomType giveBCGeoType() const override { return EdgeLoadBGT; }
    FormulationType giveFormulationType() override { return formulation; }

    const char *giveClassName() const override { return "LinearEdgeLoad"; }
    const char *giveInputRecordName() const override { return _IFT_LinearEdgeLoad_Name; }

protected:
    void computeNArray(FloatArray &answer, const FloatArray &coords) const override;
};
} // end namespace oofem
#endif // linearedgeload_h
