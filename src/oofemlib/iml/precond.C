// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "precond.h"

namespace oofem {
Preconditioner :: Preconditioner(const SparseMtrx &a, InputRecord &attributes)
{
    this->initializeFrom(attributes);
    this->init(a);
};
} // end namespace oofem
