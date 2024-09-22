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

#ifndef dummycrosssection_h
#define dummycrosssection_h

#include "cs/crosssection.h"
#include "material/materialmode.h"
#include "material/matresponsemode.h"
#include "material/material.h"
#include "input/internalstatetype.h"
#include "input/internalstatevaluetype.h"
#include "utility/dictionary.h"
#include "input/crosssectextension.h"
#include "math/gausspoint.h"

///@name Input fields for CrossSection
//@{
#define _IFT_DummyCrossSection_Name "dummycs"
#define _IFT_DummyCrossSection_material "mat"
//@}

namespace oofem {

/**
 * Class representing dummy cross section, that forwards all requests to material model.
 *
 */
class OOFEM_EXPORT DummyCrossSection : public CrossSection
{
protected:
    int matNumber = 0;
public:
    DummyCrossSection(int n, Domain *d);
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool isCharacteristicMtrxSymmetric(MatResponseMode rMode) const override;
    int packUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *ip) override;
    int unpackAndUpdateUnknowns(DataStream &buff, TimeStep *tStep, GaussPoint *ip) override;
    int estimatePackSize(DataStream &buff, GaussPoint *ip) override;
    Material *giveMaterial(IntegrationPoint *ip) const override;
    const char *giveClassName() const override { return "DummyCrossSection"; }
    const char *giveInputRecordName() const override { return _IFT_DummyCrossSection_Name; }

};
} // end namespace oofem
#endif // dummycrosssection_h
