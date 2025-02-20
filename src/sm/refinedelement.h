// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef refinedelement_h
#define refinedelement_h

#include "math/intarray.h"
#include "input/valuemodetype.h"

#include <vector>

namespace oofem {
class Domain;
class Element;
class TimeStep;
class Node;

class RefinedElement
{
protected:
    int elementId;
    std::vector< IntArray >fineNodeList;
    IntArray boundaryFlag;

public:
    RefinedElement(Domain * d, int elem, int level);
    ~RefinedElement();

    IntArray *giveFineNodeArray(int node);
    IntArray *giveBoundaryFlagArray(void) { return & boundaryFlag; }

    void giveBoundaryFlagArray(int inode, Element *element, IntArray &answer);

    bool giveBoundaryLoadArray1D(int inode, Element *element, IntArray &boundaryLoadArray);
    bool giveBoundaryLoadArray2D(int inode, Element *element, std::vector< IntArray > &boundaryLoadList);
    bool giveBoundaryLoadArray3D(int inode, Element *element, std::vector< IntArray > &boundaryLoadList);

    bool giveBcDofArray1D(int inode, Element *element, IntArray &sideBcDofId, int &sideNumBc, TimeStep *tStep);
    bool giveBcDofArray2D(int inode, Element *element, std::vector< IntArray > &sideBcDofIdList, IntArray &sideNumBc, TimeStep *tStep);
    bool giveBcDofArray3D(int inode, Element *element, std::vector< IntArray > &sideBcDofIdList, IntArray &sideNumBc,
                          std::vector< IntArray > &faceBcDofIdList, IntArray &faceNumBc, TimeStep *tStep);

protected:
    /**
     * Extract from dofArray of slave_node those Dofs that have compatible BCs with master_node
     * @param master_node Node to which Dof compatibility will be compared.
     * @param slave_node Node with original Dofs.
     * @param dofIDArray Array of ids of Dofs of slave_node to chose from.
     * @param dofs Number of Dofs in dofArray.
     * @param answer Array of ids of Dofs in dofArray with compatible BCs.
     * @param mode Mode of Dof values.
     * @param tStep Active time step.
     * @return Number of Dofs with compatible BCs.
     */
    int giveCompatibleBcDofArray(Node *master_node, Node *slave_node, IntArray &dofIDArray, int dofs, IntArray &answer,
                                 ValueModeType mode, TimeStep *tStep);

    /// Returns string for prepending output (used by error reporting macros).
    std :: string errorInfo(const char *func) const;
};
} // end namespace oofem
#endif // refinedelement_h
