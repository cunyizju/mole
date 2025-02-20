// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef iga_h
#define iga_h

#include "input/inputrecord.h"
#include "math/intarray.h"
#include "fei/feinterpol.h"
#include "math/gaussintegrationrule.h"

///@name Input fields for IGAElement
//@{
#define _IFT_IGAElement_KnotSpanParallelMode "knotspanparmode"
//@}

namespace oofem {

/**
 * Geometry wrapper for IGA elements.
 */
class OOFEM_EXPORT FEIIGAElementGeometryWrapper : public FEICellGeometry
{
public:
    const IntArray *knotSpan;
    Element *elem;
public:
    FEIIGAElementGeometryWrapper(Element *elem, const IntArray *knotSpan=nullptr) : FEICellGeometry(), knotSpan(knotSpan), elem(elem) { }

    int giveNumberOfVertices() const override { return elem->giveNumberOfNodes(); }
    const FloatArray &giveVertexCoordinates(int i) const override { return elem->giveNode(i)->giveCoordinates(); }
};


/**
 * IntegrationElement represent nonzero knot span, derived from Integration Rule.
 */
class OOFEM_EXPORT IGAIntegrationElement : public GaussIntegrationRule
{
protected:
    IntArray knotSpan;     // knot_span(nsd)
public:
    IGAIntegrationElement(int _n, Element *e, IntArray knotSpan) :
        GaussIntegrationRule(_n, e, 0, 0, false),
        knotSpan(std::move(knotSpan)) { }
    const IntArray *giveKnotSpan() override { return & this->knotSpan; }
    void setKnotSpan1(const IntArray &src) { this->knotSpan = src; }
};


/**
 * Implements base IGAElement, supposed to be a parent class of all elements with B-spline or NURBS based interpolation.
 */
class OOFEM_EXPORT IGAElement : public Element
{
protected:
#ifdef __PARALLEL_MODE
    IntArray knotSpanParallelMode;
#endif
public:
    IGAElement(int n, Domain * aDomain) : Element(n, aDomain) { }
    void initializeFrom(InputRecord &ir) override;

#ifdef __PARALLEL_MODE
    elementParallelMode giveKnotSpanParallelMode(int) const override;
#endif

protected:
    virtual int giveNsd() = 0; // this info is available also from interpolation. Do we need it here ???
};


/**
 * IGATSplineElement setups integration rules differently from IGAElement.
 */
class OOFEM_EXPORT IGATSplineElement : public IGAElement
{
public:
    IGATSplineElement(int n, Domain * aDomain) : IGAElement(n, aDomain) { }
    void initializeFrom(InputRecord &ir) override;
};
} // end namespace oofem
#endif //iga_h
