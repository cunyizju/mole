// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "dofman/dof.h"
#include "dofman/dofmanager.h"
#include "solvers/timestep.h"
#include "bc/boundarycondition.h"
#include "bc/initialcondition.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"
#include "input/unknownnumberingscheme.h"

#include <cstring>
#include <cstdlib>
#include <cstdarg>

namespace oofem {

Dof :: Dof(DofManager *aNode, DofIDItem id)
{
    dofManager     = aNode;
    dofID          = id;
}

int Dof :: giveEquationNumber(const UnknownNumberingScheme &s)
{
    return s.giveDofEquationNumber(this);
}

void Dof :: giveEquationNumbers(IntArray &masterEqNumbers, const UnknownNumberingScheme &s)
{
    masterEqNumbers.resize(1);
    masterEqNumbers.at(1) = s.giveDofEquationNumber(this);
}

void Dof :: giveDofIDs(IntArray &masterDofIDs)
{
    masterDofIDs = {this->giveDofID()};
}

int Dof :: giveDofManNumber() const { return this->dofManager->giveNumber(); }

int Dof :: giveDofManGlobalNumber() const { return this->dofManager->giveGlobalNumber(); }

void Dof :: printSingleOutputAt(FILE *File, TimeStep *tStep, char ch, ValueModeType mode, double scale)
// Prints in the data file the unknown 'u' (for example, the displacement
// 'd') of the receiver, at tStep.
{
    double x = scale * this->giveUnknown(mode, tStep);
    fprintf(File, "  dof %-3d %c % .8e\n", dofID, ch, x);
}

void Dof :: printSingleOutputWithAdditionAt(FILE *File, TimeStep *tStep, char ch, ValueModeType mode, double addend)
// Prints in the data file the unknown 'u' (for example, the displacement
// 'd') of the receiver, at tStep.
{
    double x = addend + this->giveUnknown(mode, tStep);
    fprintf(File, "  dof %-3d %c % .8e\n", dofID, ch, x);
}

void Dof :: printMultipleOutputAt(FILE *File, TimeStep *tStep, char *ch,
                                  ValueModeType *mode, int nite)
// Prints in the data file the unknown 'u' (for example, the displacement
// 'd') of the receiver, at tStep.
{
    fprintf(File, "  dof %d", dofID);
    for ( int i = 1; i <= nite; i++ ) {
        double x = this->giveUnknown(mode [ i - 1 ], tStep);
        fprintf(File, "   %c % .8e", ch [ i - 1 ], x);
    }

    fprintf(File, "\n");
}


void Dof :: printYourself()
{
    printf( "dof %d  of %s %d :\n", dofID, dofManager->giveClassName(), dofManager->giveNumber() );
}


std :: string Dof :: errorInfo(const char *func) const
{
    return std::string(this->giveClassName()) + "::" + func + " id " + std::to_string(dofID) + ", of DofManager " + std::to_string(dofManager->giveNumber());
}


double
Dof :: giveBcValue(ValueModeType mode, TimeStep *tStep)
{
    if ( this->hasBc(tStep) ) {
        double rel = 0.0;
        if ( mode == VM_Incremental && tStep->isTheFirstStep() && hasIcOn(VM_Total) ) {
            rel = giveIc()->give(VM_Total);
            return this->giveBc()->give(this, VM_Total, tStep) - rel;
        } else {
            return this->giveBc()->give(this, mode, tStep) - rel;
        }
    } else {
        return 0.0;
    }
}

void
Dof :: saveContext(DataStream &stream, ContextMode mode)
{
    int _val = dofID;
    if ( !stream.write(_val) ) {
        THROW_CIOERR(CIO_IOERR);
    }
}


void
Dof :: restoreContext(DataStream &stream, ContextMode mode)
{
    // restore dofid
    int _val;
    if ( !stream.read(_val) ) {
        THROW_CIOERR(CIO_IOERR);
    }

    dofID = ( DofIDItem ) _val;
}

void
Dof :: giveUnknowns(FloatArray &masterUnknowns, ValueModeType mode, TimeStep *tStep)
{
    masterUnknowns.resize(1);
    masterUnknowns.at(1) = this->giveUnknown(mode, tStep);
}

void
Dof :: giveUnknowns(FloatArray &masterUnknowns, PrimaryField &field, ValueModeType mode, TimeStep *tStep)
{
    masterUnknowns.resize(1);
    masterUnknowns.at(1) = this->giveUnknown(field, mode, tStep);
}

void
Dof :: computeDofTransformation(FloatArray &masterContribs)
{
    masterContribs.resize(1);
    masterContribs.at(1) = 1.0;
}

void
Dof :: giveMasterDofManArray(IntArray &answer)
{
    answer.resize(1);
    answer.at(1) = this->giveDofManNumber();
}
} // end namespace oofem
