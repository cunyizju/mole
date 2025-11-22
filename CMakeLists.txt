cmake_minimum_required (VERSION 3.10)

# Set policies to suppress warnings and ensure compatibility
if(POLICY CMP0175)
    cmake_policy(SET CMP0175 NEW)
endif()
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

# =============================================================================
# PROJECT SETUP
# =============================================================================
project (mole CXX C)

# Git submodule helper function
function(add_git_submodule dir)
    find_package(Git REQUIRED)
    if(NOT EXISTS ${dir}/CMakeLists.txt)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${dir}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    endif()
    add_subdirectory(${dir})
endfunction(add_git_submodule)

# Set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Include required CMake modules
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)
include (GenerateExportHeader)
include (ProcessorCount)
include (GenerateVersion)

# Use ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()
# Project version and metadata
set (mole_VERSION_MAJOR 0)
set (mole_VERSION_MINOR 0)
set (mole_VERSION_PATCH 1)
set (mole_HOMEPAGE "https://github.com/cunyizju/mole")
set (bp_HOMEPAGE "http://ksm.fsv.cvut.cz/%7Ebp/bp.html")
string(TIMESTAMP YEAR "%Y")
set (mole_COPYRIGHT "Copyright (C) 1994-${YEAR} Bruce LI (based on OOFEM by Borek Patzak)")
set (mole_EMAIL "cunyizju@gmail.com")

# Build configuration
set(CMAKE_VERBOSE_MAKEFILE ON)
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif ()

# =============================================================================
# COMPILER SETTINGS
# =============================================================================
if (MSVC)
    add_definitions (-D_USE_MATH_DEFINES)
    set (CMAKE_REQUIRED_DEFINITIONS "-D_USE_MATH_DEFINES")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -wd4786 -wd4100 -wd4996 -wd4706 -wd4512 -wd4251 -wd4800")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
endif ()

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# Debug and profiling flags
set (CMAKE_CXX_FLAGS_DEBUG "-DDEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -Wsuggest-override ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
set (CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_PROFILING} -O2 -pg")

# =============================================================================
# CUSTOM PATHS (User-configurable directories)
# =============================================================================
set (TINYXML2_DIR CACHE PATH "Manual TinyXML-2 directory (CEMHYD only)")
set (MPI_DIR CACHE PATH "Manual MPI directory")
set (VTK_DIR CACHE PATH "Manual VTK directory (For binary VTU export)")
set (LAPACK_DIR CACHE PATH "Manual LAPACK directory")
set (SUPERLU_MT_DIR CACHE PATH "Manual SuperLU_MT directory")
set (SPOOLES_DIR CACHE PATH "Manual SPOOLES directory")
set (METIS_DIR CACHE PATH "Manual Metis directory (Load balancing)")
set (PARMETIS_DIR CACHE PATH "Manual ParMetis directory (Load balancing)")
set (INTEL_MKL_PARDISO_DIR CACHE PATH "Manual mkl pardiso directory (sparse direct solver)")
set (INTEL_ONEAPI_DIR CACHE PATH "Manual path to oneAPI install directory (sparse direct solver)")
set (PARDISO_ORG_DIR CACHE PATH "Manual pardiso-project.org directory (sparse direct solver)")
set (PARDISO_ORG_LIB CACHE PATH "Manual pardiso-project.org library's name with complete path (sparse direct solver)")
set (PYBIND_DIR CACHE PATH "Manual pybind11 directory")

# =============================================================================
# BUILD OPTIONS
# =============================================================================

# Core build options
option (USE_SHARED_LIB "Build shared library" ON)
option (USE_SM "Enable structural mechanics module" ON)

# Parallel computing
option (USE_MPI_PARALLEL "Enable MPI-based, distributed memory parallel support" OFF)
option (USE_OPENMP "Compile with OpenMP support (for parallel assembly)" OFF)
option (USE_METIS "Enable metis support" OFF)
option (USE_PARMETIS "Enable Parmetis support" OFF)

# External solvers
option (USE_PETSC "Enable PETSc support (specify PETSC_DIR and/or PETSC_ARCH if needed)" OFF)
option (USE_LAPACK "Use LAPACK" OFF)
option (USE_SUPERLU_MT "Use SUPERLU_MT" OFF)
option (USE_INTEL_MKL_PARDISO "Enable MKLPARDISO solver support" OFF)
option (USE_ONEAPI "Enable OneMKL solver support via OneAPI" OFF)
option (USE_PARDISO_ORG "Enable PARDISO-project.org solver support" OFF)

# Optional modules (mostly obsolete)
option (USE_DSS "Enable DSS module" OFF)
option (USE_IML "Enable iml++ solvers" OFF)
option (USE_SPOOLES "Enable SPOOLES-solver" OFF)

# External libraries
option (USE_TRIANGLE "Compile with Triangle bindings" OFF)
option (USE_VTK "Enable VTK (for exporting binary VTU-files)" OFF)

# Python bindings
option (USE_BOOSTPYTHON_BINDINGS "Enable Python bindings using boost (obsolete)" OFF)
option (USE_PYBIND_BINDINGS "Enable Python Pybind11 bindings" OFF)
option (USE_PYTHON_EXTENSION "Enable Python extension for exposing C++ code to python" OFF)

# Other modules
option (USE_MFRONT "Build shared library" OFF)
option (USE_CEMHYD "Enable CemHyd support" OFF)
option (USE_MPM "Enable experimental multiphysics module" OFF)

# =============================================================================
# DEPENDENCY SETUP
# =============================================================================

# Enable MPI if parallel mode is requested
if (USE_MPI_PARALLEL)
    add_definitions (-D__MPI_PARALLEL_MODE)
    set (USE_MPI ON)
endif ()

# Coverage testing
if (CMAKE_COMPILER_IS_GNUCC)
    option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
    if (ENABLE_COVERAGE)
        add_compile_options(--coverage -O0)
        list (APPEND EXT_LIBS --coverage)
    endif()
endif ()

# =============================================================================
# INTERNAL LIBRARIES SETUP
# =============================================================================

# Include directories
include_directories ("${mole_SOURCE_DIR}/src")
include_directories ("${mole_SOURCE_DIR}/src/oofemlib")
include_directories ("${mole_BINARY_DIR}")

# Core library
list (APPEND LIBS $<TARGET_OBJECTS:core>)

# Structural mechanics module
if (USE_SM)
    include_directories ("${mole_SOURCE_DIR}/src/sm")
    include_directories ("${mole_SOURCE_DIR}/src/sm/Elements")
    include_directories ("${mole_SOURCE_DIR}/src/sm/Materials")
    add_definitions (-D__SM_MODULE)
    list (APPEND LIBS $<TARGET_OBJECTS:sm> $<TARGET_OBJECTS:sm_elements> $<TARGET_OBJECTS:sm_materials>)
    list (APPEND MODULE_LIST "sm" "sm_elements" "sm_materials")

    if (NOT MSVC AND NOT MINGW)
        find_library (DL_LIB dl) 
        list (APPEND EXT_LIBS ${DL_LIB})
    endif ()
endif ()

# DSS module
if (USE_DSS)
    include_directories ("${mole_SOURCE_DIR}/src/dss")
    list (APPEND LIBS $<TARGET_OBJECTS:dss>)
    list (APPEND MODULE_LIST "dss")
endif ()

# OpenMP support
if (USE_OPENMP)
    include (FindOpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        list (APPEND MODULE_LIST "openmp")
    endif ()
endif ()

# =============================================================================
# EXTERNAL LIBRARIES DETECTION
# =============================================================================

# LAPACK
if (USE_LAPACK)
    set (CMAKE_LIBRARY_DIR ${LAPACK_DIR})
    find_package (LAPACK REQUIRED)
    list (APPEND EXT_LIBS ${LAPACK_LIBRARIES})
    list (APPEND MODULE_LIST "LAPACK")
    add_definitions (-D__LAPACK_MODULE)
endif ()


# SuperLU_MT (Note: has conflict with libstdc set.h)
if (USE_SUPERLU_MT)
    set (CMAKE_LIBRARY_DIR ${SUPERLU_MT_DIR})
    find_package(BLAS REQUIRED)
    find_library (SUPERLU_MT_LIB superlu_mt_OPENMP PATH "${SUPERLU_MT_DIR}/lib")
    list (APPEND EXT_LIBS ${SUPERLU_MT_LIB} ${BLAS_LIBRARIES})
    list (APPEND MODULE_LIST "SuperLU_MT")
    include_directories("${SUPERLU_MT_DIR}")
    set (CMAKE_REQUIRED_INCLUDES "${SUPERLU_MT_DIR}")
    add_definitions (-D__SuperLU_MT_MODULE)
    check_include_file_cxx ("${SUPERLU_MT_DIR}/include/slu_mt_ddefs.h" HAVE_SUPERLU_MT_DDEFS_H)
    if (NOT HAVE_SUPERLU_MT_DDEFS_H)
        message (FATAL_ERROR "Necessary SUPERLU_MT headers not found; refer to CMakeLists.txt for details")
    endif ()
endif ()

# VTK
if (USE_VTK)
    set (CMAKE_LIBRARY_DIR ${VTK_DIR})
    find_package (VTK REQUIRED NO_MODULE)
    include (${VTK_USE_FILE})
    add_definitions (-D__VTK_MODULE)
    list (APPEND EXT_LIBS ${VTK_LIBRARIES})
    list (APPEND MODULE_LIST "VTK")
endif ()

if (USE_PARMETIS)
    if (PARMETIS_DIR)
        find_library (PARMETIS_LIB parmetis PATH "${PARMETIS_DIR}/lib")
        include_directories ("${PARMETIS_DIR}/include")
    else ()
        find_library (PARMETIS_LIB parmetis)
    endif ()
    if (${PARMETIS_LIB} STREQUAL "PARMETIS_LIB-NOTFOUND")
        message (FATAL_ERROR "Parmetis library not found")
    endif ()
    list (APPEND EXT_LIBS ${PARMETIS_LIB})
    list (APPEND MODULE_LIST "parmetis")
    set (USE_MPI ON)
    set (USE_METIS ON)
endif ()

if (USE_METIS)
    if (METIS_DIR)
        find_library (METIS_LIB metis PATH "${METIS_DIR}/lib")
        include_directories ("${METIS_DIR}/include")
    else ()
        find_library (METIS_LIB metis)
    endif ()
    if (${METIS_LIB} STREQUAL "METIS_LIB-NOTFOUND")
        message (FATAL_ERROR "Metis library not found")
    endif ()
    list (APPEND EXT_LIBS ${METIS_LIB})
    list (APPEND MODULE_LIST "metis")
endif ()

# PARDISO solvers
if (USE_PARDISO_ORG)
    if (NOT PARDISO_ORG_LIB)
        if (PARDISO_ORG_DIR)
            find_library (PARDISO_ORG_LIB pardiso PATH "${PARDISO_ORG_DIR}")
        else ()
            find_library (PARDISO_ORG_LIB pardiso)
        endif ()
    endif ()
    if (${PARDISO_ORG_LIB} STREQUAL "PARDISO_ORG_LIB-NOTFOUND")
        message (FATAL_ERROR "Pardiso library not found")
    endif ()
    message(STATUS "PARDISO_ORG_LIB = ${PARDISO_ORG_LIB}")
    list (APPEND EXT_LIBS ${PARDISO_ORG_LIB} blas lapack gfortran pthread)
    set (CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_COMPILE_FLAGS} -fopenmp")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp" ) 
    list (APPEND MODULE_LIST "pardiso-project.org")
endif ()

# Intel MKL PARDISO
if (USE_INTEL_MKL_PARDISO)
    list(APPEND MKL_LIBS "mkl_core" "mkl_gf_lp64" "mkl_gnu_thread" "pthread")
    foreach (f ${MKL_LIBS})
        if (INTEL_MKL_PARDISO_DIR)
            find_library (${f}_LIB ${f} PATH "${INTEL_MKL_PARDISO_DIR}/mkl/lib/intel64/" DOC "MKLPARDISO (library)")
            include_directories ("${INTEL_MKL_PARDISO_DIR}/mkl/include")
        else ()
            find_library (${f}_LIB ${f} HINTS "/opt/intel/composerxe/mkl/lib/intel64/" DOC "MKLPARDISO (library)")
            include_directories ("/opt/intel/composerxe/mkl/include")
        endif ()
        if (${${f}_LIB} STREQUAL "${f}_LIB-NOTFOUND")
            message (FATAL_ERROR "MKLPARDISO library not found")
        endif ()
        list (APPEND EXT_LIBS ${${f}_LIB})
    endforeach ()
    list (APPEND MODULE_LIST "MKLPARDISO")
endif ()

# Intel OneAPI MKL
if (USE_ONEAPI)
    if (USE_INTEL_MKL_PARDISO)
        message (FATAL_ERROR "Can't use both OneMKL and classic Intel MKL (disable USE_INTEL_MKL_PARDISO option)")
    endif()

    message(STATUS "Seeking OneAPI libraries...")
    if (WIN32)
        list(APPEND MKL_LIBS "mkl_core" "mkl_intel_lp64" "mkl_intel_thread" "libiomp5md")
    else()
        list(APPEND MKL_LIBS "mkl_core" "mkl_intel_lp64" "mkl_intel_thread" "pthread")
    endif ()
    
    # Determine OneAPI installation directory
    if (INTEL_ONEAPI_DIR)
        set(oneapiDir ${INTEL_ONEAPI_DIR})
    elseif (WIN32)
        set(oneapiDir "C:/Program Files (x86)/Intel/oneAPI")
    else()
        set(oneapiDir "/opt/intel/oneapi")
    endif()

    # Set library paths based on platform
    if (WIN32)
        set(oneapiMklDir "${oneapiDir}/mkl/latest")
        set(oneapiMklLibDir "${oneapiMklDir}/lib/intel64")
        set(oneapiLibDir "${oneapiDir}/compiler/latest/windows/compiler/lib/intel64")
    elseif(APPLE)
        set(oneapiMklDir "${oneapiDir}/mkl/latest")
        set(oneapiMklLibDir "${oneapiMklDir}/lib")
        set(oneapiLibDir "")
    elseif(UNIX)
        set(oneapiMklDir "${oneapiDir}/mkl/latest")
        set(oneapiMklLibDir "${oneapiMklDir}/lib/intel64_lin")
        set(oneapiLibDir "")
    endif()

    foreach (f ${MKL_LIBS})
        message(STATUS "    Seeking for ${f} in: ${oneapiMklLibDir};${oneapiLibDir}...")
        find_library (${f}_LIB ${f} HINTS "${oneapiMklLibDir}" "${oneapiLibDir}" DOC "MKLPARDISO (library)")
        include_directories ("${oneapiMklDir}/include")

        if (${${f}_LIB} STREQUAL "${f}_LIB-NOTFOUND")
            message (FATAL_ERROR "OneAPI ${f} library not found")
        else()
            message (STATUS "    OK: ${f} library successfully found at ${${f}_LIB}")
        endif ()
        list (APPEND EXT_LIBS ${${f}_LIB})
    endforeach ()
    list (APPEND MODULE_LIST "MKLPARDISO")
endif ()


# SLEPc (depends on PETSc)
if (USE_SLEPC)
    set (USE_PETSC ON)
endif ()

# PETSc
if (USE_PETSC)
    find_package (PETSc)
    include_directories (${PETSC_INCLUDES})
    list (APPEND EXT_LIBS ${PETSC_LIBRARIES})
    list (APPEND MODULE_LIST "PETSc")
    set (USE_MPI ON)
    add_definitions (-D__PETSC_MODULE)
endif ()

# MPI
if (USE_MPI)
    if (MPI_DIR)
        include_directories ("${MPI_DIR}/include")
        find_library (MPI_LIBRARY NAMES mpi PATHS ${MPI_DIR}/lib64 ${MPI_DIR}/lib)
        find_library (MPI_CXX_LIBRARY NAMES mpi_cxx PATHS ${MPI_DIR}/lib64 ${MPI_DIR}/lib)
        list (APPEND EXT_LIBS ${MPI_LIBRARY})
        if (MPI_CXX_LIBRARY)
            list (APPEND EXT_LIBS ${MPI_CXX_LIBRARY})
        endif ()
    else ()
        include (FindMPI)
        find_package (MPI REQUIRED)
        set (CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
        set (CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
        add_definitions (-DMPICH_IGNORE_CXX_SEEK)
        include_directories (${MPI_INCLUDE_PATH})
        list (APPEND EXT_LIBS ${MPI_LIBRARIES})
    endif (MPI_DIR)
    list (APPEND MODULE_LIST "MPI")
endif ()

# Other optional libraries
if (USE_TRIANGLE)
    find_library (TRIANGLE_LIB triangle DOC "Triangle (library)")
    if (${TRIANGLE_LIB} STREQUAL "TRIANGLE_LIB-NOTFOUND")
        message (FATAL_ERROR "Triangle library not found")
    endif ()
    add_definitions (-D__TRIANGLE_MODULE)
    list (APPEND EXT_LIBS ${TRIANGLE_LIB})
    list (APPEND MODULE_LIST "Triangle")
endif ()

if (USE_TINYXML)
    if (TINYXML2_DIR)
        include_directories (${TINYXML2_DIR})
        find_library (TINYXML2_LIB NAMES tinyxml2 HINTS ${TINYXML2_DIR} NO_DEFAULT_PATH)
    else ()
        find_library (TINYXML2_LIB NAMES tinyxml2 )
    endif ()
    if (${TINYXML2_LIB} STREQUAL "TINYXML2_LIB-NOTFOUND")
        message (FATAL_ERROR "TinyXML2 library not found")
    endif ()
    list (APPEND EXT_LIBS ${TINYXML2_LIB})
    list (APPEND MODULE_LIST "TinyXML-2")
endif ()

if (USE_IML)
    include_directories ("${mole_SOURCE_DIR}")
    set (CMAKE_REQUIRED_INCLUDES "${mole_SOURCE_DIR}")
    check_include_file_cxx ("iml/cg.h" HAVE_IML_CG_H)
    check_include_file_cxx ("iml/gmres.h" HAVE_IML_GMRES_H)
    if ((NOT HAVE_IML_CG_H) OR (NOT HAVE_IML_GMRES_H))
        message (FATAL_ERROR "Necessary IML++ headers not found")
    endif ()
    list (APPEND MODULE_LIST "IML++")
endif ()

if (USE_SPOOLES)
    if (SPOOLES_DIR)
        find_library (SPOOLES_LIB spooles DOC "SPOOLES library" PATH "${SPOOLES_DIR}/lib")
        include_directories ("${SPOOLES_DIR}/include")
    else ()
        find_library (SPOOLES_LIB spooles DOC "SPOOLES library")
    endif ()
    if (${SPOOLES_LIB} STREQUAL "SPOOLES_LIB-NOTFOUND")
        message (FATAL_ERROR "SPOOLES not found")
    endif ()
    list (APPEND EXT_LIBS ${SPOOLES_LIB})
    list (APPEND MODULE_LIST "SPOOLES")
endif ()

# =============================================================================
# PYTHON BINDINGS
# =============================================================================

# Python module extension
if(WIN32)
    set(PYTHON_MODULE_EXTENSION ".pyd")
else()
    set(PYTHON_MODULE_EXTENSION ".so")
endif()

# Python extension and Boost Python bindings
if (USE_PYTHON_EXTENSION OR USE_BOOSTPYTHON_BINDINGS)
    message(STATUS "PYTHON_LIBRARY = ${Python3_LIBRARIES}")
    message(STATUS "PYTHON_EXECUTABLE = ${Python3_EXECUTABLE}")
    message(STATUS "PYTHON_INCLUDE_DIR = ${Python3_INCLUDE_DIRS}")
    find_package (Python3 COMPONENTS Interpreter Development)
    include_directories (${Python3_INCLUDE_DIRS})
    list (APPEND EXT_LIBS ${Python3_LIBRARIES})
    list (APPEND MODULE_LIST "Python")
    
    if(USE_PYTHON_EXTENSION)
        add_definitions(-D_PYTHON_EXTENSION)    
    endif ()
    
    if(USE_BOOSTPYTHON_BINDINGS)
        find_package(Boost COMPONENTS "python-py${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}" REQUIRED)
        include_directories (${Boost_INCLUDE_DIRS})
        list (APPEND EXT_LIBS ${Boost_LIBRARIES})
        add_definitions(-D_BOOSTPYTHON_BINDINGS)
    endif ()   
endif ()

# Pybind11 bindings
if (USE_PYBIND_BINDINGS)
    find_package(Python COMPONENTS Interpreter Development)
    add_git_submodule(extern/pybind11)
    if (PYBIND_DIR)
        message(STATUS "PYBIND_DIR = ${PYBIND_DIR}")
        include_directories (${PYBIND_DIR})
    endif ()
    include_directories (${pybind11_INCLUDE_DIR} ${Python_INCLUDE_DIRS})
    list (APPEND EXT_LIBS ${Python_LIBRARIES})
    pybind11_add_module(molepy THIN_LTO ${mole_SOURCE_DIR}/bindings/python/oofem.cpp ${mole_SOURCE_DIR}/src/molecfg.C ${PYBIND11_HEADERS})
    target_link_libraries(molepy PRIVATE libmole)
    set_target_properties (molepy PROPERTIES SUFFIX "${PYTHON_MODULE_EXTENSION}")

    # Check for pytest
    if(NOT PYTEST_FOUND)
        execute_process(COMMAND ${Python_EXECUTABLE} -c "import pytest; print(pytest.__version__)"
                        RESULT_VARIABLE pytest_not_found OUTPUT_VARIABLE pytest_version ERROR_QUIET)
        if(pytest_not_found)
            message(FATAL_ERROR "Running the tests requires pytest. Please install it manually"
                    " (try: ${Python_EXECUTABLE} -m pip install pytest)")
        endif()
        set(PYBIND11_PYTEST_FOUND TRUE CACHE INTERNAL "")
        add_definitions(-D_PYBIND_BINDINGS)
    endif()
    set (USE_SHARED_LIB ON)
endif()

# =============================================================================
# CONFIGURATION
# =============================================================================

# System information
site_name (HOST_NAME)
set (HOST_TYPE "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_SYSTEM_NAME}")
set (BUILDNAME "MOLE(${MODULE_LIST})-${HOST_TYPE}-${HOST_NAME}")

# Check system capabilities
check_include_file_cxx ("execinfo.h" HAVE_EXECINFO_H)
check_cxx_symbol_exists ("cbrt" "cmath" HAVE_CBRT)
check_cxx_symbol_exists ("isnan" "cmath" HAVE_ISNAN)

# Export header configuration
if (USE_SHARED_LIB)
    set (MOLE_EXPORT_HEADER "#include \"mole_export.h\"")
else ()
    set (MOLE_EXPORT_HEADER "#define MOLE_EXPORT\n#define MOLE_NO_EXPORT")
endif ()

# Generate configuration header
configure_file (
  "${mole_SOURCE_DIR}/src/molecfg.h.in"
  "${mole_BINARY_DIR}/molecfg.h"
  )

# =============================================================================
# TARGETS BUILDING
# =============================================================================

# Subtargets
set (LIB_TYPE OBJECT)
add_subdirectory ("${mole_SOURCE_DIR}/src/oofemlib")

if (USE_DSS)
    add_subdirectory ("${mole_SOURCE_DIR}/src/dss")
endif ()

if (USE_SM)
    add_subdirectory ("${mole_SOURCE_DIR}/src/sm")
    add_subdirectory ("${mole_SOURCE_DIR}/src/sm/Elements")
    add_subdirectory ("${mole_SOURCE_DIR}/src/sm/Materials")
endif ()

# Python bindings source
set (molepy)
if (USE_BOOSTPYTHON_BINDINGS)
    set (molepy ${mole_SOURCE_DIR}/bindings/Deprecated/python/oofemlib.cpp)
endif ()

# Main library target
if (USE_SHARED_LIB)
    add_library (libmole SHARED ${molepy} ${LIBS})
    target_link_libraries (libmole ${EXT_LIBS})
    set_target_properties (libmole PROPERTIES OUTPUT_NAME mole)
    add_dependencies(libmole version)

    if (NOT "${LIBMOLE_EXPORT_CFLAGS}" STREQUAL "")
        set_target_properties (libmole PROPERTIES COMPILE_FLAGS ${LIBMOLE_EXPORT_CFLAGS})
    endif ()
    install (TARGETS libmole DESTINATION lib)

    # Set export definitions for object targets
    set_target_properties (core PROPERTIES COMPILE_DEFINITIONS "libmole_EXPORTS")
    if (USE_SM)
        set_target_properties (sm PROPERTIES COMPILE_DEFINITIONS "libmole_EXPORTS")
        set_target_properties (sm_elements PROPERTIES COMPILE_DEFINITIONS "libmole_EXPORTS")
        set_target_properties (sm_materials PROPERTIES COMPILE_DEFINITIONS "libmole_EXPORTS")
        if (NOT "${LIBMOLE_EXPORT_CFLAGS}" STREQUAL "")
            set_target_properties (sm PROPERTIES COMPILE_FLAGS ${LIBMOLE_EXPORT_CFLAGS})
            set_target_properties (sm_elements PROPERTIES COMPILE_FLAGS ${LIBMOLE_EXPORT_CFLAGS})
            set_target_properties (sm_materials PROPERTIES COMPILE_FLAGS ${LIBMOLE_EXPORT_CFLAGS})
        endif ()
    endif()

    generate_export_header (libmole BASE_NAME MOLE)
endif ()

# Main executable target
if (USE_SHARED_LIB)
    add_executable (mole ${mole_SOURCE_DIR}/src/molecfg.C ${mole_SOURCE_DIR}/src/main/main.C)
    add_dependencies(mole version)
    target_link_libraries (mole libmole)
else ()
    add_executable (mole ${mole_SOURCE_DIR}/src/molecfg.C ${mole_SOURCE_DIR}/src/main/main.C ${LIBS})
    add_dependencies(mole version)
    target_link_libraries (mole ${EXT_LIBS})
endif ()

install (TARGETS mole DESTINATION bin)

# MFRONT support
if (USE_MFRONT)
    find_package (MFrontGenericInterface REQUIRED)
    target_link_libraries(mole mgis::MFrontGenericInterface)
endif ()

# Additional utility targets
add_executable(libmole_benchmark ${mole_SOURCE_DIR}/src/molecfg.C ${mole_SOURCE_DIR}/src/main/benchmark.C)
add_dependencies(libmole_benchmark version)
set_target_properties(libmole_benchmark PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries (libmole_benchmark libmole benchmark)

# Example applications
add_executable(beam2d_1 ${mole_SOURCE_DIR}/src/molecfg.C ${mole_SOURCE_DIR}/bindings/oofemlib/beam2d_1.C)
add_dependencies(beam2d_1 version)
set_target_properties(beam2d_1 PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries (beam2d_1 libmole)

add_executable(hexgrid ${mole_SOURCE_DIR}/src/molecfg.C ${mole_SOURCE_DIR}/bindings/oofemlib/hexgrid.C)
set_target_properties(hexgrid PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries (hexgrid libmole)

add_executable(dream3d_analysis ${mole_SOURCE_DIR}/src/molecfg.C ${mole_SOURCE_DIR}/bindings/oofemlib/dream3d_analysis.C)
set_target_properties(dream3d_analysis PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries(dream3d_analysis libmole)

# Code analysis tools
add_custom_target(cppcheck)
set_target_properties(cppcheck PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_custom_command(TARGET cppcheck 
    POST_BUILD
    COMMAND cppcheck "--xml" "--template=gcc" "--enable=all" 
    "-I${oofem_BINARY_DIR}/"
    "-I${oofem_SOURCE_DIR}/src/oofemlib/"
    "-I${oofem_SOURCE_DIR}/src/sm/"
    "-DDEBUG"
    ${oofem_SOURCE_DIR}/src/oofemlib/
    ${oofem_SOURCE_DIR}/src/sm/
    ${oofem_SOURCE_DIR}/src/main/
    "2>" "cppcheck.xml"
    COMMENT "Running cppcheck on entire source"
    )


# =============================================================================
# DOCUMENTATION (Doxygen removed - use external tools if needed)
# =============================================================================

# =============================================================================
# TESTING
# =============================================================================

set (mole_TEST_DIR "${mole_SOURCE_DIR}/tests/")
set (mole_cmd  $<TARGET_FILE:mole>)
file(TO_CMAKE_PATH "${mole_TEST_DIR}/valgrind.supp" MEMORYCHECK_SUPPRESSIONS_FILE)
include (CTest)

# Parallel tests
if (USE_MPI_PARALLEL)
    set (par_dir ${mole_TEST_DIR}/partests)
    file (GLOB parallel_tests RELATIVE "${par_dir}" "${par_dir}/*/")
    foreach (case ${parallel_tests})
        file (GLOB files "${par_dir}/${case}/${case}.mole.in.*")
        list (LENGTH files num_files)
        add_test (NAME "partest_${case}" WORKING_DIRECTORY ${par_dir}/${case} COMMAND "mpirun" "--oversubscribe" "-np" ${num_files} ${mole_cmd} "-p" "-f" ${case}.mole.in "-ksp_type" "cg")
    endforeach (case)
    set_tests_properties(partest_brazil_2d_nl7 PROPERTIES TIMEOUT 2500)
endif ()

# Sequential tests
if (USE_SM)
    file (GLOB sm_tests RELATIVE "${mole_TEST_DIR}/sm" "${mole_TEST_DIR}/sm/*.in")
    foreach (case ${sm_tests})
        add_test (NAME "test_sm_${case}" WORKING_DIRECTORY ${mole_TEST_DIR}/sm COMMAND ${mole_cmd} "-f" ${case})
    endforeach (case)

    file (GLOB sm_tests RELATIVE "${mole_TEST_DIR}/sm" "${mole_TEST_DIR}/sm/*.sh")
    foreach (case ${sm_tests})
        add_test (NAME "test_sm_${case}" WORKING_DIRECTORY ${mole_TEST_DIR}/sm COMMAND bash ${case} ${mole_cmd})
    endforeach (case)
endif ()


# Additional test modules
if (USE_TM AND USE_SM)
    file (GLOB tmsm_tests RELATIVE "${mole_TEST_DIR}/tmsm" "${mole_TEST_DIR}/tmsm/*.in")
    foreach (case ${tmsm_tests})
        add_test (NAME "test_tmsm_${case}" WORKING_DIRECTORY ${mole_TEST_DIR}/tmsm COMMAND ${mole_cmd} "-f" ${case})
    endforeach (case)
endif()

if (USE_TM AND USE_CEMHYD)
    file (GLOB tmcemhyd_tests RELATIVE "${mole_TEST_DIR}/tmcemhyd" "${mole_TEST_DIR}/tmcemhyd/*.in")
    foreach (case ${tmcemhyd_tests})
        add_test (NAME "test_tm_${case}" WORKING_DIRECTORY ${mole_TEST_DIR}/tmcemhyd COMMAND ${mole_cmd} "-f" ${case})
    endforeach (case)
endif()

if (USE_SM AND USE_MFRONT)
    file (GLOB smmfront_tests RELATIVE "${mole_TEST_DIR}/smmfront" "${mole_TEST_DIR}/smmfront/*.in")
    foreach (case ${smmfront_tests})
        add_test (NAME "test_sm_${case}" WORKING_DIRECTORY ${mole_TEST_DIR}/smmfront COMMAND ${mole_cmd} "-f" ${case})
    endforeach (case)
endif()

# Benchmarks
set (mole_BENCHMARK_DIR "${mole_SOURCE_DIR}/tests/benchmark")

if (USE_TM AND USE_SM AND USE_DSS AND USE_IML)
    file (GLOB tmsmdssiml_benchmark RELATIVE "${mole_BENCHMARK_DIR}/tmsmdssiml" "${mole_BENCHMARK_DIR}/tmsmdssiml/*.in")
    foreach (case ${tmsmdssiml_benchmark})
        add_test (NAME "benchmark_tmsm_${case}" WORKING_DIRECTORY ${mole_BENCHMARK_DIR}/tmsmdssiml COMMAND ${mole_cmd} "-f" ${case})
    endforeach (case)
endif()

if (USE_SM)
    file (GLOB sm_benchmark RELATIVE "${mole_BENCHMARK_DIR}/sm" "${mole_BENCHMARK_DIR}/sm/*.in")
    foreach (case ${sm_benchmark})
        add_test (NAME "benchmark_sm_${case}" WORKING_DIRECTORY ${mole_BENCHMARK_DIR}/sm COMMAND ${mole_cmd} "-f" ${case})
    endforeach (case)
endif()

# Test targets
ProcessorCount(N)
if (N EQUAL 0)
    set (N 1)
endif ()
if (USE_OPENMP)
    set (N 1)
endif ()

if (MSVC)
    add_custom_target (tests      COMMAND ${CMAKE_CTEST_COMMAND} "-j${N}" "-R" "^test" "-C" "Debug")
    add_custom_target (partests   COMMAND ${CMAKE_CTEST_COMMAND} "-R" "^partest" "-C" "Debug")
    add_custom_target (benchmarks COMMAND ${CMAKE_CTEST_COMMAND} "-j${N}" "-R" "^benchmark" "-C" "Debug")
else ()
    add_custom_target (tests      COMMAND ${CMAKE_CTEST_COMMAND} "-j${N}" "-R" "^test")
    add_custom_target (partests   COMMAND ${CMAKE_CTEST_COMMAND} "-R" "^partest")
    add_custom_target (benchmarks COMMAND ${CMAKE_CTEST_COMMAND} "-j${N}" "-R" "^benchmark")
endif ()

# Python bindings tests
if (USE_PYBIND_BINDINGS)
    add_test (NAME "test_python_pybind11_bindings" WORKING_DIRECTORY ${mole_SOURCE_DIR}/bindings/python COMMAND ${Python_EXECUTABLE} "-m" "pytest" ${mole_SOURCE_DIR}/bindings/python)
    set_tests_properties("test_python_pybind11_bindings" PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}")
endif()


# =============================================================================
# PACKAGING
# =============================================================================

# Package version
set (CPACK_PACKAGE_VERSION_MAJOR ${mole_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${mole_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${mole_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

# Package settings
set (CPACK_PACKAGE_NAME "mole")
set (CPACK_PACKAGE_VENDOR "MOLE development team")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Multi-physics Object-oriented Lattice Element solver")
set (CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.LGPLv2.1")
set (CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set (CPACK_PACKAGE_EXECUTABLES "mole")
set (CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CMAKE_SYSTEM_PROCESSOR})

# Package generators
if (WIN32)
    set(CPACK_GENERATOR "ZIP")
else ()
    set (CPACK_GENERATOR "TGZ;DEB")
endif ()

# Source package
set (CPACK_SOURCE_GENERATOR "ZIP")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "mole-${CPACK_PACKAGE_VERSION}")
set (CPACK_SOURCE_IGNORE_FILES "~$;/build/;tags;cscope.*;.*\\\\.out$;\\\\.out\\\\.;/\\\\..*;\\\\.kdev4$;do_release;release_filter\\\\.pl")

# Debian package options
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Mikael Öhman <micketeer@gmail.com>")
set (CPACK_DEBIAN_PACKAGE_SECTION "Mathematics")
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}+sid1")

include (CPack)