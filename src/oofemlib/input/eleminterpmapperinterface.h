// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef eleminterpmapperinterface_h
#define eleminterpmapperinterface_h

#include "input/valuemodetype.h"
#include "utility/interface.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "error/error.h"

namespace oofem {
class TimeStep;

/**
 * The element interface class related to Element Interpolation Mappers.
 */
class OOFEM_EXPORT EIPrimaryUnknownMapperInterface : public Interface
{
public:
    EIPrimaryUnknownMapperInterface() : Interface() { }

    /**
     * Computes the element vector of primary unknowns at given point in the local coordinate system.
     * @see EIPrimaryUnknownMI_computePrimaryUnknownVectorAt
     * @param mode    Identifies mode of unknown (eg. total value or velocity of unknown).
     * @param tStep   Time step, when vector of unknowns is requested.
     * @param lcoords Local coordinates of point of interest.
     * @param answer  Vector of unknowns.
     * @return Nonzero if given point is in receiver volume otherwise zero
     */
    virtual void EIPrimaryUnknownMI_computePrimaryUnknownVectorAtLocal(ValueModeType mode,
                                                                       TimeStep *tStep, const FloatArray &lcoords, FloatArray &answer)
    { OOFEM_ERROR("Not implemented"); }
};
} // end namespace oofem
#endif // eleminterpmapperinterface_h
