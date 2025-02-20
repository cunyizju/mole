// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mesher/unstructuredgridfield.h"
#include "fei/feinterpol.h"
namespace oofem {
FEI2dLineLin UnstructuredGridField::Cell::i1(1, 2);
FEI2dLineQuad UnstructuredGridField::Cell::i2(1, 2);
FEI2dTrLin UnstructuredGridField::Cell::i3(1, 2);
FEI2dTrQuad UnstructuredGridField::Cell::i4(1, 2);
FEI2dQuadLin UnstructuredGridField::Cell::i5(1, 2);
FEI2dQuadQuad UnstructuredGridField::Cell::i6(1, 2);
FEI3dTetLin UnstructuredGridField::Cell::i7;
FEI3dHexaLin UnstructuredGridField::Cell::i8;

FEInterpolation *UnstructuredGridField::Cell::interpTable[] = {
    & UnstructuredGridField::Cell::i1,
    & UnstructuredGridField::Cell::i2,
    & UnstructuredGridField::Cell::i3,
    & UnstructuredGridField::Cell::i4,
    & UnstructuredGridField::Cell::i5,
    & UnstructuredGridField::Cell::i6,
    & UnstructuredGridField::Cell::i7,
    & UnstructuredGridField::Cell::i8
};
} // end namespace oofem
