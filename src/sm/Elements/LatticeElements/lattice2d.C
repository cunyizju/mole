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
 *               Copyright (C) 1993 - 2019   Borek Patzak
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

#include "sm/Elements/LatticeElements/lattice2d.h"
#include "sm/Materials/LatticeMaterials/latticematstatus.h"
#include "sm/Elements/LatticeElements/latticestructuralelement.h"
#include "sm/CrossSections/latticecrosssection.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatmatrixf.h"
#include "math/intarray.h"
#include "math/floatarray.h"
#include "math/floatarrayf.h"
#include "math/mathfem.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(Lattice2d);

Lattice2d :: Lattice2d(int n, Domain *aDomain) : LatticeStructuralElement(n, aDomain)
{
    numberOfDofMans = 2;

    length = 0.;
    pitch = 10.;  // a dummy value
    couplingNumbers.zero();
}

Lattice2d :: ~Lattice2d()
{ }


int
Lattice2d :: giveCrackFlag()
{
    GaussPoint *gp = this->giveDefaultIntegrationRulePtr()->getIntegrationPoint(0);
    LatticeMaterialStatus *status = static_cast< LatticeMaterialStatus * >( gp->giveMaterialStatus() );
    return status->giveCrackFlag();
}


double
Lattice2d :: giveCrackWidth()
{
    GaussPoint *gp = this->giveDefaultIntegrationRulePtr()->getIntegrationPoint(0);
    LatticeMaterialStatus *status = static_cast< LatticeMaterialStatus * >( gp->giveMaterialStatus() );
    return status->giveCrackWidth();
}



double
Lattice2d :: giveDissipation()
{
    GaussPoint *gp = this->giveDefaultIntegrationRulePtr()->getIntegrationPoint(0);
    LatticeMaterialStatus *status = static_cast< LatticeMaterialStatus * >( gp->giveMaterialStatus() );
    return status->giveDissipation();
}


double
Lattice2d :: giveDeltaDissipation()
{
    GaussPoint *gp = this->giveDefaultIntegrationRulePtr()->getIntegrationPoint(0);
    LatticeMaterialStatus *status = static_cast< LatticeMaterialStatus * >( gp->giveMaterialStatus() );
    return status->giveDeltaDissipation();
}

void
Lattice2d :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
// Returns the strain matrix of the receiver.
{
    double l = this->giveLength();
    double x1, x2, y1, y2, xp, yp;

    //Coordinates of the nodes
    x1 = this->giveNode(1)->giveCoordinate(1);
    y1 = this->giveNode(1)->giveCoordinate(2);
    x2 = this->giveNode(2)->giveCoordinate(1);
    y2 = this->giveNode(2)->giveCoordinate(2);

    xp = this->gpCoords.at(1);
    yp = this->gpCoords.at(2);

    double ecc;

    double areaHelp = 0.5 * ( x1 * y2 + x2 * yp + xp * y1 - ( xp * y2 + yp * x1 + x2 * y1 ) );

    ecc = 2 * areaHelp / l;

    //    Assemble Bmatrix (used to compute strains and rotations
    answer.resize(3, 6);
    answer.zero();
    answer.at(1, 1) = -1.;
    answer.at(1, 2) = 0.;
    answer.at(1, 3) = ecc;
    answer.at(1, 4) = 1.;
    answer.at(1, 5) = 0.;
    answer.at(1, 6) = -ecc;

    answer.at(2, 1) = 0.;
    answer.at(2, 2) = -1.;
    answer.at(2, 3) =  -l / 2.;
    answer.at(2, 4) = 0.;
    answer.at(2, 5) = 1.;
    answer.at(2, 6) = -l / 2.;

    answer.at(3, 1) = 0.;
    answer.at(3, 2) = 0.;
    answer.at(3, 3) = -this->width / sqrt(12.);
    answer.at(3, 4) = 0.;
    answer.at(3, 5) = 0.;
    answer.at(3, 6) = this->width / sqrt(12.);

    answer.times(1. / l);
}

void
Lattice2d :: computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    answer = static_cast< LatticeCrossSection * >( this->giveCrossSection() )->give2dStiffnessMatrix(rMode, gp, tStep);
}

void
Lattice2d :: computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    answer = static_cast< LatticeCrossSection * >( this->giveCrossSection() )->giveLatticeStress2d(strain, gp, tStep);
}

void
Lattice2d :: computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode,
                                    TimeStep *tStep)
// Computes numerically the stiffness matrix of the receiver.
{
    double dV;
    FloatMatrix d, bj, dbj;
    answer.resize(6, 6);
    answer.zero();
    this->computeBmatrixAt(integrationRulesArray [ 0 ]->getIntegrationPoint(0), bj);
    this->computeConstitutiveMatrixAt(d, rMode, integrationRulesArray [ 0 ]->getIntegrationPoint(0), tStep);
    dV = this->computeVolumeAround(integrationRulesArray [ 0 ]->getIntegrationPoint(0) );
    dbj.beProductOf(d, bj);
    answer.plusProductUnsym(bj, dbj, dV);
}


void Lattice2d :: computeGaussPoints()
// Sets up the array of Gauss Points of the receiver.
{
    // the gauss point is used only when methods from crosssection and/or material
    // classes are requested
    integrationRulesArray.resize(1);
    integrationRulesArray [ 0 ] = std :: make_unique< GaussIntegrationRule >(1, this, 1, 3);
    integrationRulesArray [ 0 ]->SetUpPointsOnLine(1, _2dLattice);
}


bool
Lattice2d :: computeGtoLRotationMatrix(FloatMatrix &answer)
{
    double sine, cosine;
    answer.resize(6, 6);
    answer.zero();

    sine           = sin(this->givePitch() );
    cosine         = cos(pitch);
    answer.at(1, 1) =  cosine;
    answer.at(1, 2) =  sine;
    answer.at(2, 1) = -sine;
    answer.at(2, 2) =  cosine;
    answer.at(3, 3) =  1.;
    answer.at(4, 4) =  cosine;
    answer.at(4, 5) =  sine;
    answer.at(5, 4) = -sine;
    answer.at(5, 5) =  cosine;
    answer.at(6, 6) =  1.;
    return 1;
}


double
Lattice2d :: computeVolumeAround(GaussPoint *gp)
{
    double area = this->width * this->thickness;
    double weight  = gp->giveWeight();
    return weight * 0.5 * this->giveLength() * area;
}


void
Lattice2d :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    answer = {
        D_u, D_v, R_w
    };
}


double Lattice2d :: giveLength()
// Returns the length of the receiver.
{
    double dx, dy;
    Node *nodeA, *nodeB;

    if ( length == 0. ) {
        nodeA   = this->giveNode(1);
        nodeB   = this->giveNode(2);
        dx      = nodeB->giveCoordinate(1) - nodeA->giveCoordinate(1);
        dy      = nodeB->giveCoordinate(2) - nodeA->giveCoordinate(2);
        length  = sqrt(dx * dx + dy * dy);
    }

    return length;
}


double Lattice2d :: givePitch()
// Returns the pitch of the receiver.
{
    double xA, xB, yA, yB;
    Node *nodeA, *nodeB;

    if ( pitch == 10. ) {            // 10. : dummy initialization value
        nodeA  = this->giveNode(1);
        nodeB  = this->giveNode(2);
        xA     = nodeA->giveCoordinate(1);
        xB     = nodeB->giveCoordinate(1);
        yA     = nodeA->giveCoordinate(2);
        yB     = nodeB->giveCoordinate(2);
        pitch  = atan2(yB - yA, xB - xA);
    }

    return pitch;
}


double
Lattice2d :: giveNormalStress()
{
    LatticeMaterialStatus *status;

    IntegrationRule *iRule = this->giveDefaultIntegrationRulePtr();
    GaussPoint *gp = iRule->getIntegrationPoint(0);
    status = static_cast< LatticeMaterialStatus * >( gp->giveMaterialStatus() );
    double normalStress = 0;
    normalStress = status->giveNormalLatticeStress();

    return normalStress;
}

int
Lattice2d :: hasBeenUpdated()
{
    LatticeMaterialStatus *status;

    IntegrationRule *iRule = this->giveDefaultIntegrationRulePtr();
    GaussPoint *gp = iRule->getIntegrationPoint(0);
    status = static_cast< LatticeMaterialStatus * >( gp->giveMaterialStatus() );
    int updateFlag = 0;
    updateFlag = status->hasBeenUpdated();

    return updateFlag;
}


int
Lattice2d :: giveLocalCoordinateSystem(FloatMatrix &answer)
//
// returns a unit vectors of local coordinate system at element
// stored rowwise (mainly used by some materials with ortho and anisotrophy)
//
{
    double sine, cosine;

    answer.resize(3, 3);
    answer.zero();

    sine           = sin(this->givePitch() );
    cosine         = cos(pitch);

    answer.at(1, 1) = cosine;
    answer.at(1, 2) = sine;
    answer.at(2, 1) = -sine;
    answer.at(2, 2) = cosine;
    answer.at(3, 3) = 1.0;

    return 1;
}

void
Lattice2d :: initializeFrom(InputRecord &ir)
{
    LatticeStructuralElement :: initializeFrom(ir);

    IR_GIVE_OPTIONAL_FIELD(ir, thickness, _IFT_Lattice2d_thick);

    IR_GIVE_OPTIONAL_FIELD(ir, width, _IFT_Lattice2d_width);

    IR_GIVE_OPTIONAL_FIELD(ir, gpCoords, _IFT_Lattice2d_gpcoords);

    couplingFlag = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, couplingFlag, _IFT_Lattice2d_couplingflag);

    couplingNumbers.resize(1);
    couplingNumbers.zero();
    if ( couplingFlag == 1 ) {
        IR_GIVE_OPTIONAL_FIELD(ir, couplingNumbers.at(1), _IFT_Lattice2d_couplingnumber);
    }
}


int
Lattice2d :: computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords)
{
    answer.resize(3);
    answer.at(1) = this->gpCoords.at(1);
    answer.at(2) = this->gpCoords.at(2);

    return 1;
}


void
Lattice2d :: giveGpCoordinates(FloatArray &answer)
{
    answer.resize(3);
    answer.at(1) = this->gpCoords.at(1);
    answer.at(2) = this->gpCoords.at(2);
}


void Lattice2d :: saveContext(DataStream &stream, ContextMode mode)
{
    LatticeStructuralElement :: saveContext(stream, mode);

    if ( ( mode & CM_Definition ) ) {
        if ( !stream.write(width) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        if ( !stream.write(thickness) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        if ( !stream.write(couplingFlag) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        contextIOResultType iores;
        if ( ( iores = couplingNumbers.storeYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }

        if ( ( iores = gpCoords.storeYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }
    }
}


void Lattice2d :: restoreContext(DataStream &stream, ContextMode mode)
{
    LatticeStructuralElement :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.read(width) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        if ( !stream.read(thickness) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        if ( !stream.read(couplingFlag) ) {
            THROW_CIOERR(CIO_IOERR);
        }

        contextIOResultType iores;
        if ( ( iores = couplingNumbers.restoreYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }

        if ( ( iores = gpCoords.restoreYourself(stream) ) != CIO_OK ) {
            THROW_CIOERR(iores);
        }
    }
}


} // end namespace oofem
