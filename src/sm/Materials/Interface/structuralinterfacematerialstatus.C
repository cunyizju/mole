// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#include "structuralinterfacematerial.h"
#include "utility/contextioerr.h"

#include "sm/Materials/Structural/structuralmaterial.h"
#include "structuralinterfacematerialstatus.h"
#include "math/gausspoint.h"

namespace oofem {

StructuralInterfaceMaterialStatus :: StructuralInterfaceMaterialStatus(GaussPoint *g) :
    MaterialStatus(g)
{
    F = eye<3>();
    tempF = F;
}


void StructuralInterfaceMaterialStatus :: printOutputAt(FILE *file, TimeStep *tStep) const
{
    MaterialStatus :: printOutputAt(file, tStep);

    fprintf(file, "  jump ");
    for ( auto &val : this->jump ) {
        fprintf(file, " %.4e", val );
    }

    fprintf(file, "\n              traction ");
    for ( auto &val : this->traction ) {
        fprintf(file, " %.4e", val );
    }
    fprintf(file, "\n");
}


void StructuralInterfaceMaterialStatus :: updateYourself(TimeStep *tStep)
{
    MaterialStatus :: updateYourself(tStep);

    this->jump            = this->tempJump;
    this->traction        = this->tempTraction;
    this->firstPKTraction = this->tempFirstPKTraction;
    this->F               = this->tempF;
}


void StructuralInterfaceMaterialStatus :: initTempStatus()
{
    MaterialStatus :: initTempStatus();

    this->tempJump            = this->jump;
    this->tempTraction        = this->traction;
    this->tempFirstPKTraction = this->firstPKTraction;
    this->tempF               = this->F;
}


void
StructuralInterfaceMaterialStatus :: saveContext(DataStream &stream, ContextMode mode)
{
#if 0
    MaterialStatus :: saveContext(stream, mode);

    contextIOResultType iores;
    if ( ( iores = strainVector.storeYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }

    if ( ( iores = stressVector.storeYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }
#endif
}


void
StructuralInterfaceMaterialStatus :: restoreContext(DataStream &stream, ContextMode mode)
{
#if 0
    MaterialStatus :: restoreContext(stream, mode);

    contextIOResultType iores;
    if ( ( iores = strainVector.restoreYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }

    if ( ( iores = stressVector.restoreYourself(stream) ) != CIO_OK ) {
        THROW_CIOERR(iores);
    }
#endif
}

void StructuralInterfaceMaterialStatus :: copyStateVariables(const MaterialStatus &iStatus)
{
    const StructuralInterfaceMaterialStatus &structStatus = static_cast< const StructuralInterfaceMaterialStatus & >(iStatus);

    jump                    = structStatus.giveJump();
    traction                = structStatus.giveTraction();
    tempTraction            = structStatus.giveTempTraction();
    tempJump                = structStatus.giveTempJump();
    firstPKTraction         = structStatus.giveFirstPKTraction();
    tempFirstPKTraction     = structStatus.giveTempFirstPKTraction();
    F                       = structStatus.giveF();
    tempF                   = structStatus.giveTempF();
    mNormalDir              = structStatus.giveNormal();
}


void StructuralInterfaceMaterialStatus :: addStateVariables(const MaterialStatus &iStatus)
{
    OOFEM_ERROR("not implemented");
}
} // end namespace oofem
