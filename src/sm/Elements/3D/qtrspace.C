// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#include "sm/Elements/3D/qtrspace.h"
#include "sm/Materials/Structural/structuralms.h"
#include "sm/CrossSections/structuralcrosssection.h"
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
#include "fei/fei3dtetquad.h"
#include "engng/classfactory.h"

#include <cstdio>

namespace oofem {
REGISTER_Element(QTRSpace);

FEI3dTetQuad QTRSpace :: interpolation;

QTRSpace :: QTRSpace(int n, Domain *aDomain) : Structural3DElement(n, aDomain), ZZNodalRecoveryModelInterface(this)
{
    numberOfDofMans = 10;
}


void
QTRSpace :: initializeFrom(InputRecord &ir)
{
    numberOfGaussPoints = 4;
    Structural3DElement :: initializeFrom(ir);
}


FEInterpolation *
QTRSpace :: giveInterpolation() const
{
    return & interpolation;
}


Interface *
QTRSpace :: giveInterface(InterfaceType interface)
{
    if ( interface == ZZNodalRecoveryModelInterfaceType ) {
        return static_cast< ZZNodalRecoveryModelInterface * >(this);
    } else if ( interface == SPRNodalRecoveryModelInterfaceType ) {
        return static_cast< SPRNodalRecoveryModelInterface * >(this);
    } else if ( interface == NodalAveragingRecoveryModelInterfaceType ) {
        return static_cast< NodalAveragingRecoveryModelInterface * >(this);
    }

    OOFEM_LOG_INFO("Interface on QTRSpace element not supported");
    return NULL;
}

void
QTRSpace :: SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap)
{
    pap.resize(10);
    for ( int i = 1; i <= 10; i++ ) {
        pap.at(i) = this->giveNode(i)->giveNumber();
    }
}

void
QTRSpace :: SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap)
{
    int found = 0;
    answer.resize(1);

    for ( int i = 1; i <= 10; i++ ) {
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
QTRSpace :: SPRNodalRecoveryMI_giveNumberOfIP()
{
    return numberOfGaussPoints;
}

SPRPatchType
QTRSpace :: SPRNodalRecoveryMI_givePatchType()
{
    return SPRPatchType_3dBiQuadratic;
}


void
QTRSpace :: NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep)
{
    answer.clear();
    OOFEM_WARNING("IP values will not be transferred to nodes. Use ZZNodalRecovery instead (parameter stype 1)");
}

} // end namespace oofem
