// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef communicatormode_h
#define communicatormode_h

namespace oofem {
/**
 * The communicator mode determines the communication.
 */
enum CommunicatorMode {
    /**
     * The mode can be static, meaning that each node can assemble its communication maps
     * independently (or by independent communication). This implies that the size of
     * communication buffers is known in advance. Also if no data are planned to send to remote node, there
     * is no communication with this node (both sender and receiver know that there will be no data to send).
     */
    CommMode_Static,
    /**
     * (Dynamic) In this case the communication pattern and the amount of data sent between nodes is
     * not known in advance. This requires to use dynamic (packeted) buffering.
     */
    CommMode_Dynamic,
};
} // end namespace oofem
#endif // communicatormode_h
