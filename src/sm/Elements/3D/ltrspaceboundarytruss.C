// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/3D/ltrspaceboundarytruss.h"
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
REGISTER_Element(LTRSpaceBoundaryTruss);

LTRSpaceBoundaryTruss :: LTRSpaceBoundaryTruss(int n, Domain *aDomain) :
    LTRSpaceBoundary(n, aDomain)
{}

void
LTRSpaceBoundaryTruss :: initializeFrom(InputRecord &ir)
{
    LTRSpaceBoundary :: initializeFrom(ir);
}

void
LTRSpaceBoundaryTruss :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    if ( inode == 5 ) {
        answer = { E_xx };
    } else {
        answer = { D_u, D_v, D_w };
    }
}


void
LTRSpaceBoundaryTruss :: computeTransformationMatrix(FloatMatrix &answer, TimeStep *tStep)
{
    double unitCellSize = this->giveNode(5)->giveCoordinate(1);

    IntArray switches1, switches2, switches3, switches4;
    this->giveSwitches(switches1, this->location.at(1) );
    this->giveSwitches(switches2, this->location.at(2) );
    this->giveSwitches(switches3, this->location.at(3) );
    this->giveSwitches(switches4, this->location.at(4) );

    FloatMatrix k;
    k.resize(12, 1);

    k.at(1, 1) = unitCellSize * switches1.at(1);
    k.at(4, 1) = unitCellSize * switches2.at(1);
    k.at(7, 1) = unitCellSize * switches3.at(1);
    k.at(10, 1) = unitCellSize * switches4.at(1);

    answer.resize(12, 12);
    answer.beUnitMatrix();
    answer.resizeWithData(12, 13);

    answer.assemble(k, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, { 13 });
}
} // end namespace oofem
