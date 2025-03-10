cmake_minimum_required (VERSION 3.1)
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)
include (GenerateExportHeader)
include (ProcessorCount)
include (GenerateVersion)


# Use ccache if available. Needs to be performed early for it to work with cmake.
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project (oofem CXX C)
set (oofem_VERSION_MAJOR 2)
set (oofem_VERSION_MINOR 6)
set (oofem_VERSION_PATCH 0) # Unused
set (oofem_HOMEPAGE "http://www.oofem.org")
set (bp_HOMEPAGE "http://ksm.fsv.cvut.cz/%7Ebp/bp.html")
string(TIMESTAMP YEAR "%Y")
set (oofem_COPYRIGHT "Copyright (C) 1994-${YEAR} Borek Patzak")
set (oofem_EMAIL "oofem@fsv.cvut.cz")

set(CMAKE_VERBOSE_MAKEFILE ON)

# Default is debug build
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif ()

function(add_git_submodule dir)
# add a Git submodule directory to CMake, assuming the
# Git submodule directory is a CMake project.
#
# Usage: in CMakeLists.txt
# 
# include(AddGitSubmodule.cmake)
# add_git_submodule(mysubmod_dir)
  find_package(Git REQUIRED)
  if(NOT EXISTS ${dir}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${dir}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      #COMMAND_ERROR_IS_FATAL ANY #cmake 3.19
      )
  endif()
  add_subdirectory(${dir})
endfunction(add_git_submodule)


if (MSVC)
    add_definitions (-D_USE_MATH_DEFINES)
    set (CMAKE_REQUIRED_DEFINITIONS "-D_USE_MATH_DEFINES")
    # I'm not sure we should block these warnings or not:
    # C4800 Conversion from int to bool performance warning (we should probably be using bool in most of these places though).
    # C4127 Conditional expression is constant: Most of these uses are intentional (debugging code etc.)
    # C4267 size_t to int conversion. Most of these are indices, but many places can be replaced by a for-each loop as well.
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -wd4786 -wd4100 -wd4996 -wd4706 -wd4512 -wd4251 -wd4800")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
endif ()

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS_DEBUG "-DDEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -Wsuggest-override ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
set (CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_PROFILING} -O2 -pg")

#######################################################################
######################## Custom paths #################################
#######################################################################
set (TINYXML2_DIR CACHE PATH "Manual TinyXML-2 directory (CEMHYD only)")
set (PETSC_DIR CACHE PATH "/home/licunyi/projects/petsc")
set (PETSC_ARCH CACHE STRING "linux-gnu")
set (MPI_DIR CACHE PATH "Manual MPI directory")
set (VTK_DIR CACHE PATH "Manual VTK directory (For binary VTU export)")
set (LAPACK_DIR CACHE PATH "Manual LAPACK directory")
set (SUPERLU_MT_DIR CACHE PATH "Manual SuperLU_MT directory")
set (SPOOLES_DIR CACHE PATH "Manual SPOOLES directory")
set (METIS_DIR CACHE PATH "Manual Metis directory (Load balancing)")
set (PARMETIS_DIR CACHE PATH "Manual ParMetis directory (Load balancing)")
#intel mkl pardiso
set (INTEL_MKL_PARDISO_DIR CACHE PATH "Manual mkl pardiso directory (sparse direct solver)")
set (INTEL_ONEAPI_DIR CACHE PATH "Manual path to oneAPI install directory (sparse direct solver)")
#pardiso-project.org
set (PARDISO_ORG_DIR CACHE PATH "Manual pardiso-project.org directory (sparse direct solver)")
set (PARDISO_ORG_LIB CACHE PATH "Manual pardiso-project.org library's name with complete path (sparse direct solver)")
#set (BOOST_DIR CACHE PATH "Manual Boost directory")
set (PYBIND_DIR CACHE PATH "Manual pybind11 directory")

#######################################################################
######################## Options ######################################
#######################################################################
option (USE_SHARED_LIB "Build shared library" ON)
option (USE_MFRONT "Build shared library" OFF)

# Parallel stuff
option (USE_MPI_PARALLEL "Enable MPI-based, distributed memory parallel support" OFF)
option (USE_METIS "Enable metis support" OFF)
option (USE_PARMETIS "Enable Parmetis support" OFF)
option (USE_OPENMP "Compile with OpenMP support (for parallel assembly)" OFF)
# Solvers and such
option (USE_DSS "Enable DSS module" OFF) # No reason to use this
option (USE_IML "Enable iml++ solvers" OFF) # or this
option (USE_SPOOLES "Enable SPOOLES-solver" OFF) # or this
option (USE_INTEL_MKL_PARDISO "Enable MKLPARDISO solver support" OFF)
option (USE_ONEAPI "Enable OneMKL solver support via OneAPI" OFF)
option (USE_PARDISO_ORG "Enable PARDISO-project.org solver support" OFF)
option (USE_PETSC "Enable PETSc support (specify PETSC_DIR and/or PETSC_ARCH if needed)" ON)
option (USE_LAPACK "Use LAPACK" OFF)
option (USE_SUPERLU_MT "Use SUPERLU_MT" OFF)
# Other external libraries
option (USE_TRIANGLE "Compile with Triangle bindings" OFF)
option (USE_VTK "Enable VTK (for exporting binary VTU-files)" OFF)
#option (USE_CGAL "CGAL" OFF)
# Internal modules
option (USE_SM "Enable structural mechanics module" ON)
option (USE_CEMHYD "Enable CemHyd support" OFF)
option (USE_BOOSTPYTHON_BINDINGS "Rather obsolete, use USE_PYBIND_BINDINGS. Enable Python bidings using boost (OOFEM callable from Python)." OFF)
option (USE_PYBIND_BINDINGS "Enable Python Pybind11 bindings (OOFEM callable from Python and vice versa)." OFF)
option (USE_PYTHON_EXTENSION "Enable Python extension for exposing C++ code to python. Uses Python.h library." OFF)
option (USE_MPM "Enable experimental multiphysics module" OFF)

if (USE_MPI_PARALLEL)
    add_definitions (-D__MPI_PARALLEL_MODE)
    set (USE_MPI ON)
endif ()


# Enable coverage testing
if (CMAKE_COMPILER_IS_GNUCC)
    option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

    if (ENABLE_COVERAGE)
        add_compile_options(--coverage -O0)
        list (APPEND EXT_LIBS --coverage)
    endif()
endif ()

#######################################################################
######################## Internal libraries ###########################
#######################################################################
# I believe the subdirs should be used when including instead.
include_directories ("${oofem_SOURCE_DIR}/src")
include_directories ("${oofem_SOURCE_DIR}/src/oofemlib")
include_directories ("${oofem_BINARY_DIR}")

# Is there really any reason not to compile all of them? Some has cross dependencies.
list (APPEND LIBS $<TARGET_OBJECTS:core>)

if (USE_SM)
    include_directories ("${oofem_SOURCE_DIR}/src/sm")
    include_directories ("${oofem_SOURCE_DIR}/src/sm/Elements")
    include_directories ("${oofem_SOURCE_DIR}/src/sm/Materials")
    add_definitions (-D__SM_MODULE)
    list (APPEND LIBS $<TARGET_OBJECTS:sm> $<TARGET_OBJECTS:sm_elements> $<TARGET_OBJECTS:sm_materials>)
    list (APPEND MODULE_LIST "sm" "sm_elements" "sm_materials")

    if (NOT MSVC AND NOT MINGW)
        find_library (DL_LIB dl) 
        list (APPEND EXT_LIBS ${DL_LIB})
    endif ()
endif ()

if (USE_DSS)
    include_directories ("${oofem_SOURCE_DIR}/src/dss")
    list (APPEND LIBS $<TARGET_OBJECTS:dss>)
    list (APPEND MODULE_LIST "dss")
endif ()

if (USE_OPENMP)
    include (FindOpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        list (APPEND MODULE_LIST "openmp")
    endif ()
endif ()

#######################################################################
######################## External libraries ###########################
#######################################################################

if (USE_LAPACK)
    set (CMAKE_LIBRARY_DIR ${LAPACK_DIR})
    find_package (LAPACK REQUIRED)
    list (APPEND EXT_LIBS ${LAPACK_LIBRARIES})
    list (APPEND MODULE_LIST "LAPACK")
    add_definitions (-D__LAPACK_MODULE)
endif ()


## There is a conflict on set.h as defined by libstdc and superlu_mt 3.1
## Therefore it is assumed that superlu include files
## are included using #include "SUPERLU_MT/include"
## The ${SUPERLU_MT_DIR} is added into compiler include path
## Therefore SUPERLU_MT/include has to be manually added pointing to
## ${SUPERLU_MT__DIR}/src directory
if (USE_SUPERLU_MT)
    set (CMAKE_LIBRARY_DIR ${SUPERLU_MT_DIR})
    #find_package (SuperLU REQUIRED)
    find_package(BLAS REQUIRED)
    find_library (SUPERLU_MT_LIB superlu_mt_OPENMP PATH "${SUPERLU_MT_DIR}/lib")
    list (APPEND EXT_LIBS ${SUPERLU_MT_LIB} ${BLAS_LIBRARIES})
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BLAS_LINKER_FLAGS}")
    list (APPEND MODULE_LIST "SuperLU_MT")
    include_directories("${SUPERLU_MT_DIR}")
    set (CMAKE_REQUIRED_INCLUDES "${SUPERLU_MT_DIR}")
    add_definitions (-D__SuperLU_MT_MODULE)
    check_include_file_cxx ("${SUPERLU_MT_DIR}/include/slu_mt_ddefs.h" HAVE_SUPERLU_MT_DDEFS_H)
    if (NOT HAVE_SUPERLU_MT_DDEFS_H)
        message (FATAL_ERROR "Necessary SUPERLU_MT headers not found; refer to CMakeLists.txt for details")
    endif ()
endif ()

if (USE_VTK)
    set (CMAKE_LIBRARY_DIR ${VTK_DIR})
    find_package (VTK REQUIRED NO_MODULE)
    include (${VTK_USE_FILE}) # Sets necessary includes_directories
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
    set (CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_COMPILES_FLAGS} -fopenmp")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp" ) 
    list (APPEND MODULE_LIST "pardiso-project.org")
endif ()

if (USE_INTEL_MKL_PARDISO)
    # Find the actual libraries:
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
    #list (APPEND EXT_LIBS ${MKLPARDISO_LIB})
    list (APPEND MODULE_LIST "MKLPARDISO")
endif ()

if (USE_ONEAPI)
    if (USE_INTEL_MKL_PARDISO)
        message (FATAL_ERROR "Can't use both OneMKL and classic Intel MKL (disable USE_INTEL_MKL_PARDISO option)")
    endif()

    message(STATUS "Seeking OneAPI libraries...")
    if (WIN32)
        # Windows machine must use Intel OpenMP (libiomp5) for OneAPI
        list(APPEND MKL_LIBS "mkl_core" "mkl_intel_lp64" "mkl_intel_thread" "libiomp5md")
    else()
        list(APPEND MKL_LIBS "mkl_core" "mkl_intel_lp64" "mkl_intel_thread" "pthread")
    endif ()
    foreach (f ${MKL_LIBS})
        # OneAPI installation directory
        if (INTEL_ONEAPI_DIR)
            # user provided OneAPI directory
            message(STATUS "User has provided OneAPI installation directory...")
            set(oneapiDir ${INTEL_ONEAPI_DIR})
        elseif (WIN32)
            # Windows
            set(oneapiDir "C:/Program Files (x86)/Intel/oneAPI")
        else()
            # MacOS or Linux
            set(oneapiDir "/opt/intel/oneapi")
        endif()

        # library directory
        # an alternative could be obtaining from OneAPI's Environment variables
        if (WIN32)
            # Windows
            set(oneapiMklDir "${oneapiDir}/mkl/latest")
            set(oneapiMklLibDir "${oneapiMklDir}/lib/intel64")
            set(oneapiLibDir
                    "${oneapiDir}/compiler/latest/windows/compiler/lib/intel64")
        elseif(APPLE)
            # MacOS
            set(oneapiMklDir "${oneapiDir}/mkl/latest")
            set(oneapiMklLibDir "${oneapiMklDir}/lib")
            set(oneapiLibDir "")
        elseif(UNIX)
            # Linux (not tested)
            set(oneapiMklDir "${oneapiDir}/mkl/latest")
            set(oneapiMklLibDir "${oneapiMklDir}/lib/intel64_lin")
            set(oneapiLibDir "")
        endif()

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


if (USE_SLEPC)
    set (USE_PETSC ON)
endif ()


if (USE_PETSC)
    if (NOT "${PETSC_DIR}")
        # setting defaults for typical linux environments:
        #file (GLOB paths "/usr/lib/petscdir/*.*.*")
        #list (GET paths 0 PETSC_DIR)
        #message ("***************** PETSC_DIR: ${PETSC_DIR}")
    endif ()
    if (NOT "${PETSC_ARCH}")
        #file (GLOB children RELATIVE ${PETSC_DIR} "${PETSC_DIR}/*")
        #list (GET children 0 PETSC_ARCH)
        #message ("***************** PETSC_ARCH: ${PETSC_ARCH}")
    endif ()
    find_package (PETSc)
    # find_package(PETSc COMPONENTS CXX)
    include_directories (${PETSC_INCLUDES})
    list (APPEND EXT_LIBS ${PETSC_LIBRARIES})
    list (APPEND MODULE_LIST "PETSc")
    set (USE_MPI ON)
    add_definitions (-D__PETSC_MODULE)
endif ()

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
        set (CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILES_FLAGS} ${MPI_COMPILE_FLAGS})
        set (CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
        add_definitions (-DMPICH_IGNORE_CXX_SEEK)
        include_directories (${MPI_INCLUDE_PATH})
        list (APPEND EXT_LIBS ${MPI_LIBRARIES})
    endif (MPI_DIR)
    list (APPEND MODULE_LIST "MPI")
    # Compile and link flags?
endif ()

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
    include_directories ("${oofem_SOURCE_DIR}")
    set (CMAKE_REQUIRED_INCLUDES "${oofem_SOURCE_DIR}")
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

if (USE_PYTHON_EXTENSION OR USE_BOOSTPYTHON_BINDINGS)
    # PythonLibs in cmake is bugged, have to try and rely on PythonInterp to get the suitable major and minor versions.
    #set (CMAKE_LIBRARY_DIR ${PYTHON_DIR})
    #Print if set up manually
    message(STATUS "PYTHON_LIBRARY = ${Python3_LIBRARIES}")
    message(STATUS "PYTHON_EXECUTABLE = ${Python3_EXECUTABLE}")
    message(STATUS "PYTHON_INCLUDE_DIR = ${Python3_INCLUDE_DIRS}")
    find_package (Python3 COMPONENTS Interpreter Development)
#     find_package(PythonInterp "3" REQUIRED)
#    find_package(PythonLibs "${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}" REQUIRED)
    include_directories (${Python3_INCLUDE_DIRS})
    list (APPEND EXT_LIBS ${Python3_LIBRARIES})
    list (APPEND MODULE_LIST "Python")
    if(USE_PYTHON_EXTENSION)
        add_definitions(-D_PYTHON_EXTENSION)    
    endif ()
    
    if(USE_BOOSTPYTHON_BINDINGS)
        # require boost_python also for python stuff by itself
        find_package(Boost COMPONENTS "python-py${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}" REQUIRED)
        include_directories (${Boost_INCLUDE_DIRS})
        list (APPEND EXT_LIBS ${Boost_LIBRARIES})
        add_definitions(-D_BOOSTPYTHON_BINDINGS)
     endif ()   
endif ()


if(WIN32)
    set(PYTHON_MODULE_EXTENSION ".pyd")
else()
    set(PYTHON_MODULE_EXTENSION ".so")
endif()


if (USE_PYBIND_BINDINGS)
    #build pybind11 bindings
    find_package(Python COMPONENTS Interpreter Development)
    #find_package(pybind11 CONFIG)
    add_git_submodule(extern/pybind11)
    if (PYBIND_DIR)
        message(STATUS "PYBIND_DIR = ${PYBIND_DIR}")
        include_directories (${PYBIND_DIR})#Need also to manually set it up since it can be anywhere
    endif ()
    include_directories (${pybind11_INCLUDE_DIR} ${Python_INCLUDE_DIRS})
    list (APPEND EXT_LIBS ${Python_LIBRARIES})
    pybind11_add_module(oofempy THIN_LTO ${oofem_SOURCE_DIR}/bindings/python/oofem.cpp ${oofem_SOURCE_DIR}/src/oofemcfg.C ${PYBIND11_HEADERS})
    target_link_libraries(oofempy PRIVATE liboofem)
    set_target_properties (oofempy PROPERTIES SUFFIX "${PYTHON_MODULE_EXTENSION}")

    # Make sure pytest is found or produce a fatal error
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

#######################################################################
######################## Configuration ################################
#######################################################################
site_name (HOST_NAME)
set (HOST_TYPE "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_SYSTEM_NAME}") # This should be renamed TARGET_TYPE.
set (BUILDNAME "OOFEM(${MODULE_LIST})-${HOST_TYPE}-${HOST_NAME}")

check_include_file_cxx ("execinfo.h" HAVE_EXECINFO_H)
check_cxx_symbol_exists ("cbrt" "cmath" HAVE_CBRT)
check_cxx_symbol_exists ("isnan" "cmath" HAVE_ISNAN)
if (USE_SHARED_LIB)
    set (OOFEM_EXPORT_HEADER "#include \"oofem_export.h\"")
    # For consistant library on linux and windows platforms.
    #add_compiler_export_flags (LIBOOFEM_EXPORT_CFLAGS)
else ()
    set (OOFEM_EXPORT_HEADER "#define OOFEM_EXPORT\n#define OOFEM_NO_EXPORT")
endif ()
configure_file (
  "${oofem_SOURCE_DIR}/src/oofemcfg.h.in"
  "${oofem_BINARY_DIR}/oofemcfg.h"
  )

#######################################################################
######################## Targets ######################################
#######################################################################
# Subtargets
set (LIB_TYPE OBJECT)
add_subdirectory ("${oofem_SOURCE_DIR}/src/oofemlib")
if (USE_DSS)
    add_subdirectory ("${oofem_SOURCE_DIR}/src/dss")
endif ()
if (USE_SM)
    add_subdirectory ("${oofem_SOURCE_DIR}/src/sm")
    add_subdirectory ("${oofem_SOURCE_DIR}/src/sm/Elements")
    add_subdirectory ("${oofem_SOURCE_DIR}/src/sm/Materials")
endif ()

set (oofempy)
if (USE_BOOSTPYTHON_BINDINGS)
    set (oofempy ${oofem_SOURCE_DIR}/bindings/Deprecated/python/oofemlib.cpp)
endif ()


# Main targets
if (USE_SHARED_LIB)
    add_library (liboofem
        SHARED
        ${oofempy}
        ${LIBS} 
        )
    target_link_libraries (liboofem ${EXT_LIBS})
    set_target_properties (liboofem PROPERTIES OUTPUT_NAME oofem)
    add_dependencies(liboofem version)

    if (NOT "${LIBOOFEM_EXPORT_CFLAGS}" STREQUAL "")
        set_target_properties (liboofem PROPERTIES COMPILE_FLAGS ${LIBOOFEM_EXPORT_CFLAGS})
    endif ()
    install (TARGETS liboofem DESTINATION lib)

    # This should be set automatically, but I think TARGET:objs confuses cmake:
    set_target_properties (core PROPERTIES COMPILE_DEFINITIONS "liboofem_EXPORTS")

    if (USE_SM)
        set_target_properties (sm PROPERTIES COMPILE_DEFINITIONS "liboofem_EXPORTS")
        set_target_properties (sm_elements PROPERTIES COMPILE_DEFINITIONS "liboofem_EXPORTS")
        set_target_properties (sm_materials PROPERTIES COMPILE_DEFINITIONS "liboofem_EXPORTS")
        if (NOT "${LIBOOFEM_EXPORT_CFLAGS}" STREQUAL "")
            set_target_properties (sm PROPERTIES COMPILE_FLAGS ${LIBOOFEM_EXPORT_CFLAGS})
            set_target_properties (sm_elements PROPERTIES COMPILE_FLAGS ${LIBOOFEM_EXPORT_CFLAGS})
            set_target_properties (sm_materials PROPERTIES COMPILE_FLAGS ${LIBOOFEM_EXPORT_CFLAGS})
        endif ()
    endif()

    generate_export_header (liboofem
        BASE_NAME OOFEM
        )
endif ()

# OOFEM target
if (USE_SHARED_LIB) # Prefer dynamic lib if available.
  add_executable (oofem ${oofem_SOURCE_DIR}/src/oofemcfg.C ${oofem_SOURCE_DIR}/src/main/main.C)
  add_dependencies(oofem version)

  target_link_libraries (oofem liboofem)
else ()
  add_executable (oofem ${oofem_SOURCE_DIR}/src/oofemcfg.C ${oofem_SOURCE_DIR}/src/main/main.C ${LIBS})
  add_dependencies(oofem version)

  target_link_libraries (oofem ${EXT_LIBS})
endif ()

install (TARGETS oofem DESTINATION bin)

if (USE_MFRONT)
    find_package (MFrontGenericInterface REQUIRED)
    target_link_libraries(oofem mgis::MFrontGenericInterface)
endif ()

add_executable(liboofem_benchmark ${oofem_SOURCE_DIR}/src/oofemcfg.C ${oofem_SOURCE_DIR}/src/main/benchmark.C)
add_dependencies(liboofem_benchmark version)
set_target_properties(liboofem_benchmark PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries (liboofem_benchmark liboofem benchmark)

# Example of using liboofem with dynamic input record:
add_executable(beam2d_1 ${oofem_SOURCE_DIR}/src/oofemcfg.C ${oofem_SOURCE_DIR}/bindings/oofemlib/beam2d_1.C)
add_dependencies(beam2d_1 version)
set_target_properties(beam2d_1 PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries (beam2d_1 liboofem)

add_executable(hexgrid ${oofem_SOURCE_DIR}/src/oofemcfg.C ${oofem_SOURCE_DIR}/bindings/oofemlib/hexgrid.C)
set_target_properties(hexgrid PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries (hexgrid liboofem)

add_executable(dream3d_analysis ${oofem_SOURCE_DIR}/src/oofemcfg.C ${oofem_SOURCE_DIR}/bindings/oofemlib/dream3d_analysis.C)
set_target_properties(dream3d_analysis PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries(dream3d_analysis liboofem)

# CppCheck target (not built by default)
add_custom_target(cppcheck)
set_target_properties(cppcheck PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_custom_command(TARGET cppcheck 
    COMMAND cppcheck "--xml" "--template=gcc" "--enable=all" 
    "-I${oofem_BINARY_DIR}/"
    "-I${oofem_SOURCE_DIR}/src/oofemlib/"
    "-I${oofem_SOURCE_DIR}/src/sm/"
    "-DDEBUG"
    ${oofem_SOURCE_DIR}/src/oofemlib/
    ${oofem_SOURCE_DIR}/src/sm/
    ${oofem_SOURCE_DIR}/src/main/
    "2>"
    "cppcheck.xml"
    COMMENT "Running cppcheck on entire source"
    )


#######################################################################
######################## Documentation ################################
#######################################################################
find_package (Doxygen)
if (DOXYGEN_FOUND)
    set (DOXYGEN_INPUT ${oofem_BINARY_DIR}/doxygen.cfg)
    set (DOXYGEN_OUTPUT ${oofem_BINARY_DIR}/doc/api/)
    file (MAKE_DIRECTORY ${DOXYGEN_OUTPUT})

    # configure_file (
    #     "${oofem_SOURCE_DIR}/doc/refman/footer.html.in"
    #     "${oofem_BINARY_DIR}/footer.html")
    # configure_file (
    #     "${oofem_SOURCE_DIR}/doc/refman/doxygen.cfg.in"
    #     "${oofem_BINARY_DIR}/doxygen.cfg")

    add_custom_target (
        apidoc
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
        WORKING_DIRECTORY ${oofem_BINARY_DIR}
        DEPENDS)
endif (DOXYGEN_FOUND)

# configure_file (
#     "${oofem_SOURCE_DIR}/doc/include.tex.in"
#     "${oofem_BINARY_DIR}")

# TODO: Manuals not done yet
add_custom_target (
    elementlibmanual
    COMMAND pdflatex elementlibmanual --output=${oofem_BINARY_DIR}/doc/
    COMMAND pdflatex elementlibmanual --output=${oofem_BINARY_DIR}/doc/
    #COMMAND tex4ht elementlibmanual --output=${oofem_BINARY_DIR}/doc/
    WORKING_DIRECTORY ${oofem_SOURCE_DIR}/doc/elementlibmanual/
    )

#######################################################################
######################## Tests ########################################
#######################################################################
set (oofem_TEST_DIR "${oofem_SOURCE_DIR}/tests/")
set (oofem_cmd  $<TARGET_FILE:oofem>)
file(TO_CMAKE_PATH "${oofem_TEST_DIR}/valgrind.supp" MEMORYCHECK_SUPPRESSIONS_FILE)
include (CTest)

#set (CTEST_MEMORYCHECK_SUPPRESSIONS_FILE ${oofem_TEST_DIR}/tests/valgrind.supp)

if (USE_MPI_PARALLEL)
    set (par_dir ${oofem_TEST_DIR}/partests)
    file (GLOB parallel_tests RELATIVE "${par_dir}" "${par_dir}/*/")
    foreach (case ${parallel_tests})
        file (GLOB files "${par_dir}/${case}/${case}.oofem.in.*")
        list (LENGTH files num_files)
        add_test (NAME "partest_${case}" WORKING_DIRECTORY ${par_dir}/${case} COMMAND "mpirun" "--oversubscribe" "-np" ${num_files} ${oofem_cmd} "-p" "-f" ${case}.oofem.in "-ksp_type" "cg")
    endforeach (case)
    # partest_brazil_2d_nl7 requires more time on testing server than default 1500
    set_tests_properties(partest_brazil_2d_nl7 PROPERTIES TIMEOUT 2500)
endif ()

# Sequential test cases
if (USE_SM)
    file (GLOB sm_tests RELATIVE "${oofem_TEST_DIR}/sm" "${oofem_TEST_DIR}/sm/*.in")
    foreach (case ${sm_tests})
        add_test (NAME "test_sm_${case}" WORKING_DIRECTORY ${oofem_TEST_DIR}/sm COMMAND ${oofem_cmd} "-f" ${case})
    endforeach (case)

    file (GLOB sm_tests RELATIVE "${oofem_TEST_DIR}/sm" "${oofem_TEST_DIR}/sm/*.sh")
    foreach (case ${sm_tests})
        add_test (NAME "test_sm_${case}" WORKING_DIRECTORY ${oofem_TEST_DIR}/sm COMMAND bash ${case} ${oofem_cmd})
    endforeach (case)
endif ()


if (USE_TM AND USE_SM)
    file (GLOB tmsm_tests RELATIVE "${oofem_TEST_DIR}/tmsm" "${oofem_TEST_DIR}/tmsm/*.in")
    foreach (case ${tmsm_tests})
        add_test (NAME "test_tmsm_${case}" WORKING_DIRECTORY ${oofem_TEST_DIR}/tmsm COMMAND ${oofem_cmd} "-f" ${case})
    endforeach (case)
endif()

if (USE_TM AND USE_CEMHYD)
    file (GLOB tmcemhyd_tests RELATIVE "${oofem_TEST_DIR}/tmcemhyd" "${oofem_TEST_DIR}/tmcemhyd/*.in")
    foreach (case ${tmcemhyd_tests})
        add_test (NAME "test_tm_${case}" WORKING_DIRECTORY ${oofem_TEST_DIR}/tmcemhyd COMMAND ${oofem_cmd} "-f" ${case})
    endforeach (case)
endif()

if (USE_SM AND USE_MFRONT)
    file (GLOB smmfront_tests RELATIVE "${oofem_TEST_DIR}/smmfront" "${oofem_TEST_DIR}/smmfront/*.in")
    foreach (case ${smmfront_tests})
        add_test (NAME "test_sm_${case}" WORKING_DIRECTORY ${oofem_TEST_DIR}/smmfront COMMAND ${oofem_cmd} "-f" ${case})
    endforeach (case)
endif()

######################## Benchmarks ########################################

set (oofem_BENCHMARK_DIR "${oofem_SOURCE_DIR}/tests/benchmark")

if (USE_TM AND USE_SM AND USE_DSS AND USE_IML)
    file (GLOB tmsmdssiml_benchmark RELATIVE "${oofem_BENCHMARK_DIR}/tmsmdssiml" "${oofem_BENCHMARK_DIR}/tmsmdssiml/*.in")
    foreach (case ${tmsmdssiml_benchmark})
        add_test (NAME "benchmark_tmsm_${case}" WORKING_DIRECTORY ${oofem_BENCHMARK_DIR}/tmsmdssiml COMMAND ${oofem_cmd} "-f" ${case})
    endforeach (case)
endif()

if (USE_SM)
    file (GLOB sm_benchmark RELATIVE "${oofem_BENCHMARK_DIR}/sm" "${oofem_BENCHMARK_DIR}/sm/*.in")
    foreach (case ${sm_benchmark})
        add_test (NAME "benchmark_sm_${case}" WORKING_DIRECTORY ${oofem_BENCHMARK_DIR}/sm COMMAND ${oofem_cmd} "-f" ${case})
    endforeach (case)
endif()

# Custom targets for test and benchmarks (which can conveniently be accessed from IDEs)

#ProcessorCount(N)
if (N EQUAL 0)
  set (N 1)
endif ()
if (USE_OPENMP) # OPENMP will use all processors by default
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

######################## Tests of Python bindings ########################################
if (USE_PYBIND_BINDINGS)
    add_test (NAME "test_python_pybind11_bindings" WORKING_DIRECTORY ${oofem_SOURCE_DIR}/bindings/python COMMAND ${Python_EXECUTABLE} "-m" "pytest" ${oofem_SOURCE_DIR}/bindings/python)
    set_tests_properties("test_python_pybind11_bindings" PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}")
endif()


#######################################################################
######################## Packing ######################################
#######################################################################
### versions
set (CPACK_PACKAGE_VERSION_MAJOR ${oofem_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${oofem_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${oofem_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

### general settings
set (CPACK_PACKAGE_NAME "oofem")
set (CPACK_PACKAGE_VENDOR "OOFEM development team")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Object Oriented Finite Element")
#set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
set (CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.LGPLv2.1")
set (CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set (CPACK_PACKAGE_EXECUTABLES "oofem")
set (CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CMAKE_SYSTEM_PROCESSOR})

### default generators
if (WIN32)
    set(CPACK_GENERATOR "ZIP")
else ()
    set (CPACK_GENERATOR "TGZ;DEB")
endif ()

### source package (type "make source_package" to produce a zip)
set (CPACK_SOURCE_GENERATOR "ZIP")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "oofem-${CPACK_PACKAGE_VERSION}")
set (CPACK_SOURCE_IGNORE_FILES "~$;/build/;tags;cscope.*;.*\\\\.out$;\\\\.out\\\\.;/\\\\..*;\\\\.kdev4$;do_release;release_filter\\\\.pl") # Note: All hidden files are ignored

### deb package options
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Mikael Öhman <micketeer@gmail.com>")
set (CPACK_DEBIAN_PACKAGE_SECTION "Mathematics")
# autogenerate dependency information
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}+sid1")

include (CPack)