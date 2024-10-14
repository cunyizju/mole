# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PrincipalValuesCalculator VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the OOFEM library and include directories
# Modify these paths based on where you have installed OOFEM.
set(OOFEM_INCLUDE_DIR "../../oofemlib")
set(OOFEM_LIBRARY_DIR "../../../build/dev")
set(OOFEM_LIB "oofemlib")

# Include the OOFEM header files
include_directories(${OOFEM_INCLUDE_DIR})

# Add the executable for your project
add_executable(principal_values principal_values.C)

# Link the OOFEM library with your executable
target_link_libraries(principal_values ${OOFEM_LIB})

# Set the path where the linker can find OOFEM libraries
link_directories(${OOFEM_LIBRARY_DIR})
