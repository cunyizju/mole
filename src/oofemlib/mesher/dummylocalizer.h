// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dummylocalizer_h
#define dummylocalizer_h

#include "mesher/spatiallocalizer.h"

#include <vector>

namespace oofem {
class IntArray;

/**
 * The dummy implementation of spatial localizer based on traversing the whole domain.
 * The basic task is to provide spatial information and localization for domain, to which receiver is associated.
 * Typical services include searching the closes node to give position, searching of an element containing given point, etc.
 * If special element algorithms required, these should be included using interface concept.
 */
class OOFEM_EXPORT DummySpatialLocalizer : public SpatialLocalizer
{
protected:
    std :: vector< IntArray >region_elements;
    bool initialized;

public:
    /// Constructor
    DummySpatialLocalizer(Domain * d) : SpatialLocalizer(d), region_elements(), initialized(false) { }
    /// Destructor
    virtual ~DummySpatialLocalizer() { }

    int init(bool force = false) override;

    Element *giveElementContainingPoint(const FloatArray &coords, const IntArray *regionList = nullptr) override;
    Element *giveElementClosestToPoint(FloatArray &lcoords, FloatArray &closest, const FloatArray &coords, int region = 0) override;
    GaussPoint *giveClosestIP(const FloatArray &coords, int region, bool iCohesiveZoneGP = false) override;
    void giveAllElementsWithIpWithinBox(elementContainerType &elemSet, const FloatArray &coords, const double radius) override;
    void giveAllNodesWithinBox(nodeContainerType &nodeList, const FloatArray &coords, const double radius) override;
    Node *giveNodeClosestToPoint(const FloatArray &coords, double maxDist) override;

    const char *giveClassName() const override { return "DummySpatialLocalizer"; }
};
} // end namespace oofem
#endif // dummylocalizer_h
