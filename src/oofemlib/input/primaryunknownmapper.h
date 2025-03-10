// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef primaryunknownmapper_h
#define primaryunknownmapper_h

#include "oofemcfg.h"
#include "utility/interface.h"
#include "input/valuemodetype.h"

#include <string>

namespace oofem {
class Domain;
class Element;
class TimeStep;
class FloatArray;
class IntArray;

/**
 * The base class for all primary unknowns mappers.
 * The basic task is to map the primary unknowns from one (old) mesh to the new one.
 * If this task requires the special element algorithms, these should be included using interface concept.
 *
 */
class OOFEM_EXPORT PrimaryUnknownMapper
{
public:
    /// Constructor
    PrimaryUnknownMapper() { }
    /// Destructor
    virtual ~PrimaryUnknownMapper() { }
    /**
     * Maps and updates the vector(s) of primary unknowns from old mesh oldd to new mesh newd.
     * The result is stored in answer array.
     * The interpolation of the primary unknowns is determined by element interpolation.
     * The physical meaning of primary unknowns is determined by DofManagers.
     * The ordering of unknowns in answer is determined by code numbers of
     * new mesh dofmanagers.
     * @param answer Resulting array with primary unknowns.
     * @param mode Determines the mode of unknown.
     * @param oldd Old mesh reference.
     * @param newd New mesh reference.
     * @param tStep Time step.
     * @return Nonzero if o.k.
     */
    virtual int mapAndUpdate(FloatArray &answer, ValueModeType mode,
                             Domain *oldd, Domain *newd,  TimeStep *tStep) = 0;
    /**
     * Evaluates the vector of primary unknowns, determined by domain, at given point.
     * The physical meaning of primary unknowns mapped is determined by background.
     * element containing given point.
     * @param answer Contains evaluated unknown vector.
     * @param dofMask Parameter containing dofIDs of mapped values.
     * @param mode Determines the type of mode of unknown.
     * @param oldd Old mesh reference (mesh with unknown field).
     * @param coords Coordinates of point of interest.
     * @param regList List of regions where to search, if empty all region search performed.
     * @param tStep Solution step.
     * @return Nonzero if o.k.
     */
    virtual int evaluateAt(FloatArray &answer, IntArray &dofMask, ValueModeType mode,
                           Domain *oldd, const FloatArray &coords, IntArray &regList, TimeStep *tStep) = 0;
protected:
    /// Returns string for prepending output (used by error reporting macros).
    std :: string errorInfo(const char *func) const;
};
} // end namespace oofem
#endif // primaryunknownmapper_h
