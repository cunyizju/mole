// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mmacontainingelementprojection_h
#define mmacontainingelementprojection_h

#include "material/materialmappingalgorithm.h"

namespace oofem {
class Domain;
class Element;
class TimeStep;

/**
 * The class implements the transfer of internal variables based on containing element
 * The element containing the receiving Gauss point is localized and its closest Gauss point
 * is used as a source Gauss point that determines the values of receiver.
 */
class OOFEM_EXPORT MMAContainingElementProjection : public MaterialMappingAlgorithm
{
protected:
    GaussPoint *source;

public:
    /// Constructor
    MMAContainingElementProjection();

    void __init(Domain *dold, IntArray &type, const FloatArray &coords, Set &sourceElemSet, TimeStep *tStep, bool iCohesiveZoneGP = false) override;

    void finish(TimeStep *tStep) override { }

    int __mapVariable(FloatArray &answer, const FloatArray &coords, InternalStateType type, TimeStep *tStep) override;

    int mapStatus(MaterialStatus &oStatus) const override;

    const char *giveClassName() const override { return "MMAContainingElementProjection"; }
};
} // end namespace oofem
#endif // mmacontainingelementprojection_h
