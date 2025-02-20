// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li



#include "voidprecond.h"

namespace oofem {
VoidPreconditioner :: VoidPreconditioner(const SparseMtrx &a, InputRecord &attributes) : Preconditioner(a, attributes)
{ }

VoidPreconditioner :: VoidPreconditioner() : Preconditioner()
{ }
} // end namespace oofem
