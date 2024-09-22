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

#include "sm/Elements/igaelements.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "input/element.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "material/matresponsemode.h"
#include "cs/crosssection.h"
#include "math/mathfem.h"
#include "iga/iga.h"
#include "classfactory.h"

namespace oofem {
REGISTER_Element(BsplinePlaneStressElement);
REGISTER_Element(NURBSPlaneStressElement);
REGISTER_Element(TSplinePlaneStressElement);
REGISTER_Element(NURBSSpace3dElement);


BsplinePlaneStressElement :: BsplinePlaneStressElement(int n, Domain *aDomain) : IGAElement(n, aDomain), PlaneStressStructuralElementEvaluator(), interpolation(2) { }


void BsplinePlaneStressElement :: initializeFrom(InputRecord &ir)
{
    //PlaneStressStructuralElementEvaluator::initializeFrom(ir);
    IGAElement :: initializeFrom(ir);
}


int BsplinePlaneStressElement :: checkConsistency()
{
    BSplineInterpolation *interpol = static_cast< BSplineInterpolation * >( this->giveInterpolation() );
    if ( giveNumberOfDofManagers() != interpol->giveNumberOfControlPoints(1) * interpol->giveNumberOfControlPoints(2) ) {
        OOFEM_WARNING("number of control points mismatch");
        return 0;
    }
    return 1;
}



NURBSPlaneStressElement :: NURBSPlaneStressElement(int n, Domain *aDomain) : IGAElement(n, aDomain), PlaneStressStructuralElementEvaluator(), interpolation(2) { }


void NURBSPlaneStressElement :: initializeFrom(InputRecord &ir)
{
    //PlaneStressStructuralElementEvaluator::initializeFrom(ir);
    IGAElement :: initializeFrom(ir);
}


int NURBSPlaneStressElement :: checkConsistency()
{
    NURBSInterpolation *interpol = static_cast< NURBSInterpolation * >( this->giveInterpolation() );
    if ( giveNumberOfDofManagers() != interpol->giveNumberOfControlPoints(1) * interpol->giveNumberOfControlPoints(2) ) {
        OOFEM_WARNING("number of control points mismatch");
        return 0;
    }
    return 1;
}


TSplinePlaneStressElement :: TSplinePlaneStressElement(int n, Domain *aDomain) : IGATSplineElement(n, aDomain), PlaneStressStructuralElementEvaluator(), interpolation(2) { }




NURBSSpace3dElement :: NURBSSpace3dElement(int n, Domain *aDomain) : IGAElement(n, aDomain), Space3dStructuralElementEvaluator(), interpolation(3) { }


void NURBSSpace3dElement :: initializeFrom(InputRecord &ir)
{
    //PlaneStressStructuralElementEvaluator::initializeFrom(ir);
    IGAElement :: initializeFrom(ir);
}


int NURBSSpace3dElement :: checkConsistency()
{
    NURBSInterpolation *interpol = static_cast< NURBSInterpolation * >( this->giveInterpolation() );
    if ( giveNumberOfDofManagers() != interpol->giveNumberOfControlPoints(1) * interpol->giveNumberOfControlPoints(2) * interpol->giveNumberOfControlPoints(3) ) {
        OOFEM_WARNING("number of control points mismatch");
        return 0;
    }
    return 1;
}

// HUHU should be implemented by IGA element (it is the same for Bspline NURBS and TSpline)
// however in such a case it should be generalized in terms of appropriately multiplying
// nseq for those integration elements which span more tham just a single knot span
// the reason is to ensure compatible division to quads over which scalar quantity is interpolated
// bilinearly !!!

} // end namespace oofem
