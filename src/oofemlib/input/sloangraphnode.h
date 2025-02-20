// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef sloangraphnode_h
#define sloangraphnode_h

#include <list>

#include "oofemcfg.h"

namespace oofem {
class SloanGraph;

/**
 * Class representing node in undirected graph, used by Sloan profile optimizer.
 * Node keeps its old and new number, status, degree, distance and corresponding priority.
 * The list of neighbouring nodes is also kept.
 * @author Milan Jirasek
 * @author Borek Patzak
 */
class OOFEM_EXPORT SloanGraphNode
{
public:
    /// Status type definition.
    enum SloanGraphNode_StatusType { Inactive, Preactive, Active, Postactive };

private:
    /// Associated graph structure, to which node belongs.
    SloanGraph *graph;
    /// Old (original) number.
    int NumberOld;
    /// New (optimized) number.
    int NumberNew;
    /// Status of node.
    SloanGraphNode_StatusType nodeStatus;
    /// Node degree (number of adjacent edges).
    int Degree;
    /// Node distance.
    int Distance;
    /// Node priority.
    int Priority;
    /// List of neighbouring nodes (represent graph edges).
    std :: list< int >neighborList;

public:
    /// Creates node belonging to given graph with given old number.
    SloanGraphNode(SloanGraph * graph, int numOld);
    /// Destructor.
    ~SloanGraphNode();

    /**
     * Add neighbouring node to corresponding list.
     * The entry is added only if not exist before,
     * the Degree member is updated accordingly.
     */
    void addNeighbor(int neighbor);

    /// Returns new number of receiver.
    int giveNewNumber() { return NumberNew; }
    /// Returns old number of receiver.
    int giveOldNumber() { return NumberOld; }
    /// Returns receiver status.
    SloanGraphNode_StatusType giveStatus() { return nodeStatus; }
    /// Return the receiver's degree.
    int giveDegree() { return Degree; }
    /// Returns distance of receiver.
    int giveDistance() { return Distance; }
    /// Returns priority of receiver.
    int givePriority() { return Priority; }
    /// Returns the neighbor list of receiver.
    std :: list< int > &giveNeighborList()  { return neighborList; }
    /// sets new number equal to old one.
    void assignOldNumber() { NumberNew = NumberOld; }

    /// Sets the receiver distance to given number.
    void setDistance(int d) { Distance = d; }
    /// Sets the receiver priority to given value.
    void setPriority(int p) { Priority = p; }
    /// Sets the status of receiver to given value.
    void setStatus(SloanGraphNode_StatusType s) { nodeStatus = s; }
    /// Sets the new number of receiver.
    void setNewNumber(int n) { NumberNew = n; }
    /// Increases the priority of receiver by given value.
    void increasePriorityBy(int p) { Priority += p; }
    /**
     * Computes the profile height corresponding to receiver from
     * current new and old numbers.
     */
    int computeProfileHeight();
};
} // end namespace oofem
#endif // sloangraphnode_h
