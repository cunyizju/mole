// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef LISTBASEDEI_H_
#define LISTBASEDEI_H_

#define _IFT_ListBasedEI_Name "listbasedei"
#define _IFT_ListBasedEI_list "list"

#include "enrichmentitem.h"

#include <vector>

namespace oofem {
class XfemManager;
class Domain;

/**
 * EnrichmentItem with geometry defined by a set of nodes to be enriched.
 * @author Erik Svenning
 * @date Sep 9, 2014
 */
class OOFEM_EXPORT ListBasedEI : public EnrichmentItem
{
public:
    ListBasedEI(int n, XfemManager *xm, Domain *aDomain);
    virtual ~ListBasedEI();

    const char *giveClassName() const override { return "ListBasedEI"; }
    const char *giveInputRecordName() const override { return _IFT_ListBasedEI_Name; }

    void updateGeometry() override;
    void propagateFronts(bool &oFrontsHavePropagated) override;
    virtual void initiateFronts(bool &oFrontsHavePropagated, IntArray &initiateDofMans);
    void updateNodeEnrMarker(XfemManager &ixFemMan) override;

    bool giveElementTipCoord(FloatArray &oCoord, double &oArcPos,  Element &iEl, const FloatArray &iElCenter) const override;

    void giveBoundingSphere(FloatArray &oCenter, double &oRadius) override { OOFEM_ERROR("Not implemented.") }

protected:
    std :: vector< int >dofManList;
    double xi;
    int setNumber;
};
} /* namespace oofem */

#endif /* LISTBASEDEI_H_ */
