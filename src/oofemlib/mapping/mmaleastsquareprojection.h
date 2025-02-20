// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mmaleastsquareprojection_h
#define mmaleastsquareprojection_h

#include "material/materialmappingalgorithm.h"
#include "utility/interface.h"

#include <list>

///@name Input fields for MMALeastSquareProjection
//@{
#define _IFT_MMALeastSquareProjection_Name "leastsquare"
#define _IFT_MMALeastSquareProjection_statefilter "mmalsp_statefilter"
#define _IFT_MMALeastSquareProjection_regionfilter "mmalsp_regionfilter"
//@}

namespace oofem {
class Domain;
class Element;
class TimeStep;
class DynamicInputRecord;

enum MMALeastSquareProjectionPatchType { MMALSPPatchType_1dq, MMALSPPatchType_2dq };
/*
 * Defines, whether only the necessary number of closest points will be used
 * to fit a polynomial. If not defined, more points from gp neighborhood will
 * be used, based on element connectivity.
 */
//#define MMALSP_ONLY_CLOSEST_POINTS

/**
 * The class implements the transfer of state variables based on
 * Least square fit over old mesh integration points (in the neighborhood of point of interest).
 *
 * The algorithm of projecting internal vars (q) can be summarized as follows:
 * -# The "source" element on old mesh containing point of interest is found
 * -# The element patch is constructed from "source" element neighbourhood
 * -# The least square fit is done
 * -# Value in point of interest is evaluated.
 *
 * It is obvious, that this mapper operates locally and therefore there is no need to declared this
 * mapper as static material member.
 *
 */
class OOFEM_EXPORT MMALeastSquareProjection : public MaterialMappingAlgorithm
{
protected:
    /// If set, then only IP in the neighbourhood with same state can be used to interpolate the values.
    int stateFilter;
    /// If set, then only IP in the same region are taken into account.
    int regionFilter;
    /// List of Gp participating in patch.
    std :: list< GaussPoint * >patchGPList;
    /// Patch domain.
    Domain *patchDomain;
    /// Type of patch.
    MMALeastSquareProjectionPatchType patchType;
public:
    /// Constructor
    MMALeastSquareProjection();
    /// Destructor
    virtual ~MMALeastSquareProjection();

    void __init(Domain *dold, IntArray &type, const FloatArray &coords, Set &sourceElemSet, TimeStep *tStep, bool iCohesiveZoneGP = false) override;

    void finish(TimeStep *tStep) override;

    int __mapVariable(FloatArray &answer, const FloatArray &coords, InternalStateType type, TimeStep *tStep) override;

    int mapStatus(MaterialStatus &oStatus) const override;

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    const char *giveClassName() const override { return "MMALeastSquareProjectionPatchType"; }
    const char *giveInputRecordName() const { return _IFT_MMALeastSquareProjection_Name; }

protected:
    void computePolynomialTerms(FloatArray &P, const FloatArray &coords, MMALeastSquareProjectionPatchType type);
    int giveNumberOfUnknownPolynomialCoefficients(MMALeastSquareProjectionPatchType regType);
};
} // end namespace oofem
#endif // mmaleastsquareprojection_h
