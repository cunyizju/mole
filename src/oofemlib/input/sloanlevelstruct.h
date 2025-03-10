// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef sloanlevelstruct_h
#define sloanlevelstruct_h

#include "oofemcfg.h"
#include "math/intarray.h"

namespace oofem {
class SloanGraph;

/**
 * Class representing level structure for Sloan profile optimizer.
 * This is partitioning of the nodes such that each node is assigned
 * to one of the levels in accordance with its distance from a specified root node.
 * @author Milan Jirasek
 * @author Borek Patzak
 */
class OOFEM_EXPORT SloanLevelStructure
{
private:
    /// Reference to corresponding graph
    SloanGraph *Graph;
    /// Root node of level structure
    int Root;
    /// End node of root structure
    int End;
    /// Data representation of structure: List of arrays, one array for each level.
    std::vector< IntArray >Structure;
    /// Depth of structure defined as number of levels
    int Depth;
    /// Width of structure defined as max number of nodes in all levels.
    int Width;

public:
    /// Creates new level structure assignment to graph, with root being the root node.
    SloanLevelStructure(SloanGraph * graph, int root) : Structure()
    {
        Graph = graph;
        Root = root;
        End = 0;
        Depth = Width = 0;
    }
    /// Destructor
    ~SloanLevelStructure();

    /// Destroys all levels
    void destroyLevels();
    /**
     * Builds the level structure. The limitWidth parameter allows receiver build-up phase
     * to be aborted during the assembly, when width of some level is greater than given value.
     * Default value for limitWidth is -1 meaning no width limit.
     * If assembly aborted, the destroyLevels() method is called.
     * @return Zero if assembly aborted due to width limit, positive value otherwise.
     */
    int formYourself(int limitWidth = -1);

    /// Returns the depth of receiver.
    int  giveDepth() {
        if ( !Depth ) {
            computeDepth();
        }

        return Depth;
    }
    /// Returns the width of receiver.
    int  giveWidth() {
        if ( !Width ) {
            computeWidth();
        }

        return Width;
    }
    /// Returns the i-th level of receiver.
    IntArray &giveLevel(int num);
    /// Sets the end node of receiver.
    void setEnd(int end) { End = end; }
    /// Returns the end node of receiver.
    int giveEnd() { return End; }
    /// Return root node of receiver.
    int giveRoot() { return Root; }

private:
    /// Computes depth of receiver.
    void computeDepth();
    /// Computes the Width of receiver.
    void computeWidth();
};
} // end namespace oofem
#endif // sloanlevelstruct_h
