// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef materialmappingalgorithmtype_h
#define materialmappingalgorithmtype_h

#include "utility/enumitem.h"

namespace oofem {
#define MaterialMappingAlgorithmType_DEF \
    ENUM_ITEM(MMA_ClosestPoint) \
    ENUM_ITEM(MMA_LeastSquareProjection) \
    ENUM_ITEM(MMA_ShapeFunctionProjection)

/**
 * Enumerative type used to classify supported
 * MaterialMappingAlgorithms
 */
enum MaterialMappingAlgorithmType {
    MaterialMappingAlgorithmType_DEF
};

#undef ENUM_ITEM
#undef ENUM_ITEM_WITH_VALUE
#undef enumitem_h


const char *__MaterialMappingAlgorithmTypeToString(MaterialMappingAlgorithmType _value);
} // end namespace oofem
#endif // materialmappingalgorithmtype_h
