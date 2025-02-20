// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef DIRECTEI_H_
#define DIRECTEI_H_

#define _IFT_DirectEI_Name "directei"

#include "xfem/geometrybasedei.h"

namespace oofem {
class XfemManager;
class Domain;

/**
 * EnrichmentItem with direct geometry description in the following sense:
 * We have a BasicGeometry to describe the underlying geometry, and we use this
 * BasicGeometry directly to compute level set fields.
 * @author Erik Svenning
 * @date Sep 10, 2014
 */
class DirectEI : public GeometryBasedEI
{
public:
    DirectEI(int n, XfemManager *xm, Domain *aDomain);
    virtual ~DirectEI();

    const char *giveClassName() const override { return "DirectEI"; }
    const char *giveInputRecordName() const override { return _IFT_DirectEI_Name; }

    void evalLevelSetNormal(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const override;
    void evalLevelSetTangential(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const override;
    void evalGradLevelSetNormal(FloatArray &oGradLevelSet, const FloatArray &iGlobalCoord, const FloatMatrix &idNdX, const IntArray &iNodeInd) const override;
};
} /* namespace oofem */

#endif /* DIRECTEI_H_ */
