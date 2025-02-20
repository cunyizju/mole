// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "xfemtolerances.h"

namespace oofem {

double XfemTolerances :: sCharElLength = 0.0;

void XfemTolerances :: setCharacteristicElementLength(double iCharElLength)
{
    sCharElLength = iCharElLength;
}

double XfemTolerances :: giveRelLengthTolLoose()
{
    return sRelLengthTolLoose;
}

double XfemTolerances :: giveRelLengthTolTight()
{
    return sRelLengthTolTight;
}

} // end namespace oofem
