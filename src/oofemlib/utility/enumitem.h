// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef enumitem_h
#define enumitem_h

namespace oofem {
#ifndef ENUM_ITEM
 #define ENUM_ITEM(element) element,
 #define ENUM_ITEM_WITH_VALUE(element, val) element = val,
#endif
} // end namespace oofem
#endif // enumitem_h
