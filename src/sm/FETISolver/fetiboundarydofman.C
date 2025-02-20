// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/FETISolver/fetiboundarydofman.h"
#include "error/error.h"

using namespace std;


namespace oofem {
FETIBoundaryDofManager :: FETIBoundaryDofManager() : partitions(), codeNumbers()
{
    globalNumber = 0;
    numberOfPartitions = 0;
    ndofs = 0;
}

FETIBoundaryDofManager :: FETIBoundaryDofManager(int num, int part, int ndof) : partitions(part), codeNumbers(part * ndof)
{
    globalNumber = num;
    numberOfPartitions = part;
    ndofs = ndof;
}



FETIBoundaryDofManager :: FETIBoundaryDofManager(const FETIBoundaryDofManager &src)
{
    globalNumber = src.globalNumber;
    numberOfPartitions = src.numberOfPartitions;
    ndofs = src.ndofs;
    referencePartition = src.referencePartition;
    partitions = src.partitions;
    codeNumbers = src.codeNumbers;
}


void
FETIBoundaryDofManager :: addPartition(int partitionNumber)
{
    IntArray partitionToAdd(1);

    partitionToAdd.at(1) = partitionNumber;
    partitions.followedBy(partitionToAdd);
    if ( numberOfPartitions != 0 ) {
        if ( partitionNumber < referencePartition ) {
            referencePartition = partitionNumber;
        }
    } else {
        referencePartition = partitionNumber;
    }

    numberOfPartitions++;
}

int
FETIBoundaryDofManager :: setCodeNumbers(int &equationCounter)
{
    int i, size;

    this->codeNumbers.resize( ( size = ( numberOfPartitions - 1 ) * ndofs ) );
    for ( i = 1; i <= size; i++ ) {
        codeNumbers.at(i) = ++equationCounter;
    }

    return equationCounter;
}


int
FETIBoundaryDofManager :: giveCodeNumber(int partition_num, int dof_num)
{
    int indx = 0, i;
    for ( i = 1; i <= numberOfPartitions; i++ ) {
        if ( partitions.at(i) != referencePartition ) {
            indx++;
        }

        if ( partitions.at(i) == partition_num ) {
            break;
        }
    }

    if ( ( indx == 0 ) || ( partition_num == referencePartition ) ) {
        return 0;
    }

    if ( ( dof_num < 1 ) || ( dof_num > ndofs ) ) {
        OOFEM_ERROR("bad dof_num requested");
    }

    return codeNumbers.at( ( indx - 1 ) * ndofs + dof_num );
}

int
FETIBoundaryDofManager :: giveCompleteLocationArray(int rank, IntArray &locationArray)
{
    int indx = 0, i;
    for ( i = 1; i <= numberOfPartitions; i++ ) {
        if ( partitions.at(i) != referencePartition ) {
            indx++;
        }

        if ( partitions.at(i) == rank ) {
            break;
        }
    }

    if ( ( indx == 0 ) || ( rank == referencePartition ) ) {
        return 0;
    }

    locationArray.resize(ndofs);
    for ( i = 1; i <= ndofs; i++ ) {
        locationArray.at(i) = codeNumbers.at( ( indx - 1 ) * ndofs + i );
    }

    return 1;
}
} // end namespace oofem
