// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#include "primaryunknownmapper.h"

#include <cstdarg>

namespace oofem {
std :: string PrimaryUnknownMapper :: errorInfo(const char *func) const
{
    return std :: string("PrimaryUnknownMapper::") + func;
}

} // end namespace oofem
