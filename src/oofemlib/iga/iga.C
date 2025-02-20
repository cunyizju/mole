// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/inputrecord.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "math/mathfem.h"
#include "iga.h"
#include "math/gausspoint.h"
#include "feitspline.h"



namespace oofem {
void IGAElement :: initializeFrom(InputRecord &ir)
{
    int indx = 0;
    numberOfGaussPoints = 1;
#ifdef __PARALLEL_MODE
    int numberOfKnotSpans = 0;

#endif

    Element :: initializeFrom(ir); // read nodes , material, cross section
    // set number of dofmanagers
    this->numberOfDofMans = dofManArray.giveSize();
    this->giveInterpolation()->initializeFrom(ir); // read geometry

    // generate individual IntegrationElements; one for each nonzero knot span
    int nsd = this->giveNsd();
    if ( nsd == 1 ) {
        //HUHU
    } else if ( nsd == 2 ) {
        int numberOfKnotSpansU = this->giveInterpolation()->giveNumberOfKnotSpans(1);
        int numberOfKnotSpansV = this->giveInterpolation()->giveNumberOfKnotSpans(2);
#ifdef __PARALLEL_MODE
        numberOfKnotSpans = numberOfKnotSpansU * numberOfKnotSpansV;
#endif
        const IntArray *knotMultiplicityU = this->giveInterpolation()->giveKnotMultiplicity(1);
        const IntArray *knotMultiplicityV = this->giveInterpolation()->giveKnotMultiplicity(2);
        const FloatArray *knotValuesU = this->giveInterpolation()->giveKnotValues(1);
        const FloatArray *knotValuesV = this->giveInterpolation()->giveKnotValues(2);

        FloatArray newgpcoords(2);
        IntArray knotSpan(2);

        int numberOfIntegrationRules = numberOfKnotSpansU * numberOfKnotSpansV;
        integrationRulesArray.resize( numberOfIntegrationRules );

        knotSpan.at(2) = -1;
        for ( int vi = 1; vi <= numberOfKnotSpansV; vi++ ) {
            double dv = knotValuesV->at(vi + 1) - knotValuesV->at(vi);
            knotSpan.at(2) += knotMultiplicityV->at(vi);

            knotSpan.at(1) = -1;
            for ( int ui = 1; ui <= numberOfKnotSpansU; ui++ ) {
                double du = knotValuesU->at(ui + 1) - knotValuesU->at(ui);
                knotSpan.at(1) += knotMultiplicityU->at(ui);

                integrationRulesArray [ indx ] = std::make_unique<IGAIntegrationElement>(indx, this, knotSpan);
                integrationRulesArray [ indx ]->SetUpPointsOnSquare(numberOfGaussPoints, _PlaneStress); // HUHU _PlaneStress, rectangle

                // remap local subelement gp coordinates into knot span coordinates and update integration weight
                for ( auto &gp: *integrationRulesArray [ indx ] ) {
                    const FloatArray &gpcoords = gp->giveNaturalCoordinates();

                    newgpcoords.at(1) = knotValuesU->at(ui) + du * ( gpcoords.at(1) / 2.0 + 0.5 );
                    newgpcoords.at(2) = knotValuesV->at(vi) + dv * ( gpcoords.at(2) / 2.0 + 0.5 );
                    gp->setNaturalCoordinates(newgpcoords);
                    gp->setWeight(gp->giveWeight() / 4.0 * du * dv);
                }

                indx++;
            }
        }
    } else if ( nsd == 3 ) {
        int numberOfKnotSpansU = this->giveInterpolation()->giveNumberOfKnotSpans(1);
        int numberOfKnotSpansV = this->giveInterpolation()->giveNumberOfKnotSpans(2);
        int numberOfKnotSpansW = this->giveInterpolation()->giveNumberOfKnotSpans(3);
#ifdef __PARALLEL_MODE
        numberOfKnotSpans = numberOfKnotSpansU * numberOfKnotSpansV * numberOfKnotSpansW;
#endif
        const IntArray *knotMultiplicityU = this->giveInterpolation()->giveKnotMultiplicity(1);
        const IntArray *knotMultiplicityV = this->giveInterpolation()->giveKnotMultiplicity(2);
        const IntArray *knotMultiplicityW = this->giveInterpolation()->giveKnotMultiplicity(3);
        const FloatArray *knotValuesU = this->giveInterpolation()->giveKnotValues(1);
        const FloatArray *knotValuesV = this->giveInterpolation()->giveKnotValues(2);
        const FloatArray *knotValuesW = this->giveInterpolation()->giveKnotValues(3);

        FloatArray newgpcoords(3);
        IntArray knotSpan(3);

        int numberOfIntegrationRules = numberOfKnotSpansU * numberOfKnotSpansV * numberOfKnotSpansW;
        integrationRulesArray.resize( numberOfIntegrationRules );

        knotSpan.at(3) = -1;
        for ( int wi = 1; wi <= numberOfKnotSpansW; wi++ ) {
            double dw = knotValuesW->at(wi + 1) - knotValuesW->at(wi);
            knotSpan.at(3) += knotMultiplicityW->at(wi);

            knotSpan.at(2) = -1;
            for ( int vi = 1; vi <= numberOfKnotSpansV; vi++ ) {
                double dv = knotValuesV->at(vi + 1) - knotValuesV->at(vi);
                knotSpan.at(2) += knotMultiplicityV->at(vi);

                knotSpan.at(1) = -1;
                for ( int ui = 1; ui <= numberOfKnotSpansU; ui++ ) {
                    double du = knotValuesU->at(ui + 1) - knotValuesU->at(ui);
                    knotSpan.at(1) += knotMultiplicityU->at(ui);

                    integrationRulesArray [ indx ] = std::make_unique<IGAIntegrationElement>(indx, this, knotSpan);
                    integrationRulesArray [ indx ]->SetUpPointsOnCube(numberOfGaussPoints, _3dMat);

                    // remap local subelement gp coordinates into knot span coordinates and update integration weight
                    for ( auto &gp: *integrationRulesArray [ indx ] ) {
                        const FloatArray &gpcoords = gp->giveNaturalCoordinates();

                        newgpcoords.at(1) = knotValuesU->at(ui) + du * ( gpcoords.at(1) / 2.0 + 0.5 );
                        newgpcoords.at(2) = knotValuesV->at(vi) + dv * ( gpcoords.at(2) / 2.0 + 0.5 );
                        newgpcoords.at(3) = knotValuesW->at(wi) + dw * ( gpcoords.at(3) / 2.0 + 0.5 );
                        gp->setNaturalCoordinates(newgpcoords);
                        gp->setWeight(gp->giveWeight() / 8.0 * du * dv * dw);
                    }

                    indx++;
                }
            }
        }
    } else {
        throw ValueInputException(ir, "Domain", "unsupported number of spatial dimensions");
    }
    
#ifdef __PARALLEL_MODE
    // read optional knot span parallel mode
    this->knotSpanParallelMode.resize(numberOfKnotSpans);
    // set Element_local as default
    for ( int i = 1; i <= numberOfKnotSpans; i++ ) {
        knotSpanParallelMode.at(i) = Element_local;
    }
    IR_GIVE_OPTIONAL_FIELD(ir, knotSpanParallelMode, _IFT_IGAElement_KnotSpanParallelMode);
#endif
}


#ifdef __PARALLEL_MODE
elementParallelMode
IGAElement :: giveKnotSpanParallelMode(int knotSpanIndex) const
{
    elementParallelMode emode = this->giveParallelMode();
    if ( emode == Element_remote ) {
        return Element_remote;
    } else if ( emode == Element_local ) {
        return ( elementParallelMode ) this->knotSpanParallelMode.at(knotSpanIndex + 1);
    } else {
        OOFEM_ERROR("Cannot determine elementParallelMode");
    }

    return Element_local; //to make compiler happy
}

#endif // __PARALLEL_MODE


// integration elements are setup in the same way as for IGAElement for now HUHU

void IGATSplineElement :: initializeFrom(InputRecord &ir)
{
    TSplineInterpolation *interpol = static_cast< TSplineInterpolation * >( this->giveInterpolation() );

    int indx = 0, numberOfGaussPoints = 1;

    Element :: initializeFrom(ir); // read nodes , material, cross section

    // set number of dofmanagers
    this->numberOfDofMans = dofManArray.giveSize();
    // set number of control points before initialization HUHU HAHA
    interpol->setNumberOfControlPoints(this->numberOfDofMans);
    this->giveInterpolation()->initializeFrom(ir); // read geometry


    // generate individual IntegrationElements; one for each nonzero knot span
    int nsd = giveNsd();
    if ( nsd == 2 ) {
        int numberOfKnotSpansU = this->giveInterpolation()->giveNumberOfKnotSpans(1);
        int numberOfKnotSpansV = this->giveInterpolation()->giveNumberOfKnotSpans(2);
        const IntArray *knotMultiplicityU = this->giveInterpolation()->giveKnotMultiplicity(1);
        const IntArray *knotMultiplicityV = this->giveInterpolation()->giveKnotMultiplicity(2);
        const FloatArray *knotValuesU = this->giveInterpolation()->giveKnotValues(1);
        const FloatArray *knotValuesV = this->giveInterpolation()->giveKnotValues(2);

        FloatArray newgpcoords(2);
        IntArray knotSpan(2);

        int numberOfIntegrationRules = numberOfKnotSpansU * numberOfKnotSpansV;
        integrationRulesArray.resize( numberOfIntegrationRules );

        knotSpan.at(2) = -1;
        for ( int vi = 1; vi <= numberOfKnotSpansV; vi++ ) {
            double dv = knotValuesV->at(vi + 1) - knotValuesV->at(vi);
            knotSpan.at(2) += knotMultiplicityV->at(vi);

            knotSpan.at(1) = -1;
            for ( int ui = 1; ui <= numberOfKnotSpansU; ui++ ) {
                double du = knotValuesU->at(ui + 1) - knotValuesU->at(ui);
                knotSpan.at(1) += knotMultiplicityU->at(ui);

                integrationRulesArray [ indx ] = std::make_unique<IGAIntegrationElement>(indx, this, knotSpan);
                integrationRulesArray [ indx ]->SetUpPointsOnSquare(numberOfGaussPoints, _PlaneStress); // HUHU _PlaneStress, rectangle

                // remap local subelement gp coordinates into knot span coordinates and update integration weight
                for ( auto &gp: *integrationRulesArray [ indx ] ) {
                    const auto &gpcoords = gp->giveNaturalCoordinates();

                    newgpcoords.at(1) = knotValuesU->at(ui) + du * ( gpcoords.at(1) / 2.0 + 0.5 );
                    newgpcoords.at(2) = knotValuesV->at(vi) + dv * ( gpcoords.at(2) / 2.0 + 0.5 );
                    gp->setNaturalCoordinates(newgpcoords);
                    gp->setWeight(gp->giveWeight() / 4.0 * du * dv);
                }

                indx++;
            }
        }
    } else {
        throw ValueInputException(ir, "Domain", "unsupported number of spatial dimensions");
    }
}

} // end namespace oofem
