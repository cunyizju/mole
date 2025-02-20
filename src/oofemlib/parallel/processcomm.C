// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "parallel/processcomm.h"
#include "math/intarray.h"
#include "parallel/combuff.h"
#include "parallel/dyncombuff.h"

#ifdef __USE_MPI
 #include <mpi.h>
#endif

namespace oofem {
ProcessCommunicatorBuff :: ProcessCommunicatorBuff(CommBuffType t)
{
    if ( t == CBT_static ) {
        send_buff = std::make_unique<StaticCommunicationBuffer>(MPI_COMM_WORLD);
        recv_buff = std::make_unique<StaticCommunicationBuffer>(MPI_COMM_WORLD);
    } else {
        send_buff = std::make_unique<DynamicCommunicationBuffer>(MPI_COMM_WORLD);
        recv_buff = std::make_unique<DynamicCommunicationBuffer>(MPI_COMM_WORLD);
    }
}


ProcessCommunicator :: ProcessCommunicator(ProcessCommunicatorBuff *b, int rank, CommunicatorMode m) :
    rank(rank),
    pcBuffer(b),
    toSend(), toReceive(),
    mode(m)
{
}


int
ProcessCommunicator :: initSend(int tag)
{
    int result = 1;
    if ( !toSend.isEmpty() || ( this->mode == CommMode_Dynamic ) ) {
        //  fprintf (stderr, "\nNlDEIDynamicDomainComunicator :: initExchange: sending to %d",rank);
        result = giveProcessCommunicatorBuff()->initSend(this->rank, tag);
    } else {
        giveProcessCommunicatorBuff()->initSendBuff();
    }

    return result;
}


int
ProcessCommunicator :: initReceive(int tag)
{
    int result = 1;
    if ( !toReceive.isEmpty() || ( this->mode == CommMode_Dynamic ) ) {
        //  fprintf (stderr, "\nNlDEIDynamicDomainComunicator :: initExchange: recv from %d",rank);
        result &= giveProcessCommunicatorBuff()->initReceive(this->rank, tag);
    } else {
        giveProcessCommunicatorBuff()->initRecvBuff();
    }

    return result;
}


int
ProcessCommunicator :: initExchange(int tag)
{
    int result = 1;
    result &= initSend(tag);
    result &= initReceive(tag);

    return result;
}

int
ProcessCommunicator :: finishExchange()
{
    return waitCompletion();
}

void
ProcessCommunicator :: clearBuffers()
{
    giveProcessCommunicatorBuff()->init();
}

int
ProcessCommunicator :: sendCompleted()
{
    if ( !toSend.isEmpty() || ( this->mode == CommMode_Dynamic ) ) {
        return giveProcessCommunicatorBuff()->sendCompleted();
    } else {
        return 1;
    }
}

int
ProcessCommunicator :: receiveCompleted()
{
    if ( !toReceive.isEmpty() || ( this->mode == CommMode_Dynamic ) ) {
        return giveProcessCommunicatorBuff()->receiveCompleted();
    } else {
        return 1;
    }
}

int
ProcessCommunicator :: testCompletion()
{
    return ( sendCompleted() && receiveCompleted() );
}

int
ProcessCommunicator :: waitCompletion()
{
    while ( !testCompletion() ) {
        ;
    }

    return 1;
}
} // end namespace oofem
