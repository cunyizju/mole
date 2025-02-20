// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mmaclosestiptransfer_h
#define mmaclosestiptransfer_h

#include "material/materialmappingalgorithm.h"

#define _IFT_MMAClosestIPTransfer_Name "closest"

namespace oofem {
class Domain;
class Element;
class TimeStep;
class MaterialStatus;

/**
 * The class implements the closest integration point transfer of state variables.
 */
class OOFEM_EXPORT MMAClosestIPTransfer : public MaterialMappingAlgorithm
{
protected:
    GaussPoint *source;
    MaterialStatus *mpMaterialStatus;

public:
    /// Constructor
    MMAClosestIPTransfer();
    virtual ~MMAClosestIPTransfer() {}

    void __init(Domain *dold, IntArray &type, const FloatArray &coords, Set &sourceElemSet, TimeStep *tStep, bool iCohesiveZoneGP = false) override;

    void finish(TimeStep *tStep) override { }

    int __mapVariable(FloatArray &answer, const FloatArray &coords, InternalStateType type, TimeStep *tStep) override;

    int mapStatus(MaterialStatus &oStatus) const override;

    const char *giveClassName() const override { return "MMAClosestIPTransfer"; }
};
} // end namespace oofem
#endif // mmaclosestiptransfer_h
