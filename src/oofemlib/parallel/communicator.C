// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "parallel/communicator.h"
#include "math/intarray.h"

#include <cstdarg>

#ifdef __USE_MPI
 #include <mpi.h>
#endif

namespace oofem {
CommunicatorBuff :: CommunicatorBuff(int s, CommBuffType t)
{
    this->processCommBuffs.reserve(s);
    for ( int i = 0; i < s; ++i ) {
        this->processCommBuffs.emplace_back(t);
    }
}


Communicator :: Communicator(EngngModel *emodel, CommunicatorBuff *b, int rank, int size, CommunicatorMode m) :
    rank(rank),
    engngModel(emodel),
    mode(m)
{
    processComms.reserve(size);
    for ( int i = 0; i < size; i++ ) {
        processComms.emplace_back(b->giveProcessCommunicatorBuff ( i ), i, mode);
    }
}


int
Communicator :: initExchange(int tag)
{
    int result = 1;
    for  ( auto &pc : processComms ) {
        result &= pc.initExchange(tag);
    }

    return result;
}

int
Communicator :: finishExchange()
{
    int result = 1;
    for  ( auto &pc : processComms ) {
        result &= pc.finishExchange();
    }

    return result;
}



int
Communicator :: initSend(int tag)
{
    int result = 1;
    for  ( auto &pc : processComms ) {
        result &= pc.initSend(tag);
    }

    return result;
}

int
Communicator :: initReceive(int tag)
{
    int result = 1;
    for  ( auto &pc : processComms) {
        result &= pc.initReceive(tag);
    }

    return result;
}

void
Communicator :: clearBuffers()
{
    for  ( auto &pc : processComms ) {
        pc.clearBuffers();
    }
}

std :: string
Communicator :: errorInfo(const char *func) const
{
    return std::string("Communicator::") + func + ", Rank: " + std::to_string(rank);
}
} // end namespace oofem
