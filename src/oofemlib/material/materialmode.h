// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef materialmode_h
#define materialmode_h

#include "utility/enumitem.h"

namespace oofem {
#define MaterialMode_DEF \
    ENUM_ITEM(_Unknown)   \
    ENUM_ITEM(_3dMat) \
    ENUM_ITEM(_3dMatGrad)  /* 3d model with gradient of internal variable */ \
    ENUM_ITEM(_1dMatGrad)  /* 1d model with gradient of internal variable */ \
    ENUM_ITEM(_PlaneStressGrad)  /* plane stress with gradient of internal variable */ \
    ENUM_ITEM(_PlaneStrainGrad)  /* plane strain with gradient of internal variable */ \
    ENUM_ITEM(_PlaneStress) \
    ENUM_ITEM(_PlaneStrain) \
    ENUM_ITEM(_2dPlate) \
    ENUM_ITEM(_2dPlateSubSoil) /* Subsoil mode for plates */ \
    ENUM_ITEM(_1dMat) \
    ENUM_ITEM(_2dBeam) \
    ENUM_ITEM(_3dBeam) \
    ENUM_ITEM(_3dShell) \
    ENUM_ITEM(_3dShellRot) \
    ENUM_ITEM(_3dDegeneratedShell) \
    ENUM_ITEM(_3dBeamSubSoil) /* Subsoil model for beams */ \
  \
    ENUM_ITEM(_PlateLayer) \
    ENUM_ITEM(_2dBeamLayer) \
    ENUM_ITEM(_PlaneStressRot) /* Plane stress with rotation around z */ \
  \
    ENUM_ITEM(_Fiber) \
    ENUM_ITEM(_3dMicroplane) \
    ENUM_ITEM(_3dInterface) \
    ENUM_ITEM(_2dInterface) \
    ENUM_ITEM(_1dInterface) \
  \
    ENUM_ITEM(_1dHeat) /* 1d heat or 1d mass*/ \
    ENUM_ITEM(_1dHeMo) /* 1d heat and mass (one component) transfer */ \
    ENUM_ITEM(_2dHeat) /* 2d heat or 2d mass */ \
    ENUM_ITEM(_2dHeMo) /* 2d heat and mass (one component) transfer */ \
    ENUM_ITEM(_3dHeat) /* 3d heat or 3d mass */ \
    ENUM_ITEM(_3dHeMo) /* 3d heat and mass (one component) transfer */ \
  \
    ENUM_ITEM(_2dFlow) \
    ENUM_ITEM(_2dAxiFlow) \
    ENUM_ITEM(_3dFlow) \
  \
    ENUM_ITEM(_2dUP) \
    ENUM_ITEM(_3dUP) \
    ENUM_ITEM(_2dUPV) \
    ENUM_ITEM(_3dUPV) \
  \
    ENUM_ITEM(_1dLattice) \
    ENUM_ITEM(_2dLattice)			\
    ENUM_ITEM(_3dLattice) \
    ENUM_ITEM(_2dMTLattice) \
    ENUM_ITEM(_3dMTLattice) \
    ENUM_ITEM(_Warping) \

/**
 * Type representing material mode of integration point.
 */
enum MaterialMode {
    MaterialMode_DEF
};

#undef ENUM_ITEM
#undef ENUM_ITEM_WITH_VALUE
#undef enumitem_h


const char *__MaterialModeToString(MaterialMode _value);
} // end namespace oofem
#endif // materialmode_h
