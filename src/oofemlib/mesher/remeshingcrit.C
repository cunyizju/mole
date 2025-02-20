// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mesher/remeshingcrit.h"
#include "error/errorestimator.h"

#ifdef __PARALLEL_MODE
 #include "parallel/problemcomm.h"
#endif

namespace oofem {
RemeshingCriteria :: RemeshingCriteria(int n, ErrorEstimator *e) : FEMComponent( n, e->giveDomain() )
{
    ee = e;
#ifdef __PARALLEL_MODE
    communicator = NULL;
    commBuff = NULL;
    initCommMap = true;
#endif
}

RemeshingCriteria :: ~RemeshingCriteria()
{
#ifdef __PARALLEL_MODE
    delete communicator;
    delete commBuff;
#endif
}
} // end namespace oofem
