// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dofman/simpleslavedof.h"
#include "dofman/dofmanager.h"
#include "input/domain.h"
#include "export/datastream.h"
#include "input/entityrenumberingscheme.h"
#include "utility/contextioerr.h"

namespace oofem {
SimpleSlaveDof :: SimpleSlaveDof(DofManager *aNode, int master, DofIDItem id) : Dof(aNode, id)
{
    masterDofMngr = master;
    masterDofIndx = -1;
}


SimpleSlaveDof :: SimpleSlaveDof(DofManager *aNode, DofIDItem id) : Dof(aNode, id)
{
    masterDofMngr = -1;
    masterDofIndx = -1;
}


Dof *SimpleSlaveDof :: giveMasterDof() const
{
    // returns reference to master dof
    // checks dof compatibility and slave to slave references

    return dofManager->giveDomain()->giveDofManager(masterDofMngr)->giveDofWithID(this->dofID);
}

BoundaryCondition *SimpleSlaveDof :: giveBc()
// Returns the boundary condition the receiver is subjected to.
{
    return this->giveMasterDof()->giveBc();
}


int SimpleSlaveDof :: __giveEquationNumber() const
// Returns the number of the equation in the governing system of equations that corres-
// ponds to the receiver. The equation number is 0 if the receiver is
// subjected to a boundary condition, else it is n+1, where n is the
// equation number of the most recently numbered degree of freedom.
{
    return this->giveMasterDof()->__giveEquationNumber();
}

int SimpleSlaveDof :: __givePrescribedEquationNumber()
// Returns the number of the equation in the governing system of equations that corres-
// ponds to the receiver. The equation number is 0 if the receiver is
// subjected to a boundary condition, else it is n+1, where n is the
// equation number of the most recently numbered degree of freedom.
{
    return this->giveMasterDof()->__givePrescribedEquationNumber();
}

InitialCondition *SimpleSlaveDof :: giveIc()
// Returns the initial condition on the receiver. Not used.
{
    return this->giveMasterDof()->giveIc();
}


double SimpleSlaveDof :: giveUnknown(ValueModeType mode, TimeStep *tStep)
{
    return this->giveMasterDof()->giveUnknown(mode, tStep);
}

double SimpleSlaveDof :: giveUnknown(PrimaryField &field, ValueModeType mode, TimeStep *tStep)
{
    return this->giveMasterDof()->giveUnknown(field, mode, tStep);
}


bool SimpleSlaveDof :: hasBc(TimeStep *tStep)
// Returns True if the receiver is subjected to a boundary condition, else
// returns False. If necessary, reads the answer in the data file.
{
    return this->giveMasterDof()->hasBc(tStep);
}


bool SimpleSlaveDof :: hasIc()
// Returns True if the receiver is subjected to an initial condition,
// else returns False.
{
    return this->giveMasterDof()->hasIc();
}


bool SimpleSlaveDof :: hasIcOn(ValueModeType u)
// Returns True if the unknown 'u' (e.g., the displacement 'd') of the
// receiver is subjected to an initial condition, else returns False.
{
    return this->giveMasterDof()->hasIcOn(u);
}

int SimpleSlaveDof :: giveBcId()
{
    return this->giveMasterDof()->giveBcId();
}

int SimpleSlaveDof :: giveIcId()
{
    return this->giveMasterDof()->giveIcId();
}


double
SimpleSlaveDof :: giveBcValue(ValueModeType mode, TimeStep *tStep)
{
    return this->giveMasterDof()->giveBcValue(mode, tStep);
}


void SimpleSlaveDof :: saveContext(DataStream &stream, ContextMode mode)
{
    Dof :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {

        if ( mode & CM_DefinitionGlobal ) {
            int _masterGlobNum = dofManager->giveDomain()->giveDofManager(masterDofMngr)->giveGlobalNumber();
            if ( !stream.write(_masterGlobNum) ) {
                THROW_CIOERR(CIO_IOERR);
            }
        } else {
            if ( !stream.write(masterDofMngr) ) {
                THROW_CIOERR(CIO_IOERR);
            }
        }
    }
}


void SimpleSlaveDof :: restoreContext(DataStream &stream, ContextMode mode)
{
    Dof :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.read(masterDofMngr) ) {
            THROW_CIOERR(CIO_IOERR);
        }
    }

    this->masterDofIndx = -1;
}

void
SimpleSlaveDof :: updateLocalNumbering(EntityRenumberingFunctor &f)
{
    masterDofMngr = f(masterDofMngr, ERS_DofManager);
}
} // end namespace oofem
