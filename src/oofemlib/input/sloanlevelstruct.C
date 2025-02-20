// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/* Modified and optimized by: Borek Patzak */
/* Author: Milan Jirasek */

#include "input/sloanlevelstruct.h"
#include "input/sloangraphnode.h"
#include "input/sloangraph.h"
#include "math/intarray.h"
#include "input/domain.h"

namespace oofem {
#define LEVEL_LIST_GROW_CHUNK 50

SloanLevelStructure :: ~SloanLevelStructure()
{
    destroyLevels();
}

void
SloanLevelStructure :: destroyLevels()
{
    Structure.clear();
}

int
SloanLevelStructure :: formYourself(int limitWidth)
{
    if ( ! Structure.empty() ) {
        return 1;
    }

    int nnodes = Graph->getNumberOfNodes();
    IntArray nodalStatuses(nnodes);
    IntArray Level = {Root};

    // mark root
    nodalStatuses.at(Root) = 1;

    while ( !Level.isEmpty() ) { /* loop over levels */
        Structure.push_back(Level);
        /* start new level */
        /* loop over nodes on prev. level */
        Level.resize(0);
        int CurrLevelWidth = 0;
        for ( int inode: Structure.back() ) {
            for ( int n: Graph->giveNode( inode ).giveNeighborList() ) {
                if ( nodalStatuses.at(n) == 0 ) {
                    Level.followedBy(n, LEVEL_LIST_GROW_CHUNK);
                    nodalStatuses.at(n) = 1;
                    if ( ( limitWidth > 0 ) && ( ++CurrLevelWidth > limitWidth ) ) {
                        this->destroyLevels();
                        return 0; // zero mean aborted assembly
                    }
                }
            }
        }
    }

    return 1;
}

void
SloanLevelStructure :: computeDepth()
{
    this->formYourself();
    Depth = Structure.size();
}

void
SloanLevelStructure :: computeWidth()
{
    Width = 0;
    for ( int i = 1; i <= giveDepth(); i++ ) {
        int LevelWidth = giveLevel(i).giveSize();
        if ( Width < LevelWidth ) {
            Width = LevelWidth;
        }
    }
}

IntArray &
SloanLevelStructure :: giveLevel(int num)
{
    if ( Structure.empty() ) {
        this->formYourself();
    }

    if ( num < 1 || num > giveDepth() ) {
        OOFEM_ERROR("out of bounds (%d)", num);
    }

    return Structure [ num - 1 ];
}
} // end namespace oofem
