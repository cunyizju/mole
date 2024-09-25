# engng
file(GLOB engng_SRCS engng/*.C)
set(engng ${engng_SRCS})

# input
file(GLOB input_SRCS input/*.C)
set(input ${input_SRCS})

# input
file(GLOB dofman_SRCS dofman/*.C)
set(dofman ${dofman_SRCS})

set (solvers
    solvers/sparselinsystemnm.C
    solvers/sparsenonlinsystemnm.C
    solvers/nrsolver.C
    solvers/dynamicrelaxationsolver.C
    solvers/linesearch.C
    solvers/calmls.C
    solvers/staggeredsolver.C
    parallel/parallelcontext.C
    solvers/timestep.C
    solvers/metastep.C 
    )

# material
file(GLOB material_SRCS material/*.C)
set(material ${material_SRCS})

# material
file(GLOB bc_SRCS bc/*.C)
set(bc ${bc_SRCS})

# loading functions
file(GLOB fields_SRCS fields/*.C)
set(fields ${fields_SRCS})

# loading functions
file(GLOB ltf_SRCS ltf/*.C)
set(core_ltf ${ltf_SRCS})

# cross section
file(GLOB cs_SRCS cs/*.C)
set(crosssection ${cs_SRCS})

# loading functions
file(GLOB export_SRCS export/*.C)
set(export ${export_SRCS})

# monitor
file(GLOB monitor_SRCS monitor/*.C)
set(monitors ${monitor_SRCS})

# mesh
file(GLOB mesh_SRCS mesher/*.C)
set(mesh ${mesh_SRCS})

# finite element
file(GLOB fei_SRCS fei/*.C)
set(fei ${fei_SRCS})

# contact
file(GLOB contact_SRCS contact/*.C)
set(contact ${contact_SRCS})

# mapping
file(GLOB mapping_SRCS mapping/*.C)
set(mapping ${mapping_SRCS})

# math
file(GLOB math_SRCS math/*.C)
set(math ${math_SRCS})

# error
file(GLOB error_SRCS error/*.C)
set(error ${error_SRCS})

set(utility
    utility/timer.C
    utility/dictionary.C
    utility/heap.C
    utility/util.C
    utility/set.C
    utility/tokenizer.C
    utility/parser.C
)

# xfem
file(GLOB xfem_SRCS xfem/*.C xfem/enrichmentfronts/*.C xfem/integrationrules/*.C)
set(xfem ${xfem_SRCS})

# iga
file(GLOB iga_SRCS iga/*.C)
set(iga ${iga_SRCS})

# nodalrecovery
file(GLOB nodalrecovery_SRCS nodalrecovery/*.C)
set(nodalrecovery ${nodalrecovery_SRCS})

######## External solvers #########
if (USE_PETSC)
    list (APPEND solvers 
    solvers/petscsolver.C 
    solvers/petscsparsemtrx.C)
endif ()

if (USE_INTEL_MKL_PARDISO OR USE_ONEAPI)
    message (STATUS "    MKL Solver is enabled for compilation")

    list (APPEND solvers 
    solvers/mklpardisosolver.C)
endif ()

if (USE_PARDISO_ORG)
  list (APPEND solvers 
  solvers/pardisoprojectorgsolver.C)
endif ()

if (USE_SLEPC)
    list (APPEND solvers 
    solvers/slepcsolver.C)
endif ()

if (USE_SPOOLES)
    list (APPEND solvers 
    solvers/spoolessparsemtrx.C
    solvers/spoolessolver.C)
endif ()

if (USE_IML)
file(GLOB iml_SRCS iml/*.C)
    list (APPEND solvers ${iml_SRCS})
endif ()

if (USE_SUPERLU_MT)
  list (APPEND solvers solvers/superlusolver.C)
endif ()

set (core_parallel
    parallel/communicator.C
    parallel/processcomm.C
    parallel/problemcomm.C
    parallel/combuff.C
    parallel/domaintransactionmanager.C
    parallel/parallelordering.C
    parallel/dyncombuff.C
    parallel/wallclockloadbalancermonitor.C
    parallel/nonlocalmatwtp.C
    )

if (USE_PARMETIS)
    list (APPEND core_parallel
    parallel/parmetisloadbalancer.C)
endif ()

if (USE_PYTHON_EXTENSION)
    list (APPEND bc bc/userdefdirichletbc.C)
    list (APPEND engng utility/pythonexpression.C)
endif ()


set (core
    ${engng}
    ${solvers}
    ${dofman}
    ${input}
    ${crosssection}
    ${bc}
    ${mesh}
    ${mapping}
    ${material}
    ${fei}
    ${xfem}
    ${error}
    ${math}
    ${utility}
    ${core_ltf}
    ${nodalrecovery}
    ${fields}
    ${export}
    ${monitors}
    ${contact}
    ${iga}
    )

if (USE_PARALLEL)
    list (APPEND core ${core_parallel})
endif ()

add_library (core
    ${LIB_TYPE}
    ${core}
    )