// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef eleminterpunknownmapper_h
#define eleminterpunknownmapper_h

#include "primaryunknownmapper.h"

namespace oofem {
class Domain;
class Element;
class TimeStep;

/**
 * The class implementing the primary unknown mapper using element interpolation functions.
 * The basic task is to map the primary unknowns from one (old) mesh to the new one.
 * This task requires the special element algorithms, these are to be included using interface concept.
 */
class OOFEM_EXPORT EIPrimaryUnknownMapper : public PrimaryUnknownMapper
{
public:
    /// Constructor
    EIPrimaryUnknownMapper();
    /// Destructor
    virtual ~EIPrimaryUnknownMapper() { }

    int mapAndUpdate(FloatArray &answer, ValueModeType mode,
                     Domain *oldd, Domain *newd,  TimeStep *tStep) override;
    int evaluateAt(FloatArray &answer, IntArray &dofMask, ValueModeType mode,
                   Domain *oldd, const FloatArray &coords, IntArray &regList, TimeStep *tStep) override;
};
} // end namespace oofem
#endif // eleminterpunknownmapper_h
