// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef macrolspace_h
#define macrolspace_h

#include "sm/Elements/3D/lspace.h"

///@name Input fields for MacroLspace
//@{
#define _IFT_MacroLSpace_Name "macrolspace"
#define _IFT_MacroLspace_microMasterNodes "micromasternodes"
#define _IFT_MacroLspace_microBoundaryNodes "microboundarynodes"
#define _IFT_MacroLspace_stiffMatrxFileName "stiffmatrxfilename"
//@}

namespace oofem {
class Domain;
class EngngModel;
class MicroMaterial;

/**
 * This class implements a macroelement. It is derived from eight-node brick element.
 * The stiffness matrix is computed from underlying RVE and is condensed to 24 DoFs to corner nodes.
 *
 * @author Vit Smilauer
 */
class MacroLSpace : public LSpace
{
public:
    MacroLSpace(int n, Domain * d);
    virtual ~MacroLSpace();

    const char *giveInputRecordName() const override { return _IFT_MacroLSpace_Name; }
    const char *giveClassName() const override { return "MacroLSpace"; }

    void initializeFrom(InputRecord &ir) override;

    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;

    void computeField(ValueModeType mode, TimeStep *tStep, const FloatArray &lcoords, FloatArray &answer) override
    { OOFEM_ERROR("Macro space element doesn't support computing local unknown vector (yet)\n"); }

    /// Related to setting the boundary conditions of micro problem.
    virtual void changeMicroBoundaryConditions(TimeStep *tStep);

    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override;

    /**
     * Evaluates shape function at a given pointnodal representation of real internal forces obtained from microProblem.
     * @param answer Array of shape function values at given node.
     * @param coords Coordinates of nodes defining the interpolation geometry.
     * @param gcoords Global coordinates of point of interest.
     */
    virtual void evalInterpolation(FloatArray &answer, const std::vector< FloatArray > &coords, const FloatArray &gcoords);

    void updateYourself(TimeStep *tStep) override;

protected:
    /// Array containing the node mapping from microscale (which microMasterNodes corresponds to which macroNode)
    IntArray microMasterNodes;
    IntArray microBoundaryNodes;
    IntArray microDOFs;
    bool firstCall;
    MicroMaterial *microMaterial;
    Domain *microDomain;
    EngngModel *microEngngModel;
    /// Information of iteration number.
    int iteration;
    /// Stores node number on the boundary in the triplets.
    IntArray microBoundaryDofManager;
    FloatMatrix stiffMatrix;
    /// Process with external file for the storage of stiffness matrix 0-None, 1-read, 2-write.
    int stiffMatrxFileNoneReadingWriting;
    /// Array containg the force vector from nodes (if condensation is skipped, use this vector).
    FloatArray internalMacroForcesVector;
    /// Last time step when stiffness matrix was assembled.
    TimeStep *lastStiffMatrixTimeStep;
};
} // end namespace oofem
#endif //macrolspace_h
