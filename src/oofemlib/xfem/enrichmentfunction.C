// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfunction.h"
#include "math/gausspoint.h"
#include "math/mathfem.h"
#include "fei/feinterpol.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_EnrichmentFunction(DiscontinuousFunction)
REGISTER_EnrichmentFunction(HeavisideFunction)
REGISTER_EnrichmentFunction(RampFunction)

void EnrichmentFunction :: initializeFrom(InputRecord &ir)
{
}

void EnrichmentFunction :: giveInputRecord(DynamicInputRecord &input)
{
    FEMComponent :: giveInputRecord(input);
}

void DiscontinuousFunction :: evaluateEnrFuncAt(double &oEnrFunc, const FloatArray &iPos, const double &iLevelSet) const
{
    oEnrFunc = sgn(iLevelSet);
}

void HeavisideFunction :: evaluateEnrFuncDerivAt(FloatArray &oEnrFuncDeriv, const FloatArray &iPos, const double &iLevelSet, const FloatArray &iGradLevelSet) const
{
    oEnrFuncDeriv.resize(2);
    oEnrFuncDeriv.zero();
}

void HeavisideFunction :: evaluateEnrFuncAt(double &oEnrFunc, const FloatArray &iPos, const double &iLevelSet) const
{
    if ( iLevelSet > 0.0 ) {
        oEnrFunc = 1.0;
    } else {
        oEnrFunc = 0.0;
    }
}

void DiscontinuousFunction :: evaluateEnrFuncDerivAt(FloatArray &oEnrFuncDeriv, const FloatArray &iPos, const double &iLevelSet, const FloatArray &iGradLevelSet) const
{
    oEnrFuncDeriv.resize(2);
    oEnrFuncDeriv.zero();
}

void RampFunction :: evaluateEnrFuncAt(double &oEnrFunc, const FloatArray &iPos, const double &iLevelSet) const
{
    oEnrFunc = fabs(iLevelSet);
}

void RampFunction :: evaluateEnrFuncDerivAt(FloatArray &oEnrFuncDeriv, const FloatArray &iPos, const double &iLevelSet, const FloatArray &iGradLevelSet) const
{
    oEnrFuncDeriv.resize(2);
    oEnrFuncDeriv.zero();
    oEnrFuncDeriv.at(1) = iGradLevelSet.at(1) * sgn(iLevelSet);
    oEnrFuncDeriv.at(2) = iGradLevelSet.at(2) * sgn(iLevelSet);
}

void LinElBranchFunction :: evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const double &iR, const double &iTheta) const
{
    oEnrFunc.push_back( sqrt(iR) * sin(0.5 * iTheta) );
    oEnrFunc.push_back( sqrt(iR) * sin(0.5 * iTheta) * sin(iTheta) );
    oEnrFunc.push_back( sqrt(iR) * cos(0.5 * iTheta) );
    oEnrFunc.push_back( sqrt(iR) * cos(0.5 * iTheta) * sin(iTheta) );
}

void LinElBranchFunction :: evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const double &iR, const double &iTheta) const
{
    // Evaluate the enrichment function derivatives using the chain rule:
    // dPdx = dPdr*drdx + dPdt*dtdx
    // dPdy = dPdr*drdy + dPdt*dtdy

    const double drdx =  cos(iTheta);
    const double drdy =  sin(iTheta);
    const double dtdx = -( 1.0 / iR ) * sin(iTheta);
    const double dtdy =  ( 1.0 / iR ) * cos(iTheta);

    FloatArray dP;

    /*
     *      double dtdy = 0.0;
     *      double eps = 1.0e-12;
     *      if( fabs(cos(iTheta)) < eps )
     *      {
     *              dtdy = 0.0;
     *      }
     *      else
     *      {
     *              dtdy = 1.0/(iR*iR);
     *      }
     */
    // Psi 1
    const double dP1dr = ( 1.0 / ( 2.0 * sqrt(iR) ) ) * sin(0.5 * iTheta);
    const double dP1dt = 0.5 * sqrt(iR) * cos(0.5 * iTheta);
    oEnrFuncDeriv.push_back({ dP1dr *drdx + dP1dt *dtdx, dP1dr *drdy + dP1dt *dtdy });

    // Psi 2
    const double dP2dr = ( 1.0 / ( 2.0 * sqrt(iR) ) ) * sin(0.5 * iTheta) * sin(iTheta);
    const double dP2dt = 0.5 * sqrt(iR) * cos(0.5 * iTheta) * sin(iTheta) + sqrt(iR) * sin(0.5 * iTheta) * cos(iTheta);
    oEnrFuncDeriv.push_back({ dP2dr *drdx + dP2dt *dtdx, dP2dr *drdy + dP2dt *dtdy });

    // Psi 3
    const double dP3dr = ( 1.0 / ( 2.0 * sqrt(iR) ) ) * cos(0.5 * iTheta);
    const double dP3dt = -0.5 * sqrt(iR) * sin(0.5 * iTheta);
    oEnrFuncDeriv.push_back({ dP3dr *drdx + dP3dt *dtdx, dP3dr *drdy + dP3dt *dtdy });

    // Psi 4
    const double dP4dr = ( 1.0 / ( 2.0 * sqrt(iR) ) ) * cos(0.5 * iTheta) * sin(iTheta);
    const double dP4dt = -0.5 * sqrt(iR) * sin(0.5 * iTheta) * sin(iTheta) + sqrt(iR) * cos(0.5 * iTheta) * cos(iTheta);
    oEnrFuncDeriv.push_back({ dP4dr *drdx + dP4dt *dtdx, dP4dr *drdy + dP4dt *dtdy });
}

void LinElBranchFunction :: giveJump(std :: vector< double > &oJumps) const
{
    OOFEM_ERROR("The radius is needed to compute the jump for branch functions.")
}

void LinElBranchFunction :: giveJump(std :: vector< double > &oJumps, const double &iRadius) const
{
    /**
     * Psi1 is discontinuous with jump magnitude 2*sqrt(r), the others are continuous.
     */

    oJumps.clear();
    oJumps.push_back( 2.0 * sqrt(iRadius) );
    oJumps.push_back(0.0);
    oJumps.push_back(0.0);
    oJumps.push_back(0.0);
}

void CohesiveBranchFunction :: evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const double &iR, const double &iTheta) const
{
    oEnrFunc.push_back( pow(iR,mExponent) * sin(0.5 * iTheta) );
}

void CohesiveBranchFunction :: evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const double &iR, const double &iTheta) const
{
    // Evaluate the enrichment function derivatives using the chain rule:
    // dPdx = dPdr*drdx + dPdt*dtdx
    // dPdy = dPdr*drdy + dPdt*dtdy

    const double drdx =  cos(iTheta);
    const double drdy =  sin(iTheta);
    const double dtdx = -( 1.0 / iR ) * sin(iTheta);
    const double dtdy =  ( 1.0 / iR ) * cos(iTheta);

    FloatArray dP;

    // Psi 1
    const double dP1dr = mExponent*pow(iR, mExponent-1.0) * sin(0.5 * iTheta);
    const double dP1dt = 0.5 * pow(iR,mExponent) * cos(0.5 * iTheta);
    oEnrFuncDeriv.push_back({ dP1dr *drdx + dP1dt *dtdx, dP1dr *drdy + dP1dt *dtdy });
}

void CohesiveBranchFunction :: giveJump(std :: vector< double > &oJumps) const
{
    OOFEM_ERROR("The radius is needed to compute the jump for branch functions.")
}

void CohesiveBranchFunction :: giveJump(std :: vector< double > &oJumps, const double &iRadius) const
{
    /**
     * Psi1 is discontinuous with jump magnitude 2*sqrt(r), the others are continuous.
     */

    oJumps.clear();
    oJumps.push_back( 2.0 * pow(iRadius,mExponent) );
}

} // end namespace oofem
