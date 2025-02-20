// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fields/field.h"

#include <cstdarg>

namespace oofem {
std :: string Field :: errorInfo(const char *func) const
{
    return std :: string(this->giveClassName()) + "::" + func;
}

} // end namespace oofem
