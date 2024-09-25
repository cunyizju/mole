# engng
file(GLOB engng_SRCS engng/*.C)
set(engng ${engng_SRCS})

# input
# file(GLOB input_SRCS input/*.C)
# set(input ${input_SRCS})

set(input
    input/initmodulemanager.C
    input/inputrecord.C 
    input/oofemtxtinputrecord.C
    input/dynamicinputrecord.C
    input/domain.C
    input/assemblercallback.C
    input/element.C
    input/geotoolbox.C
    input/geometry.C
    input/connectivitytable.C
    input/dynamicdatareader.C
    input/oofemtxtdatareader.C
    input/logger.C
    input/compcol.C
    input/femcmpnn.C  
    input/loadbalancer.C
    input/cltypes.C 
    input/smoothednodalintvarfield.C
    input/sloangraph.C 
    input/sloangraphnode.C 
    input/sloanlevelstruct.C
    input/eleminterpunknownmapper.C
    input/primaryunknownmapper.C 
)

set (dofman
    dofs/dof.C
    dofs/masterdof.C
    dofs/simpleslavedof.C
    dofs/slavedof.C
    dofs/activedof.C
    dofs/dofmanvalfield.C
    dofman/dofmanager.C
    dofman/node.C
    dofman/slavenode.C
    dofman/generalslavenode.C
    dofman/hangingnode.C
    dofman/qcnode.C
    dofman/rigidarmnode.C
    dofman/elementinternaldofman.C
    dofman/elementside.C
    dofman/latticedirichletcouplingnode.C
    dofman/latticeneumanncouplingnode.C
    )

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

set (material
    material/material.C
    material/dummymaterial.C
    material/materialmappingalgorithm.C
    material/fracturemanager.C
    material/homogenize.C
    material/randommaterialext.C
    nonlocal/nonlocalbarrier.C
    nonlocal/nonlocalmaterialext.C
    )


set (bc
    bc/generalboundarycondition.C
    bc/boundarycondition.C
    bc/prescribedgradienthomogenization.C
    bc/prescribedgradient.C
    bc/prescribedgradientbcneumann.C
    bc/prescribedgradientbcweak.C
    bc/prescribedgradientbcweakperiodic.C
    bc/prescribedgradientbcweakdirichlet.C
    bc/prescribedgradientbcperiodic.C
    bc/mixedgradientpressurebc.C
    bc/mixedgradientpressuredirichlet.C
    bc/mixedgradientpressureneumann.C
    bc/mixedgradientpressureweakperiodic.C
    bc/load.C
    bc/boundaryload.C
    bc/pointload.C
    bc/linearedgeload.C
    bc/constantedgeload.C
    bc/constantsurfaceload.C
    bc/nodalload.C
    bc/deadweight.C
    bc/reinforcement.C
    bc/surfacetensionbc.C
    bc/linearconstraintbc.C
    bc/weakperiodicbc.C
    bc/bctracker.C
    bc/neumannmomentload.C
    bc/prescribedmean.C
    initial/initmodule.C
    initial/initialcondition.C
    )

set (fields
    fields/fieldmanager.C
    fields/field.C
    fields/primaryfield.C
    fields/intvarfield.C
    fields/maskedprimaryfield.C
    fields/dofdistributedprimaryfield.C
    fields/eigenvectorprimaryfield.C
    fields/uniformgridfield.C
    )

# loading functions
file(GLOB ltf_SRCS ltf/*.C)
set(core_ltf ${ltf_SRCS})

set (crosssection
    cs/crosssection.C
    cs/emptycs.C
    cs/dummycrosssection.C
    )
# loading functions
file(GLOB export_SRCS export/*.C)
set(export ${export_SRCS})

# monitor
file(GLOB monitor_SRCS monitor/*.C)
set(monitors ${monitor_SRCS})

# mesh
file(GLOB mesh_SRCS mesher/*.C)
set(mesh ${mesh_SRCS})

set (fei
    fei/feinterpol.C
    fei/feinterpol1d.C
    fei/feinterpol2d.C
    fei/feinterpol3d.C
    fei/fei1dlin.C
    fei/fei1dquad.C
    fei/fei1dhermite.C
    fei/fei2dtrlin.C
    fei/fei2dtrquad.C
    fei/fei2dlinehermite.C
    fei/fei2dlinelin.C
    fei/fei2dlinequad.C
    fei/fei2dquadconst.C
    fei/fei2dquadlin.C
    fei/fei2dquadquad.C
    fei/fei2dquadbiquad.C
    fei/fei3dlinelin.C
    fei/fei3dtrlin.C
    fei/fei3dquadlin.C
    fei/fei3dtrquad.C
    fei/fei3dtetlin.C
    fei/fei3dtetquad.C
    fei/fei3dhexalin.C
    fei/fei3dhexaquad.C
    fei/fei3dhexatriquad.C
    fei/fei3dwedgelin.C 
    fei/fei3dwedgequad.C
    fei/fei3dhexaconst.C
    )

set(contact
    contact/contactmanager.C
    contact/contactdefinition.C
    contact/contactelement.C
)

# mapping
file(GLOB mapping_SRCS mapping/*.C)
set(mapping ${mapping_SRCS})

set(math
    math/intarray.C
    math/floatarray.C
    math/floatmatrix.C
    math/mathfem.C
    math/grid.C
    math/gausspoint.C
    math/rowcol.C
    math/skyline.C
    math/skylineu.C
    math/integrationrule.C
    math/gaussintegrationrule.C
    math/inverseit.C
    math/subspaceit.C
    math/gjacobi.C
    math/lobattoir.C
    math/symcompcol.C
    math/ldltfact.C
    math/solutionbasedshapefunction.C
)

set(error
    error/error.C 
    error/errorestimator.C
    error/meshqualityerrorestimator.C
)

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

set (nodalrecovery
    nodalrecovery/nodalrecoverymodel.C
    nodalrecovery/zznodalrecoverymodel.C
    nodalrecovery/nodalaveragingrecoverymodel.C
    nodalrecovery/sprnodalrecoverymodel.C
    )

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
    list (APPEND solvers
        iml/dyncomprow.C iml/dyncompcol.C
        iml/precond.C iml/voidprecond.C iml/icprecond.C
        iml/iluprecond.C
        iml/ilucomprowprecond.C
        iml/diagpre.C
        iml/imlsolver.C
        )
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