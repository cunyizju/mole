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

#ifndef deadweight_h
#define deadweight_h

#include "bc/bodyload.h"
#include "bc/bcgeomtype.h"
#include "input/valuemodetype.h"

#define _IFT_DeadWeight_Name "deadweight"

namespace oofem {
/**
 * This class implements a gravity-like load, or internal source (heat etc.) for transport problems.
 * The inherited attribute 'componentArray' contains the components of an
 * loading prescribed per unit volume.
 *
 * Its task is to return the body force @f$ \rho a @f$
 */
class OOFEM_EXPORT DeadWeight : public BodyLoad
{
public:
    /// Constructor
    DeadWeight(int i, Domain * d) : BodyLoad(i, d) { }
    /**
     * Computes components values of deadweight field at given point (coordinates given in Global c.s.).
     * taking into account corresponding load time function value while respecting load response mode.
     * @param answer Component values at given point and time.
     * @param tStep Time step.
     * @param coords Global coordinates, which are used to evaluate components values.
     * @param mode Determines response mode-
     */
    void computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode) override;

    bcValType giveBCValType() const override { return ForceLoadBVT; }
    bcGeomType giveBCGeoType() const override { return BodyLoadBGT; }

    void setDeadWeighComponents(FloatArray newComponents);

    const char *giveClassName() const override { return "DeadWeight"; }
    const char *giveInputRecordName() const override { return _IFT_DeadWeight_Name; }
};
} // end namespace oofem
#endif // deadweight_h
