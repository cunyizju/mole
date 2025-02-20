// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "cs/emptycs.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_CrossSection(EmptyCS);

EmptyCS :: EmptyCS(int n, Domain *d) : CrossSection(n, d) { }

Material* EmptyCS::giveMaterial(IntegrationPoint* ip) const
{
    return nullptr;
}

} // end namespace oofem
