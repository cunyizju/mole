// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/3D/ltrspaceboundarymembrane.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "dofman/node.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/mathfem.h"
#include "fei/fei3dtetlin.h"
#include "engng/classfactory.h"
#include "Materials/Structural/structuralms.h"


namespace oofem {
REGISTER_Element(LTRSpaceBoundaryMembrane);

LTRSpaceBoundaryMembrane :: LTRSpaceBoundaryMembrane(int n, Domain *aDomain) :
    LTRSpaceBoundary(n, aDomain)
{}

void
LTRSpaceBoundaryMembrane :: initializeFrom(InputRecord &ir)
{
    LTRSpaceBoundary :: initializeFrom(ir);
}

void
LTRSpaceBoundaryMembrane :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode == 5 ) {
        answer = { E_xx, E_xy, E_yx, E_yy };
    } else {
        answer = { D_u, D_v, D_w };
    }
}


void
LTRSpaceBoundaryMembrane :: computeTransformationMatrix(FloatMatrix &answer, TimeStep *tStep)
{
    FloatArray unitCellSize;
    unitCellSize.resize(2);
    unitCellSize.at(1) = this->giveNode(5)->giveCoordinate(1);
    unitCellSize.at(2) = this->giveNode(5)->giveCoordinate(2);

    IntArray switches1, switches2, switches3, switches4;
    this->giveSwitches(switches1, this->location.at(1) );
    this->giveSwitches(switches2, this->location.at(2) );
    this->giveSwitches(switches3, this->location.at(3) );
    this->giveSwitches(switches4, this->location.at(4) );

    FloatMatrix k1, k2, k3, k4;
    k1.resize(3, 4);
    k2.resize(3, 4);
    k3.resize(3, 4);
    k4.resize(3, 4);

    k1.at(1, 1) = unitCellSize.at(1) * switches1.at(1);
    k1.at(1, 2) = unitCellSize.at(2) * switches1.at(2);
    k1.at(2, 3) = unitCellSize.at(1) * switches1.at(1);
    k1.at(2, 4) = unitCellSize.at(2) * switches1.at(2);

    k2.at(1, 1) = unitCellSize.at(1) * switches2.at(1);
    k2.at(1, 2) = unitCellSize.at(2) * switches2.at(2);
    k2.at(2, 3) = unitCellSize.at(1) * switches2.at(1);
    k2.at(2, 4) = unitCellSize.at(2) * switches2.at(2);

    k3.at(1, 1) = unitCellSize.at(1) * switches3.at(1);
    k3.at(1, 2) = unitCellSize.at(2) * switches3.at(2);
    k3.at(2, 3) = unitCellSize.at(1) * switches3.at(1);
    k3.at(2, 4) = unitCellSize.at(2) * switches3.at(2);

    k4.at(1, 1) = unitCellSize.at(1) * switches4.at(1),
    k4.at(1, 2) = unitCellSize.at(2) * switches4.at(2);
    k4.at(2, 3) = unitCellSize.at(1) * switches4.at(1);
    k4.at(2, 4) = unitCellSize.at(2) * switches4.at(2);

    answer.resize(12, 12);
    answer.beUnitMatrix();
    answer.resizeWithData(12, 16);

    answer.assemble(k1, { 1, 2, 3 }, { 13, 14, 15, 16 });
    answer.assemble(k2, { 4, 5, 6 }, { 13, 14, 15, 16 });
    answer.assemble(k3, { 7, 8, 9 }, { 13, 14, 15, 16 });
    answer.assemble(k4, { 10, 11, 12 }, { 13, 14, 15, 16 });
}
} // end namespace oofem
