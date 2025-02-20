// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef HYBRIDEI_H_
#define HYBRIDEI_H_

#define _IFT_HybridEI_Name "hybridei"

#include "xfem/geometrybasedei.h"

namespace oofem {
class XfemManager;
class Domain;

/**
 * EnrichmentItem with hybrid geometry description in the following sense:
 * We have a BasicGeometry to describe the underlying geometry, and we use this
 * BasicGeometry to compute nodal level set fields. The enrichment is based on
 * interpolation of these nodal fields.
 * @author Erik Svenning
 * @date Sep 9, 2014
 */
class OOFEM_EXPORT HybridEI : public GeometryBasedEI
{
public:
    HybridEI(int n, XfemManager *xm, Domain *aDomain);
    virtual ~HybridEI();

    const char *giveClassName() const override { return "HybridEI"; }
    const char *giveInputRecordName() const override { return _IFT_HybridEI_Name; }

    void evalLevelSetNormal(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const override;
    void evalLevelSetTangential(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const override;
    void evalGradLevelSetNormal(FloatArray &oGradLevelSet, const FloatArray &iGlobalCoord, const FloatMatrix &idNdX, const IntArray &iNodeInd) const override;

    // By templating the function this way, we may choose if we want to pass iNodeInd as
    // an IntArray, a std::vector<int> or something else.
    // Any container that contains int and implements [] is legal.
    //    template< typename T >
    void interpLevelSet(double &oLevelSet, const FloatArray &iN, const IntArray &iNodeInd) const;

    void interpLevelSetTangential(double &oLevelSet, const FloatArray &iN, const IntArray &iNodeInd) const;

    void interpGradLevelSet(FloatArray &oGradLevelSet, const FloatMatrix &idNdX, const IntArray &iNodeInd) const;
};
} /* namespace oofem */

#endif /* HYBRIDEI_H_ */
