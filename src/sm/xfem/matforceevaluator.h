// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef MATFORCEEVALUATOR_H_
#define MATFORCEEVALUATOR_H_

namespace oofem {

class TipInfo;
class Domain;
class FloatArray;
class TimeStep;

/**
 * Evaluates material forces.
 *
 * Under development. Currently, only elastic material and traction free cracks are considered.
 *
 * @author Erik Svenning
 * @date Nov 12, 2014
 */
class MaterialForceEvaluator {
public:
    MaterialForceEvaluator();
    virtual ~MaterialForceEvaluator();

    void computeMaterialForce(FloatArray &oMatForce, Domain &iDomain, const TipInfo &iTipInfo, TimeStep *tStep, const double &iRadius);

    double computeWeightFunctionInPoint(const FloatArray &iCoord, const FloatArray &iTipCoord, const double &iRadius) const;
};

} /* namespace oofem */

#endif /* MATFORCEEVALUATOR_H_ */
