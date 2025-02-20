// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef SOLUTIONBASEDSHAPEFUNCTION_H_
#define SOLUTIONBASEDSHAPEFUNCTION_H_

#include "bc/activebc.h"
#include "dofman/dofiditem.h"
#include "math/floatarray.h"

#define _IFT_SolutionbasedShapeFunction_Name "solutionbasedshapefunction"
#define _IFT_SolutionbasedShapeFunction_ShapeFunctionFile "shapefunctionfile"
#define _IFT_SolutionbasedShapeFunction_Externalset "externalset"
#define _IFT_SolutionbasedShapeFunction_UseCorrectionFactors "usecorrectionfactors"
#define _IFT_SolutionbasedShapeFunction_DumpSnapshots "dumpsnapshots"

namespace oofem {
class EngngModel;
class Element;
class Node;

struct SurfaceDataStruct {
    bool isPlus;
    bool isMinus;
    bool isZeroBoundary;
    bool isFree;
    DofIDItem DofID;
    DofManager *DofMan;
    double value;
};

struct modeStruct {
    double am, ap;
    std::unique_ptr<EngngModel> myEngngModel;
    std :: vector<SurfaceDataStruct> SurfaceData;

    /* Visual c++ 2017 compatibility */
#ifdef _MSC_VER
	modeStruct() {}
	modeStruct(std::unique_ptr<EngngModel>& model) : myEngngModel(std::move(model)) {}
	modeStruct(modeStruct&& B) : myEngngModel(std::move(B.myEngngModel)) { am = B.am; ap = B.ap; SurfaceData = B.SurfaceData; }
	modeStruct(modeStruct& B) : myEngngModel(std::move(B.myEngngModel)) { am = B.am; ap = B.ap; SurfaceData = B.SurfaceData; }

	modeStruct& operator= (modeStruct& other) {
		myEngngModel = std::move(other.myEngngModel);
		am = other.am;
		ap = other.ap;
		SurfaceData = other.SurfaceData;
		return *this;
	}
#endif /* end of Visual c++ 2017 compatibility */ 



};

class OOFEM_EXPORT SolutionbasedShapeFunction : public ActiveBoundaryCondition
{
private:
    Node *myNode;
    IntArray myDofIDs;
    int externalSet;
    int order;
    double TOL;
    std :: string filename;
    bool useConstantBase;
    bool isLoaded;

    bool useCorrectionFactors;
    bool dumpSnapshot;

    double bigNorm;
    int worstIndex;

    bool isCoeff(ActiveDof *dof);

    std :: vector< modeStruct >modes;

    void updateModelWithFactors(modeStruct &m);
    TimeStep *thisTimestep;

    FloatArray maxCoord, minCoord;

    void setBoundaryConditionOnDof(Dof *d, double value);

    void setLoads(EngngModel &myEngngModel, int d);
    void loadProblem();
    void init();

    void computeCorrectionFactors(modeStruct &myMode, IntArray &Dofs, double &am, double &ap);

    /**
     * @brief giveValueAtPoint
     * @param answer        Values as point corresponding to the coordinate and dofID
     * @param coords        Coordinated at which evaluation takes place
     * @param dofID         Specifies DofIDs to evaluate
     * @param myEngngModel  EngngModel
     */
    void giveValueAtPoint(FloatArray &answer, const FloatArray &coords, IntArray &dofID, EngngModel &myEngngModel);

    void giveCorrectedValueAtPoint(FloatArray &answer, const FloatArray &coords, IntArray &dofID, EngngModel &myEngngModel);

    void splitBoundaryNodeIDs(modeStruct &mode, Element &e, IntArray &boundary, IntArray &pList, IntArray &mList, IntArray &zList, FloatMatrix &nodeValues);

    void computeBaseFunctionValueAt(FloatArray &answer, const FloatArray &coords, IntArray &dofIDs, EngngModel &myEngngModel);

    void initializeSurfaceData(modeStruct &mode);

    void copyDofManagersToSurfaceData(modeStruct &mode, IntArray nodeList, bool isPlus, bool isMinus, bool isZero);

    void whichBoundary(const FloatArray &coord, bool &isPlus, bool &isMinus, bool &isZero);

public:
    SolutionbasedShapeFunction(int n, Domain * d);
    virtual ~SolutionbasedShapeFunction() {}

    void initializeFrom(InputRecord &ir) override;

    bool requiresActiveDofs() override { return true; }
    int giveNumberOfInternalDofManagers() override { return 1; }
    DofManager *giveInternalDofManager(int i) override;

    double giveUnknown(PrimaryField &field, ValueModeType mode, TimeStep *tStep, ActiveDof *dof) override;
    double giveUnknown(ValueModeType mode, TimeStep *tStep, ActiveDof *dof) override;

    bool hasBc(Dof *dof, TimeStep *tStep) override { return false; }

    bool isPrimaryDof(ActiveDof *dof) override { return false; }

    void computeDofTransformation(ActiveDof *dof, FloatArray &masterContribs) override;

    int giveNumberOfMasterDofs(ActiveDof *dof) override;
    Dof *giveMasterDof(ActiveDof *dof, int mdof) override;

    const char *giveClassName() const override { return "SolutionbasedShapeFunction"; }
    const char *giveInputRecordName() const override { return _IFT_SolutionbasedShapeFunction_Name; }

    double checkIncompressibility(EngngModel &myEngngModel);
};
}

#endif /* SOLUTIONBASEDSHAPEFUNCTION_H_ */
