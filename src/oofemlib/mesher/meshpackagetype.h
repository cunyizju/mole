// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef meshpackagetype_h
#define meshpackagetype_h

#include "utility/enumitem.h"

namespace oofem {
#define MeshPackageType_DEF \
    ENUM_ITEM(MPT_T3D) \
    ENUM_ITEM(MPT_TARGE2) \
    ENUM_ITEM(MPT_FREEM) \
    ENUM_ITEM(MPT_SUBDIVISION)

/**
 * Enumerative type used to classify supported mesh packages.
 */
enum MeshPackageType {
    MeshPackageType_DEF
};

#undef ENUM_ITEM
#undef ENUM_ITEM_WITH_VALUE
#undef enumitem_h

const char *__MeshPackageTypeToString(MeshPackageType _value);
} // end namespace oofem
#endif // meshpackagetype_h
