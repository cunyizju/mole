// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef doftype_h
#define doftype_h

#include "utility/enumitem.h"

namespace oofem {
#define dofType_DEF \
    ENUM_ITEM_WITH_VALUE(DT_master, 0) \
    ENUM_ITEM_WITH_VALUE(DT_simpleSlave, 1) \
    ENUM_ITEM_WITH_VALUE(DT_slave, 2) \
    ENUM_ITEM_WITH_VALUE(DT_active, 3)

/// Dof Type, determines the type of DOF created.
enum dofType {
    dofType_DEF
};

#undef ENUM_ITEM
#undef ENUM_ITEM_WITH_VALUE
#undef enumitem_h


const char *__dofTypeToString(dofType _value);
} // end namespace oofem
#endif // doftype_h
