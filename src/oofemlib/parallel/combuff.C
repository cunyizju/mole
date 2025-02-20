// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include <cstdlib>
#include <cstring> // for memmove

#include "parallel/combuff.h"
#include "math/intarray.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "error/error.h"

namespace oofem {
#ifdef __USE_MPI
MPIBuffer :: MPIBuffer(int size, bool dynamic)
{
    this->size = 0;
    curr_pos = 0;
    buff = NULL;
    isDynamic = dynamic;
    request = MPI_REQUEST_NULL;

    this->resize(size);
}


MPIBuffer :: MPIBuffer(bool dynamic)
{
    this->size = 0;
    curr_pos = 0;
    buff = NULL;
    isDynamic = dynamic;
    request = MPI_REQUEST_NULL;
}

#endif

MPIBuffer :: ~MPIBuffer()
{
    if ( buff ) {
        free(buff);
    }
}


int
MPIBuffer :: resize(int newSize)
{
    // do not shrink
    if ( size >= newSize ) {
        return 1;
    }


    ComBuff_BYTE_TYPE *newBuff;

    if ( newSize > 0 ) {
        // allocate new memory
        if ( ( newBuff = ( ComBuff_BYTE_TYPE * )
                         malloc( newSize * sizeof( ComBuff_BYTE_TYPE ) ) ) == NULL ) {
            // alloc failed -> memory error
            OOFEM_ERROR("resize failed");
        }

        // copy old buffer into new one
        memmove(newBuff, this->buff, curr_pos);
    } else {
        newBuff = NULL;
    }

    // dealocate old buffer
    if ( this->buff ) {
        free(this->buff);
    }

    this->buff = newBuff;
    size = newSize;

    return 1;
}

void
MPIBuffer :: init()
{
    curr_pos = 0;
    request = MPI_REQUEST_NULL;
}

#ifdef __USE_MPI

int
MPIBuffer :: packArray(MPI_Comm communicator, const void *src, int n, MPI_Datatype type)
{
    int _size;
    // ask MPI for packing size for integer
    _size = this->givePackSize(communicator, type, n);

    if ( ( this->curr_pos + _size > this->size ) ) {
        // reallocate itself
        if ( isDynamic ) {
            if ( this->resize(this->curr_pos + _size + __CommunicationBuffer_ALLOC_CHUNK) == 0 ) {
                return 0;
            }
        } else {
            OOFEM_WARNING("Resize requested in static mode");
            return 0;
        }
    }

    void *__src = const_cast< void * >(src);   // throw away const
    return ( MPI_Pack(__src, n, type, this->buff, this->size,
                      & this->curr_pos, communicator) == MPI_SUCCESS );
}

int
MPIBuffer :: unpackArray(MPI_Comm communicator, void *dest, int n, MPI_Datatype type)
{
    return ( MPI_Unpack(this->buff, this->size, & this->curr_pos,
                        dest, n, type, communicator) == MPI_SUCCESS );
}

int
MPIBuffer :: iSend(MPI_Comm communicator, int dest, int tag)
{
    return ( MPI_Isend(this->buff, this->curr_pos, MPI_PACKED, dest, tag,
                       communicator, & this->request) == MPI_SUCCESS );
}


int
MPIBuffer :: iRecv(MPI_Comm communicator, int source, int tag, int count)
{
    if ( count ) {
        if ( count >= this->size ) {
            // reallocate itself
            if ( this->resize(count) == 0 ) {
                return 0;
            }
        }
    }

    return ( MPI_Irecv(this->buff, this->size, MPI_PACKED, source, tag,
                       communicator, & this->request) == MPI_SUCCESS );
}

int
MPIBuffer :: testCompletion()
{
    int flag;
    MPI_Status status;

    MPI_Test(& this->request, & flag, & status);
    return flag;
}

int
MPIBuffer :: testCompletion(int &source, int &tag)
{
    int flag;
    MPI_Status status;

    MPI_Test(& this->request, & flag, & status);

    source = status.MPI_SOURCE;
    tag = status.MPI_TAG;

    return flag;
}

int
MPIBuffer :: waitCompletion()
{
    MPI_Status status;

    return ( MPI_Wait(& this->request, & status) == MPI_SUCCESS );
}


int
MPIBuffer :: bcast(MPI_Comm communicator, int root)
{
    return MPI_Bcast(this->buff, this->size, MPI_PACKED, root, communicator);
}


int
MPIBuffer :: givePackSize(MPI_Comm communicator, MPI_Datatype type, int size)
{
    int requredSpace;
    MPI_Pack_size(size, type, communicator, & requredSpace);
    return requredSpace;
}


void
MPIBuffer :: dump()
{
    for ( int _i = 0; _i < 20; ++_i ) {
        fprintf(stderr, "%d ", buff [ _i ]);
    }

    fprintf(stderr, "\n");
}

#endif


/*
int CommunicationBuffer :: read(int *data, int count)
{
    
}

int CommunicationBuffer :: read(unsigned long *data, int count)
int CommunicationBuffer :: read(long *data, int count)
int CommunicationBuffer :: read(double *data, int count)
int CommunicationBuffer :: read(char *data, int count)

int CommunicationBuffer :: write(const int *data, int count)
int CommunicationBuffer :: write(const unsigned long *data, int count)
int CommunicationBuffer :: write(const long *data, int count)
int CommunicationBuffer :: write(const double *data, int count)
int CommunicationBuffer :: write(const char *data, int count)
*/

int CommunicationBuffer :: read(bool &data)
{
    char val;
    int ret = this->read(& val, 1);
    data = val != 0;
    return ret;
}

int CommunicationBuffer :: write(bool data)
{
    char val = data;
    return this->write(& val, 1);
}

int CommunicationBuffer :: givePackSizeOfInt(int count)
{
    int requiredSpace;
    MPI_Pack_size(count, MPI_INT, communicator, & requiredSpace);
    return requiredSpace;
}

int CommunicationBuffer :: givePackSizeOfDouble(int count)
{
    int requiredSpace;
    MPI_Pack_size(count, MPI_DOUBLE, communicator, & requiredSpace);
    return requiredSpace;
}

int CommunicationBuffer :: givePackSizeOfChar(int count)
{
    int requiredSpace;
    MPI_Pack_size(count, MPI_CHAR, communicator, & requiredSpace);
    return requiredSpace;
}

int CommunicationBuffer :: givePackSizeOfBool(int count)
{
    int requiredSpace;
    MPI_Pack_size(count, MPI_CHAR, communicator, & requiredSpace);
    return requiredSpace;
}

int CommunicationBuffer :: givePackSizeOfLong(int count)
{
    int requiredSpace;
    MPI_Pack_size(count, MPI_LONG, communicator, & requiredSpace);
    return requiredSpace;
}

} // end namespace oofem
