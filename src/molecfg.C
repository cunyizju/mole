#include "mole_version.h"
#include "molecfg.h"

MOLE_EXPORT const char* PRG_VERSION =  "MOLE version" __MOLE_MAJOR_VERSION "." __MOLE_MINOR_VERSION;
MOLE_EXPORT const char* MOLE_VERSION = __MOLE_VERSION;
MOLE_EXPORT const char* MOLE_COPYRIGHT = __MOLE_COPYRIGHT;
MOLE_EXPORT const char* MOLE_GIT_HASH = __MOLE_GIT_HASH;
MOLE_EXPORT const char* MOLE_GIT_REPOURL = __MOLE_GIT_REPOURL;
MOLE_EXPORT const char* MOLE_GIT_BRANCH = __MOLE_GIT_BRANCH;


MOLE_EXPORT const char* PRG_HEADER_SM = ""
"____________________________________________________\n"
"  __  __  ____  __    ______\n"
" /  \\/  |/ __ \\/ /   / ____/\n"
"/ /\\  / / /_/ / /   / __/   \n"
"/_/  /_/\\____/_/___/____/   MOLE\n"
__MOLE_COPYRIGHT "\n"
"____________________________________________________\n";

MOLE_EXPORT const char* PRG_HEADER = ""
"############################################################## github.com/cunyizju/mole ###\n"
"  __  __  ____  __    ______\n"
" /  |/  |/ __ \\/ /   / ____/\n"
"/ /|_/ // /_/ / /   / __/   \n"
"\\____/  \\____/_/   /____/  MOLE ver. " __MOLE_VERSION "\n"
"                           " __MOLE_COPYRIGHT "\n"
"################################################################################\n"
"Git URL:" __MOLE_GIT_REPOURL ",\n    branch:" __MOLE_GIT_BRANCH ", hash:" __MOLE_GIT_HASH "\n\n";

                               
MOLE_EXPORT const char* HOST_TYPE = __HOST_TYPE;
MOLE_EXPORT const char* HOST_NAME = __HOST_NAME;
MOLE_EXPORT const char* MODULE_LIST = __MODULE_LIST;