// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef matstatusvar_h
#define matstatusvar_h

namespace oofem {
enum MatStatusVar {
    ms_unknown,
    ms_yield_flag,
    ms_isCracked_flag,
    ms_crackDirection_flag,
    ms_crackStatus_flag,
    ms_damage_flag
};
} // end namespace oofem
#endif // matstatusvar_h
