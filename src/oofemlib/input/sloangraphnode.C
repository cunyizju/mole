// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/* Modified and optimized by: Borek Patzak */
/* Author: Milan Jirasek */

#include "input/sloangraphnode.h"
#include "input/sloangraph.h"
#include "math/mathfem.h"

namespace oofem {
SloanGraphNode :: SloanGraphNode(SloanGraph *graph, int numOld) : neighborList()
{
    this->graph = graph;
    NumberOld  = numOld;
    NumberNew  = 0;
    nodeStatus = Inactive;
    Degree    = 0;
    Distance  = -1;
    Priority  = -1;
}

SloanGraphNode :: ~SloanGraphNode()
{ }

void SloanGraphNode :: addNeighbor(int newNeighbor)
{
    // check if neighbor already in list
    for ( int neighbor: this->neighborList ) {
        if ( neighbor == newNeighbor ) {
            return;
        }
    }

    Degree++;
    this->neighborList.push_front(newNeighbor);
}

int SloanGraphNode :: computeProfileHeight()
{
    int numberMin = NumberNew;
    for ( int neighbor: this->neighborList ) {
        numberMin = min( numberMin, this->graph->giveNode(neighbor).giveNewNumber() );
    }

    return ( NumberNew - numberMin + 1 );
}
} // end namespace oofem
