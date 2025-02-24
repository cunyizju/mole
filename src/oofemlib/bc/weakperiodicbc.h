// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef WEAKPERIODICBC_H_
#define WEAKPERIODICBC_H_

#include <vector>
#include <iostream>
#include <memory>

#include "bc/activebc.h"
#include "input/inputrecord.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "math/gausspoint.h"

///@name Input fields for WeakPeriodicBoundaryCondition
//@{
#define _IFT_WeakPeriodicBoundaryCondition_Name "weakperiodicbc"
#define _IFT_WeakPeriodicBoundaryCondition_order "order"
#define _IFT_WeakPeriodicBoundaryCondition_descritizationType "descritizationtype"
#define _IFT_WeakPeriodicBoundaryCondition_dofids "dofids"
#define _IFT_WeakPeriodicBoundaryCondition_ngp "ngp"
#define _IFT_WeakPeriodicBoundaryCondition_gradient "gradient"
#define _IFT_WeakPeriodicBoundaryCondition_nlgeo "nlgeo"
#define _IFT_WeakPeriodicBoundaryCondition_elementSidesPositive "elementsidespositive"
#define _IFT_WeakPeriodicBoundaryCondition_elementSidesNegative "elementsidesnegative"
#define _IFT_WeakPeriodicBoundaryCondition_elementSidesPositiveSet "elementsidespositiveset"
#define _IFT_WeakPeriodicBoundaryCondition_elementSidesNegativeSet "elementsidesnegativeset"
//@}

namespace oofem {

class Node;
class Element;

enum basisType { monomial=0, trigonometric=1, legendre=2 };
/**
 * Imposes weak periodicity on the doftype of choice. 2D. It is required that the two edges are parallel and either horizontal or vertical.
 *
 * @author Carl Sandström
 */
class OOFEM_EXPORT WeakPeriodicBoundaryCondition : public ActiveBoundaryCondition
{
private:

    basisType useBasisType;
    int bcID;
    int orderOfPolygon;

    /** Contains prescribed gradient */
    FloatArray g;

    /** Direction of normal. 1 if normal in x, 2 if y and 3 if z. */
    int direction;

    /** Keeps info on which coordinates varies over the surface. Depends on number of spatial dimensions and normal direction */
    IntArray surfaceIndexes;

    FloatArray smax, smin;
    bool doUpdateSminmax;

    /** Number of Gausspoints used when integrating along the element edges */
    int ngp;

    /** Number of degrees of freedom (number of terms) */
    int ndof;

    /** Set containing positive side */
    int posSet;

    /** Set containing negative side */
    int negSet;

    /** ID of dofs on which weak periodicity is imposed */
    IntArray dofids;

    /** sideSign is the sign of the normal for each side */
    signed int sideSign [ 2 ];

    /** side[] keeps track of which side of the triangle is located along the boundary. element[] keeps track of what element is located along the boundary */
    std :: vector< int >side [ 2 ], element [ 2 ];

    /** Keeps track of which coordinate(s) are changing on the surface/edge */
    std :: vector< double >directions;

    void giveEdgeNormal(FloatArray &answer, int element, int side);

    void updateSminmax();

    void updateDirection();

    double computeBaseFunctionValue(int baseID, FloatArray coordinate);

    double computeBaseFunctionValue1D(int baseID, double coordinate);

    double computeBaseFunctionValue2D(int baseID, FloatArray coordinate);

    std :: unique_ptr< Node > gammaDman;
    IntArray gamma_ids;

    double factorial(int n);

    double binomial(double n, int k);

    /** Compute exponent for term n. Exponents i and j are x^i*y^j */
    void getExponents(int n, int &i, int &j);

    /** Compute orthogonal polynomial basis using Gram-Smidth process */
    void computeOrthogonalBasis();

    double computeProjectionCoefficient(int vIndex, int uIndex);

    /** gsMatrix contains coefficients for the Gram-Schmidt polynomials*/
    FloatMatrix gsMatrix;

    void computeDeformationGradient(FloatMatrix &answer, Element *e, FloatArray *lcoord, TimeStep *tStep);

    /** Number of terms in polynomial */
    int tcount;

    /** Number of dofIDs*/
    int ndofids;

    /** Use finite strains? */
    bool nlgeo;

public:
    WeakPeriodicBoundaryCondition(int n, Domain * d);
    virtual ~WeakPeriodicBoundaryCondition();

    void initializeFrom(InputRecord &ir) override;

    basisType giveBasisType() { return useBasisType; }

    void assemble(SparseMtrx &answer, TimeStep *tStep, CharType type,
                  const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, double scale = 1.0, void* lock=nullptr) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorm = nullptr, void* lock=nullptr) override;

    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep,
                                  CharType type, ValueModeType mode,
                                  const UnknownNumberingScheme &s, FloatArray *eNorm = nullptr, void* lock=nullptr);

    void giveExternalForcesVector(FloatArray &answer, TimeStep *tStep,
                                  CharType type, ValueModeType mode,
                                  const UnknownNumberingScheme &s, void*lock=nullptr);

    int giveNumberOfInternalDofManagers() override;

    DofManager *giveInternalDofManager(int i) override;

    void addElementSide(int elem, int side) override;

    const char *giveClassName() const override { return "WeakPeriodicBoundaryCondition"; }
    const char *giveInputRecordName() const override { return _IFT_WeakPeriodicBoundaryCondition_Name; }

protected:
    void computeElementTangent(FloatMatrix &answer, Element *e, int boundary, TimeStep *tStep);
};
}
#endif /* WEAKPERIODICBC_H_ */
