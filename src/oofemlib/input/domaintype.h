// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef domaintype_h
#define domaintype_h

#include "utility/enumitem.h"

namespace oofem {
#define domainType_DEF \
    ENUM_ITEM(_unknownMode) \
    ENUM_ITEM(_2dPlaneStressMode) \
    ENUM_ITEM(_PlaneStrainMode) \
    ENUM_ITEM(_2dPlaneStressRotMode) \
    ENUM_ITEM(_3dMode) \
    ENUM_ITEM(_3dAxisymmMode) \
    ENUM_ITEM(_2dMindlinPlateMode) \
    ENUM_ITEM(_3dDegeneratedShellMode) \
    ENUM_ITEM(_3dShellMode) \
    ENUM_ITEM(_2dTrussMode) \
    ENUM_ITEM(_1dTrussMode) \
    ENUM_ITEM(_2dBeamMode) \
    ENUM_ITEM(_HeatTransferMode) \
    ENUM_ITEM(_Mass1TransferMode) \
    ENUM_ITEM(_HeatMass1Mode) /* Coupled heat and mass (1 matter) transfer */ \
    ENUM_ITEM(_2dIncompressibleFlow) /* 2d Incompressible flow, no energy eq */ \
    ENUM_ITEM(_3dIncompressibleFlow) /* 3d Incompressible flow, no energy eq */ \
    ENUM_ITEM(_2dLatticeMode) \
    ENUM_ITEM(_2dLatticeMassTransportMode) \
    ENUM_ITEM(_3dLatticeMode) \
    ENUM_ITEM(_3dLatticeMassTransportMode) \
    ENUM_ITEM(_2dLatticeHeatTransferMode) \
    ENUM_ITEM(_3dLatticeHeatTransferMode) \
    ENUM_ITEM(_3dDirShellMode) /* 7 parameter shell based on director fields */ \
    ENUM_ITEM(_WarpingMode) /* free warping of a cross section */ \
    /**
     * Type representing type of domain.
     * Domain type (the member value of Domain class) is used to determine the default
     * number of DOFs per node and side and to determine their corresponding physical meaning.
     */
enum domainType {
    domainType_DEF
};

#undef ENUM_ITEM
#undef ENUM_ITEM_WITH_VALUE
#undef enumitem_h


const char *__domainTypeToString(domainType _value);
} // end namespace oofem
#endif // domaintype_h
