// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef GEOMETRYBASEDEI_H_
#define GEOMETRYBASEDEI_H_

#define _IFT_GeometryBasedEI_Name "geometrybasedei"

#include "enrichmentitem.h"
#include "input/geometry.h"

#include <memory>

namespace oofem {
class XfemManager;
class Domain;

/**
 * EnrichmentItem with geometry described by BasicGeometry
 * @author Erik Svenning
 * @date Sep 9, 2014
 */
class OOFEM_EXPORT GeometryBasedEI : public EnrichmentItem
{
public:
    GeometryBasedEI(int n, XfemManager *xm, Domain *aDomain);
    virtual ~GeometryBasedEI();

    int instanciateYourself(DataReader &dr) override;
    void postInitialize() override;  

    virtual void updateDofIdPool();

    void appendInputRecords(DynamicDataReader &oDR) override;

    const char *giveClassName() const override { return "GeometryBasedEI"; }
    const char *giveInputRecordName() const override { return _IFT_GeometryBasedEI_Name; }

    void updateGeometry() override;
    void updateNodeEnrMarker(XfemManager &ixFemMan) override;

    void updateLevelSets(XfemManager &ixFemMan);

    void evaluateEnrFuncInNode(std :: vector< double > &oEnrFunc, const Node &iNode) const override;

    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const FloatArray &iGlobalCoord, const FloatArray &iLocalCoord, int iNodeInd, const Element &iEl) const override;
    void evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const FloatArray &iGlobalCoord, const FloatArray &iLocalCoord, int iNodeInd, const Element &iEl, const FloatArray &iN, const IntArray &iElNodes) const override;

    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const FloatArray &iGlobalCoord, const FloatArray &iLocalCoord, int iNodeInd, const Element &iEl) const override;
    void evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const FloatArray &iGlobalCoord, const FloatArray &iLocalCoord, int iNodeInd, const Element &iEl, const FloatArray &iN, const FloatMatrix &idNdX, const IntArray &iElNodes) const override;

    // TODO: Consider moving this function to a separate Cohesive Zone Interface /ES
    void evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, int iNodeInd, GaussPoint &iGP, bool iGPLivesOnCurrentCrack) const;


    virtual void computeIntersectionPoints(std :: vector< FloatArray > &oIntersectionPoints, std :: vector< int > &oIntersectedEdgeInd, Element *element, std :: vector< double > &oMinDistArcPos) const;
    virtual void computeIntersectionPoints(std :: vector< FloatArray > &oIntersectionPoints, std :: vector< int > &oIntersectedEdgeInd, Element *element, const Triangle &iTri, std :: vector< double > &oMinDistArcPos) const;

    void writeVtkDebug() const override;

    void giveSubPolygon(std :: vector< FloatArray > &oPoints, const double &iXiStart, const double &iXiEnd) const;

    void propagateFronts(bool &oFrontsHavePropagated) override;
    bool giveElementTipCoord(FloatArray &oCoord, double &oArcPos, Element &iEl, const FloatArray &iElCenter) const override;

    void giveBoundingSphere(FloatArray &oCenter, double &oRadius) override;

    BasicGeometry *giveGeometry() { return mpBasicGeometry.get(); }
    void setGeometry(std :: unique_ptr< BasicGeometry > &&ipBasicGeometry) {mpBasicGeometry = std::move(ipBasicGeometry);}

protected:
    std :: unique_ptr< BasicGeometry > mpBasicGeometry;
};
} /* namespace oofem */

#endif /* GEOMETRYBASEDEI_H_ */
