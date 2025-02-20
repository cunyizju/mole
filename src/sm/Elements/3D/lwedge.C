// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/3D/lwedge.h"
#include "sm/Materials/Structural/structuralms.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "fei/fei3dwedgelin.h"
#include "dofman/node.h"
#include "material/material.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/domain.h"
#include "input/cltypes.h"
#include "math/mathfem.h"
#include "engng/classfactory.h"

#include <cstdio>

namespace oofem {
REGISTER_Element(LWedge);

FEI3dWedgeLin LWedge :: interpolation;

LWedge :: LWedge(int n, Domain *aDomain) : Structural3DElement(n, aDomain), ZZNodalRecoveryModelInterface(this), SpatialLocalizerInterface(this)
{
    numberOfDofMans = 6;
}


void
LWedge :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 6;
    Structural3DElement :: initializeFrom(ir);
}


FEInterpolation * LWedge :: giveInterpolation() const { return & interpolation; }


Interface *
LWedge :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == NodalAveragingRecoveryModelInterfaceType ) {
        return static_cast< NodalAveragingRecoveryModelInterface * >(this);
    } else if ( interface == SpatialLocalizerInterfaceType ) {
        return static_cast< SpatialLocalizerInterface * >(this);
    }

    OOFEM_LOG_INFO("Interface on Lwedge element not supported");
    return NULL;
}

void
LWedge :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(6);
    for ( int i = 1; i <= 6; i++ ) {
        pap.at(i) = this->giveNode(i)->giveNumber();
    }
}

void
LWedge :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    int found = 0;
    answer.resize(1);

    for ( int i = 1; i <= 6; i++ ) {
        if ( this->giveNode(i)->giveNumber() == pap ) {
            found = 1;
        }
    }

    if ( found ) {
        answer.at(1) = pap;
    } else {
        OOFEM_ERROR("unknown node number %d", pap);
    }
}

int
LWedge :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return numberOfGaussPoints;
}


SPRPatchType
LWedge :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_3dBiLin;
}


void
LWedge :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep)
{
    answer.clear();
    OOFEM_WARNING("IP values will not be transferred to nodes. Use ZZNodalRecovery instead (parameter stype 1)");
}

} // end namespace oofem
