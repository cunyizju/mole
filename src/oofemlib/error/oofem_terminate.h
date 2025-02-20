// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef oofem_terminate_h
#define oofem_terminate_h

namespace oofem {
/// OOFEM terminate exception class.
class OOFEM_EXPORT OOFEM_Terminate
{
public:
    enum OOFEM_exit_status {
        ES_OK,
        ES_UnknownErr
    };

    OOFEM_exit_status status;
    OOFEM_Terminate(OOFEM_exit_status s = ES_OK) {
        status = s;
    }
};
} // end namespace oofem
#endif // oofem_terminate_h
