// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef quasicontinuum_h
#define quasicontinuum_h

#include "math/floatarray.h"
#include "input/element.h"
#include "dofman/node.h"
#include "dofman/qcnode.h"

namespace oofem {
/**
 * General simplification for Quasicontinuum simulation.
 */
class Quasicontinuum 
{
protected:
    std::vector<IntArray> interpolationMeshNodes;
    IntArray interpolationElementNumbers;
    IntArray interpolationElementIndices;
    std::vector<IntArray> connectivityTable;
    int nDimensions;
    IntArray elemList;
    IntArray nodeList;

public:
    Quasicontinuum();
    virtual ~Quasicontinuum();

    void setNoDimensions(Domain *d);
    void setupInterpolationMesh(Domain *d, int generateInterpolationElements, int interpolationElementsMaterialNumber, std::vector<IntArray> &newMeshNodes);
    void createInterpolationElements(Domain *d);
    void addCrosssectionToInterpolationElements(Domain *d);

    void applyApproach1(Domain *d);
    void applyApproach2(Domain *d, int homMtrxType, double volumeOfInterpolationMesh);
    void applyApproach3(Domain *d, int homMtrxType);

    void homogenizationOfStiffMatrix(double &homogenizedE, double &homogenizedNu, const FloatMatrix &Diso );
    void createGlobalStiffnesMatrix(FloatMatrix &Diso, double &S0, Domain *d, int homMtrxType,  double volumeOfInterpolationMesh);

    void computeStiffnessTensorOf1Link(FloatMatrix &D1, double &S0, Element *e, Domain *d);
    bool stiffnessAssignment( std::vector<FloatMatrix> &individualStiffnessTensors, FloatArray &individialS0, Domain *d, Element *e, qcNode *qn1, qcNode *qn2 );

    void computeIntersectionsOfLinkWithInterpElements( IntArray &intersected, FloatArray &lengths, Domain *d, Element *e, qcNode *qn1, qcNode *qn2);
    bool computeIntersectionsOfLinkWith2DTringleElements( IntArray &intersected, FloatArray &lengths, Domain *d, Element *e, qcNode *qn1, qcNode *qn2);
    bool computeIntersectionsOfLinkWith3DTetrahedraElements( IntArray &intersected, FloatArray &lengths, Domain *d, Element *e, qcNode *qn1, qcNode *qn2);

    void initializeConnectivityTableForInterpolationElements( Domain *d );

    bool intersectionTestSegmentTrianglePlucker3D(FloatArray &intersectCoords, const FloatArray &A, const FloatArray &B, const FloatArray &C, const FloatArray &X1, const FloatArray &X2 );

    int intersectionTestSegmentTetrahedra3D(std::vector<FloatArray> &intersectCoords, const FloatArray &A, const FloatArray &B, const FloatArray &C, const FloatArray &D, const FloatArray &X1, const FloatArray &X2 );

    bool intersectionTestSegmentSegment2D(FloatArray &intersectCoords, const FloatArray &A1, const FloatArray &A2, const FloatArray &B1, const FloatArray &B2 );
	
    int intersectionTestSegmentTriangle2D(std::vector<FloatArray> &intersectCoords, const FloatArray &A, const FloatArray &B, const FloatArray &C, const FloatArray &U1, const FloatArray &U2 );

    void transformStiffnessTensorToMatrix(FloatMatrix &matrix, const FloatMatrix &tensor );
};
} // end namespace oofem
#endif // quasicontinuum_h
