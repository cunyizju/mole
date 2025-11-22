#######################################################################
# generate git-version signature
#######################################################################
include_directories(${CMAKE_BINARY_DIR})
find_package(Git)
add_custom_target(version
  ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/src/mole_version.h.in
                   -D DST=${CMAKE_BINARY_DIR}/mole_version.h
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/src/GenerateVersionHeader.cmake
  )