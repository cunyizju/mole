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

#ifndef interpolatingfunction_h
#define interpolatingfunction_h

#include "func/function.h"

///@name Input fields for InterpolatingFuction
//@{
#define _IFT_InterpolatingFuction_Name "interpolatingfunction"
#define _IFT_InterpolatingFuction_filename "name"
#define _IFT_InterpolatingFuction_dim "dim"
//@}

namespace oofem {
/**
 * This class implements a function which reads
 * an externally generated field interpolates
 */
class OOFEM_EXPORT InterpolatingFuction : public Function
{
protected:
    FloatArray field;
    IntArray numberReal;
    int dimension;

public:
    /// Constructor
    InterpolatingFuction(int n, Domain * d);
    /// Destructor
    virtual ~InterpolatingFuction();

    void evaluate(FloatArray &answer, const std :: map< std :: string, FunctionArgument > &valDict, GaussPoint *gp=nullptr, double param=0.) override;
    double evaluateAtTime(double t) override;
    double evaluateVelocityAtTime(double t) override { return 0.; }
    double evaluateAccelerationAtTime(double t) override { return 0.; }

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "InterpolatingFuction"; }
    const char *giveInputRecordName() const override { return _IFT_InterpolatingFuction_Name; }
};
} // end namespace oofem

#endif // interpolatingfunction_h
