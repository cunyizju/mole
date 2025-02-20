// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "xfem/inclusion.h"

#include "xfemmanager.h"
#include "input/element.h"
#include "fei/feinterpol.h"
#include "math/gausspoint.h"

#include "engng/classfactory.h"

#include <string>

namespace oofem {
REGISTER_EnrichmentItem(Inclusion)

Inclusion :: Inclusion(int n, XfemManager *xm, Domain *aDomain) :
    HybridEI(n, xm, aDomain),
    mpCrossSection(NULL)
{
    mpEnrichesDofsWithIdArray = {
        D_u, D_v, D_w
    };
}

Inclusion :: ~Inclusion()
{
    if ( mpCrossSection != NULL ) {
        mpCrossSection = NULL;
    }
}

bool Inclusion :: isMaterialModified(GaussPoint &iGP, Element &iEl, CrossSection * &opCS) const
{
    // Check if the point is located inside the inclusion

    FloatArray N;
    FEInterpolation *interp = iEl.giveInterpolation();
    interp->evalN( N, iGP.giveNaturalCoordinates(), FEIElementGeometryWrapper(& iEl) );

    const IntArray &elNodes = iEl.giveDofManArray();

    double levelSetGP = 0.0;
    interpLevelSet(levelSetGP, N, elNodes);

    if ( levelSetGP < 0.0 ) {
        opCS = mpCrossSection;
        return true;
    }

    return false;
}

void Inclusion :: initializeFrom(InputRecord &ir)
{
    EnrichmentItem :: initializeFrom(ir);

    int crossSectionIndex = 0;
    IR_GIVE_FIELD(ir, crossSectionIndex, _IFT_Inclusion_CrossSection);
    mpCrossSection = this->giveDomain()->giveCrossSection(crossSectionIndex);
}
} /* namespace oofem */
