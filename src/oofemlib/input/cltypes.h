// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef cltypes_h
#define cltypes_h

#include "input/internalstatetype.h"
#include "input/internalstatevaluetype.h"
#include "input/valuemodetype.h"
#include "dofman/unknowntype.h"

namespace oofem {
/// @return Character corresponding to given ValueModeType.
char cltypesGiveUnknownTypeModeKey(ValueModeType mode);

/// @return The value type of corresponding InternalStateType
InternalStateValueType giveInternalStateValueType(InternalStateType type);
int giveInternalStateTypeSize(InternalStateValueType type);

// New /JB
InternalStateValueType giveInternalStateValueType(UnknownType type);
} // end namespace oofem
#endif // cltypes_h
