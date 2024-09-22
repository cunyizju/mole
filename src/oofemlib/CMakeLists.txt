# Much to organize
set (core_unsorted
    classfactory.C
    

    
    
    # Semi sorted:
    

     


    # Deprecated?

    #


    # 

    )

set(initial
    initial/initmodulemanager.C
    initial/initmodule.C
    initial/initialcondition.C
)

set(input
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
set(contact
    contact/contactmanager.C
    contact/contactdefinition.C
    contact/contactelement.C
)

set(math
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

if (USE_SPOOLES)
    list (APPEND core_nm 
    solvers/spoolessparsemtrx.C
    solvers/spoolessolver.C)
endif ()

if (USE_IML)
    list (APPEND core_unsorted
        iml/dyncomprow.C iml/dyncompcol.C
        iml/precond.C iml/voidprecond.C iml/icprecond.C
        iml/iluprecond.C
        iml/ilucomprowprecond.C
        iml/diagpre.C
        iml/imlsolver.C
        )
endif ()


if (USE_SUPERLU_MT)
  list (APPEND core_nm solvers/superlusolver.C)
endif ()

set(nonlocal
    nonlocal/nonlocalbarrier.C
    nonlocal/nonlocalmaterialext.C
)

set (core_oofeg 
    oofeg/oofeggraphiccontext.C)

set (core_nodalrecovery
    nodalrecovery/nodalrecoverymodel.C
    nodalrecovery/zznodalrecoverymodel.C
    nodalrecovery/nodalaveragingrecoverymodel.C
    nodalrecovery/sprnodalrecoverymodel.C
    )

set (core_dofs
    dofs/dof.C
    dofs/masterdof.C
    dofs/simpleslavedof.C
    dofs/slavedof.C
    dofs/activedof.C
    dofs/dofmanvalfield.C
    )

set (core_nm
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

######## External solvers #########
if (USE_PETSC)
    list (APPEND core_nm 
    solvers/petscsolver.C 
    solvers/petscsparsemtrx.C)
endif ()

if (USE_INTEL_MKL_PARDISO OR USE_ONEAPI)
    message (STATUS "    MKL Solver is enabled for compilation")

    list (APPEND core_nm 
    solvers/mklpardisosolver.C)
endif ()

if (USE_PARDISO_ORG)
  list (APPEND core_nm 
  solvers/pardisoprojectorgsolver.C)
endif ()

if (USE_SLEPC)
    list (APPEND core_nm 
    solvers/slepcsolver.C)
endif ()

set (core_fields
    fields/fieldmanager.C
    fields/field.C
    fields/primaryfield.C
    fields/intvarfield.C
    fields/maskedprimaryfield.C
    fields/dofdistributedprimaryfield.C
    fields/eigenvectorprimaryfield.C
    fields/uniformgridfield.C
    )

set (core_ltf
    ltf/function.C
    ltf/scalarfunction.C
    ltf/constantfunction.C
    ltf/heavisidetimefunction.C
    ltf/peakfunction.C
    ltf/piecewiselinfunction.C
    ltf/periodicpiecewiselinfunction.C
    ltf/calculatorfunction.C
    ltf/localgaussianrandomfunction.C
    ltf/interpolatingfunction.C
    )

set (core_cs
    cs/crosssection.C
    cs/emptycs.C
    cs/dummycrosssection.C
    )

set (core_td
    )

set (core_dofman
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

set (core_material
    material/material.C
    material/dummymaterial.C
    material/materialmappingalgorithm.C
    material/fracturemanager.C
    material/homogenize.C
    material/randommaterialext.C
    )

set (core_export
    export/outputmanager.C
    export/exportmodule.C
    export/exportmodulemanager.C
    export/exportregion.C
    export/outputexportmodule.C
    export/datastream.C
    export/errorcheckingexportmodule.C
    export/vtkexportmodule.C
    export/vtkbaseexportmodule.C
    export/vtkxmlexportmodule.C
    export/vtkmemoryexportmodule.C
    export/vtkxmlperiodicexportmodule.C
#    export/vtkxmllatticeexportmodule.C
    export/vtkpfemexportmodule.C
    export/vtkxfemexportmodule.C
#    export/homexportmodule.C
    export/matlabexportmodule.C
    export/gpexportmodule.C
    export/solutionstatusexportmodule.C
    export/vtkhdf5exportmodule.C
    )

set (core_monitors
    monitor/monitormanager.C
    monitor/monitor.C
    )

set (core_iga
    iga/iga.C
    iga/feibspline.C
    iga/feinurbs.C
    iga/feitspline.C
    )

set (core_arrays
    arrays/intarray.C
    arrays/floatarray.C
    arrays/floatmatrix.C
    )

set (core_engng
    engng/engngm.C
    engng/staggeredproblem.C
    engng/dummyengngm.C
    )

set (core_fei
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

set (core_xfem
    xfem/delaunay.C
    xfem/xfemelementinterface.C
    xfem/enrichmentfunction.C
    xfem/xfemmanager.C
    xfem/enrichmentitem.C
    xfem/geometrybasedei.C
    xfem/hybridei.C
    xfem/directei.C
    xfem/listbasedei.C
    xfem/inclusion.C
    xfem/patchintegrationrule.C
    xfem/propagationlaw.C
    xfem/XFEMDebugTools.C
    xfem/enrichmentfronts/enrichmentfront.C
    xfem/enrichmentfronts/enrichmentfrontdonothing.C
    xfem/enrichmentfronts/enrichmentfrontextend.C
    xfem/enrichmentfronts/enrichmentfrontreducefront.C
    xfem/enrichmentfronts/enrichmentfrontlinbranchfuncradius.C
    xfem/enrichmentfronts/enrichmentfrontlinbranchfunconeel.C
    xfem/enrichmentfronts/enrichmentfrontcohesivebranchfunconeel.C
    xfem/enrichmentfronts/enrichmentfrontintersection.C
    xfem/integrationrules/discsegintegrationrule.C
    xfem/xfemtolerances.C
    xfem/nucleationcriterion.C
    )

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

set (core_bc
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
    )

if (USE_PYTHON_EXTENSION)
    list (APPEND core_bc bc/userdefdirichletbc.C)
    list (APPEND core_unsorted utility/pythonexpression.C)
endif ()

if (USE_PYBIND_BINDINGS)
    list (APPEND core_unsorted utility/pythonfield.C)
endif ()



set (core_mapping
    mapping/mmaclosestiptransfer.C
    mapping/mmashapefunctprojection.C
    mapping/mmaleastsquareprojection.C
    mapping/mmacontainingelementprojection.C
    mapping/matstatmapperint.C
    )

set (core_mesher
    mesher/t3dinterface.C
    mesher/targe2interface.C
    mesher/freeminterface.C
    mesher/subdivision.C
    mesher/trianglemesherinterface.C
    mesher/spatiallocalizer.C
    mesher/dummylocalizer.C
    mesher/octreelocalizer.C
    mesher/remeshingcrit.C
    mesher/unstructuredgridfield.C
    )

set (core
    ${input}
    ${initial}
    ${contact}
    ${error}
    ${math}
    ${utility}
    ${nonlocal}
    ${core_unsorted}
    ${core_nodalrecovery}
    ${core_dofs}
    ${core_nm}
    ${core_td}
    ${core_fields}
    ${core_dofman}
    ${core_material}
    ${core_export}
    ${core_monitors}
    ${core_ltf}
    ${core_cs}
    ${core_arrays}
    ${core_engng}
    ${core_fei}
    ${core_xfem}
    ${core_bc}
    ${core_iga}
    ${core_mesher}
    ${core_mapping}
    )

if (USE_PARALLEL)
    list (APPEND core ${core_parallel})
endif ()

if (USE_OOFEG)
    list (APPEND core ${core_oofeg})
endif ()


add_library (core
    ${LIB_TYPE}
    ${core}
    )
