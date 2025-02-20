// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/3D/ltrspaceboundaryvoigt.h"
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
REGISTER_Element(LTRSpaceBoundaryVoigt);

LTRSpaceBoundaryVoigt :: LTRSpaceBoundaryVoigt(int n, Domain *aDomain) :
    LTRSpaceBoundary(n, aDomain)
{}

void
LTRSpaceBoundaryVoigt :: initializeFrom(InputRecord &ir)
{
    LTRSpaceBoundary :: initializeFrom(ir);
}

void
LTRSpaceBoundaryVoigt :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode == 5 ) {
        answer = { E_xx, E_yy, E_zz, G_yz, G_xz, G_xy };
    } else {
        answer = { D_u, D_v, D_w };
    }
}


void
LTRSpaceBoundaryVoigt :: computeTransformationMatrix(FloatMatrix &answer, TimeStep *tStep)
{
    FloatArray unitCellSize;
    unitCellSize.resize(3);
    unitCellSize.at(1) = this->giveNode(5)->giveCoordinate(1);
    unitCellSize.at(2) = this->giveNode(5)->giveCoordinate(2);
    unitCellSize.at(3) = this->giveNode(5)->giveCoordinate(3);

    IntArray switches1, switches2, switches3, switches4;
    this->giveSwitches(switches1, this->location.at(1) );
    this->giveSwitches(switches2, this->location.at(2) );
    this->giveSwitches(switches3, this->location.at(3) );
    this->giveSwitches(switches4, this->location.at(4) );

    FloatMatrix k21_node1, k22_node1, k21_node2, k22_node2, k21_node3, k22_node3, k21_node4, k22_node4;
    k21_node1.resize(3, 3);
    k22_node1.resize(3, 3);
    k21_node2.resize(3, 3);
    k22_node2.resize(3, 3);
    k21_node3.resize(3, 3);
    k22_node3.resize(3, 3);
    k21_node4.resize(3, 3);
    k22_node4.resize(3, 3);

    k21_node1.at(1, 1) = unitCellSize.at(1) * switches1.at(1);
    k21_node1.at(2, 2) = unitCellSize.at(2) * switches1.at(2);
    k21_node1.at(3, 3) = unitCellSize.at(3) * switches1.at(3);

    k22_node1.at(1, 2) = unitCellSize.at(3) * switches1.at(3);
    k22_node1.at(1, 3) = unitCellSize.at(2) * switches1.at(2);
    k22_node1.at(2, 1) = unitCellSize.at(3) * switches1.at(3);

    k21_node2.at(1, 1) = unitCellSize.at(1) * switches2.at(1);
    k21_node2.at(2, 2) = unitCellSize.at(2) * switches2.at(2);
    k21_node2.at(3, 3) = unitCellSize.at(3) * switches2.at(3);

    k22_node2.at(1, 2) = unitCellSize.at(3) * switches2.at(3);
    k22_node2.at(1, 3) = unitCellSize.at(2) * switches2.at(2);
    k22_node2.at(2, 1) = unitCellSize.at(3) * switches2.at(3);

    k21_node3.at(1, 1) = unitCellSize.at(1) * switches3.at(1);
    k21_node3.at(2, 2) = unitCellSize.at(2) * switches3.at(2);
    k21_node3.at(3, 3) = unitCellSize.at(3) * switches3.at(3);

    k22_node3.at(1, 2) = unitCellSize.at(3) * switches3.at(3);
    k22_node3.at(1, 3) = unitCellSize.at(2) * switches3.at(2);
    k22_node3.at(2, 1) = unitCellSize.at(3) * switches3.at(3);

    k21_node4.at(1, 1) = unitCellSize.at(1) * switches4.at(1);
    k21_node4.at(2, 2) = unitCellSize.at(2) * switches4.at(2);
    k21_node4.at(3, 3) = unitCellSize.at(3) * switches4.at(3);

    k22_node4.at(1, 2) = unitCellSize.at(3) * switches4.at(3);
    k22_node4.at(1, 3) = unitCellSize.at(2) * switches4.at(2);
    k22_node4.at(2, 1) = unitCellSize.at(3) * switches4.at(3);

    answer.resize(12, 12);
    answer.beUnitMatrix();
    answer.resizeWithData(12, 18);

    answer.assemble(k21_node1, { 1, 2, 3 }, { 13, 14, 15 });
    answer.assemble(k22_node1, { 1, 2, 3 }, { 16, 17, 18 });
    answer.assemble(k21_node2, { 4, 5, 6 }, { 13, 14, 15 });
    answer.assemble(k22_node2, { 4, 5, 6 }, { 16, 17, 18 });
    answer.assemble(k21_node3, { 7, 8, 9 }, { 13, 14, 15 });
    answer.assemble(k22_node3, { 7, 8, 9 }, { 16, 17, 18 });
    answer.assemble(k21_node4, { 10, 11, 12 }, { 13, 14, 15 });
    answer.assemble(k22_node4, { 10, 11, 12 }, { 16, 17, 18 });
}
} // end namespace oofem
