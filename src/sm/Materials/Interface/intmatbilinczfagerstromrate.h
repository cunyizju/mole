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

#ifndef intmatbilinearczmaterialFagerstromRate_h
#define intmatbilinearczmaterialFagerstromRate_h

#include "structuralinterfacematerial.h"
#include "structuralinterfacematerialstatus.h"
#include "intmatbilinczfagerstrom.h"
#include "input/dynamicinputrecord.h"

///@name Input fields for IntMatBilinearCZFagerstrom
//@{
#define _IFT_IntMatBilinearCZFagerstromRate_Name "intmatbilinearczfagerstromrate"
#define _IFT_IntMatBilinearCZFagerstromRate_kn "kn"
#define _IFT_IntMatBilinearCZFagerstromRate_ks "ks"
#define _IFT_IntMatBilinearCZFagerstromRate_knc "knc"
#define _IFT_IntMatBilinearCZFagerstromRate_g1c "g1c"
#define _IFT_IntMatBilinearCZFagerstromRate_g2c "g2c"
#define _IFT_IntMatBilinearCZFagerstromRate_mu "mu"
#define _IFT_IntMatBilinearCZFagerstromRate_gamma "gamma"
#define _IFT_IntMatBilinearCZFagerstromRate_sigf "sigf"
#define _IFT_IntMatBilinearCZFagerstromRate_cstar "c_star"
#define _IFT_IntMatBilinearCZFagerstromRate_m "m"

//@}

namespace oofem {


/**
 * Simple isotropic damage based model for 2d interface elements with rate dependence.
 * In 2d, the interface elements are used to model contact layer between
 * element edges. The generalized strain vector contains two relative displacements
 * (in normal and shear direction). The generalized stress vector contains corresponding
 * tractions in normal and tangent direction.
 *
 * The behaviour of the model is elastic, described by normal and shear stiffness components.
 * Isotropic damage is initiated  when the stress reaches the tensile strength. Damage evolution
 * is governed by normal component of generalized strain vector (normal relative displacement)
 * by an exponential softening law.
 */
class IntMatBilinearCZFagerstromRate : public IntMatBilinearCZFagerstrom
{
protected:
    /// Rate dependence coefficient
    double c_star = 0.;
    /// Rate dependence exponent
    double m = 0.;
    int checkConsistency() override;

public:
    /// Constructor
    IntMatBilinearCZFagerstromRate(int n, Domain *d);

    FloatArrayF<3> giveFirstPKTraction_3d(const FloatArrayF<3> &jump, const FloatMatrixF<3,3> &F, GaussPoint *gp, TimeStep *tStep) const override;
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    void printYourself() override;
    bool hasAnalyticalTangentStiffness() const override { return true; }
};
} // end namespace oofem
#endif // isointerfacedamage01_h
