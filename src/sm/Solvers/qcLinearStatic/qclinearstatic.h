// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qclinearstatic_h
#define qclinearstatic_h

#include "sm/Solvers/linearStatic/linearstatic.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrxtype.h"
#include "dofman/node.h"

#include "sm/Quasicontinuum/fullsolveddomain.h"
#include "sm/Quasicontinuum/quasicontinuumnumberingscheme.h"

#define _IFT_QClinearStatic_Name "qclinearstatic"
#define _IFT_QuasiContinuum_approach "qcapproach"
#define _IFT_QuasiContinuum_mtrx_type "mtrxt"
// generation
#define _IFT_QuasiContinuum_generate_Particles "genparticles"
#define _IFT_QuasiContinuum_generate_Links "genlinks"
#define _IFT_QuasiContinuum_generate_Interpolation_Elements "geninterpelem"

#define _IFT_QuasiContinuum_t3d_File_Name "t3dfile"
#define _IFT_QuasiContinuum_interp_Mat_Number "intmatnum"

#define _IFT_QuasiContinuum_T3D_Interpolation_Mesh_size "imsize"
// fullsolveddomain
#define _IFT_FullSolvedDomain_nodes "fsd_n"
#define _IFT_FullSolvedDomain_elements "fsd_e"
#define _IFT_FullSolvedDomain_radius "fsd_r"
#define _IFT_FullSolvedDomain_box "fsd_b"

namespace oofem {
class SparseMtrx;
class QCFullsolveddomain;

/**
 * This class implements linear static engineering problem.
 * Multiple loading works only if linear elastic material (such as isoLE)  is used.
 * (Other non-linear materials keep load history, so such multiple loading
 * will cause that next step will be assumed as new load increment,
 * not the total new load). Because they always compute real stresses according
 * to reached strain state, they are not able to respond to linear analysis.
 *
 * Solution of this problem is series of loading cases, maintained as sequence of
 * time-steps. This solution is in form of linear equation system Ax=b
 * Tasks:
 * - Creating Numerical method for solving @f$ K\cdot x=b @f$.
 * - Interfacing Numerical method to Elements.
 * - Managing time steps.
 */
 class QClinearStatic : public LinearStatic
{
protected:
    SparseMtrx *stiffnessMatrix;
    FloatArray loadVector;
    FloatArray displacementVector;

    LinSystSolverType solverType;
    SparseMtrxType sparseMtrxType;
    /// Numerical method used to solve the problem.
    SparseLinearSystemNM *nMethod;

    QuasicontinuumNumberingscheme qcEquationNumbering;

    int initFlag;
	
    int qcApproach; // 0-full, 1-hn, 2-global homog., 3 local homog.
    int homogenizationMtrxType; // 1-iso, 2-aniso
    int generateParticles;
    int generateLinks;
    int generateInterpolationElements; // 0-use oofem input, 1-generate, 2-load from extern file
    int interpolationElementsMaterialNumber;
    double defaultT3DMeshSize;
    std::string t3dFileName;

    std::vector<bool> activatedElementList;
    std::vector<bool> activatedNodeList;

    // km TO DO:  use this lists for faster loops
    //std :: vector< std :: unique_ptr< Element > > interpolationElementList;
    //std :: vector< std :: unique_ptr< Element > > links;

    std::vector<IntArray> interpolationMeshNodes;
    int  numberOfIntepolationElements;

    QCFullsolveddomain Fullsolveddomain;

    FloatArray FullSolvedDomainNodes;
    FloatArray FullSolvedDomainElements;
    FloatArray FullSolvedDomainRadius;
    FloatArray FullSolvedDomainBox;

public:
    QClinearStatic(int i, EngngModel *master = nullptr);
    virtual ~QClinearStatic();

    void postInitialize() override;

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;

    void initializeFrom(InputRecord &ir) override;

    // identification
    const char *giveInputRecordName() const override { return _IFT_QClinearStatic_Name; }
    const char *giveClassName() const override { return "QClinearStatic"; }
    fMode giveFormulation() override { return TL; }

    virtual void updateNodeTypes(Domain *d);
    virtual void setQCNodeType(Domain *d);

    virtual void initializeFullSolvedDomain(InputRecord &ir);
    virtual bool nodeInFullSolvedDomainTest(Node *n);
    virtual void setRepNodesInVerticesOfInterpolationMesh(Domain *d);

    virtual void  createInterpolationMeshNodes(Domain *d);
    virtual std::vector<IntArray>  generateInterpolationMesh(Domain *d);
    virtual std::vector<IntArray>  loadInterpolationMesh(Domain *d);
    virtual std::vector<IntArray>  transformMeshToParticles(Domain *d, std::vector<FloatArray> &nodeCoords, std::vector<IntArray> &meshNodes);
    virtual double computeTotalVolumeOfInterpolationMesh(Domain *d);

    virtual DofManager* findNearestParticle(Domain *d, FloatArray coords);
    /**
     * Returns Fullsolved domain to which receiver is associated.
     */
    virtual QCFullsolveddomain *giveFullSolvedDomain();
    virtual int giveQcApproachNumber() {return qcApproach;}

    bool isElementActivated( int elemNum ) override { return activatedElementList[elemNum-1]; }
    bool isElementActivated( Element  *e ) override { return activatedElementList[e->giveNumber()-1]; }

    void setActivatedNodeList( IntArray nodeList, Domain *d);
    void setActivatedElementList( IntArray elemList);

    UnknownNumberingScheme &giveEquationNumbering() override { return this->qcEquationNumbering; }
};


} // end namespace oofem
#endif // qclinearstatic_h
