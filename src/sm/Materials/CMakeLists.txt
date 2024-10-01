# elasticity
file(GLOB elas_SRCS Elasticity/*.C)
set(elasticity ${elas_SRCS})

# plasticity
file(GLOB plas_SRCS Plasticity/*.C)
set(plasticity ${plas_SRCS})

set (sm_material
    ABAQUS/abaqususermaterial.C
    lsmastermat.C
    simplevitrificationmaterial.C
    tutorialmaterial.C
    steelrelaxmat.C
    #Mixed pressure material interface
    MixedPressure/mixedpressurematerialextensioninterface.C
    1D/isoasymm1d.C
)

# hyperelastic
file(GLOB hyperelastic_SRCS Hyperelastic/*.C)
set(hyperelastic ${hyperelastic_SRCS})

# Damage
file(GLOB damage_SRCS Damage/*.C)
set(damage ${damage_SRCS})

# Concrete
file(GLOB concrete_SRCS Concrete/*.C)
set(concrete ${concrete_SRCS})

# Microplane
file(GLOB microplane_SRCS Microplane/*.C)
set(microplane ${microplane_SRCS})

# multiscale
file(GLOB multiscale_SRCS Multiscale/*.C)
set(multiscale ${multiscale_SRCS})

# Lattice
file(GLOB lattice_SRCS Lattice/*.C)
set(lattice ${lattice_SRCS})

# Structural
file(GLOB structural_SRCS Structural/*.C)
list(REMOVE_ITEM structural_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Structural/structuralpythonmaterial.C")
set(structural ${structural_SRCS})

if (USE_MFRONT)
    list (APPEND sm_material mfrontusermaterial.C)
endif ()
    
if (USE_BOOSTPYTHON_BINDINGS AND USE_PYTHON_EXTENSION)
    list (APPEND sm_material Structural/structuralpythonmaterial.C)
endif ()

set (sm_status
    deformationtheorymaterial.C
    )

# interface
file(GLOB interface_SRCS Interface/*.C Interface/Deprecated/*.c)
set(interface ${interface_SRCS})

set (sm_obsolete
    Obsolete/steel1.C
    Obsolete/plasticmaterial.C
    Obsolete/perfectlyplasticmaterial.C
    Obsolete/j2plasticmaterial.C
    Obsolete/mplasticmaterial.C
    Obsolete/j2mplasticmaterial.C
    Obsolete/rankinepm.C
    )

set (sm_materials
    ${elasticity}
    ${hyperelastic}
    ${plasticity}
    ${damage}
    ${concrete}
    ${microplane}
    ${sm_material}
    ${multiscale}
    ${lattice}
    ${structural}
    ${interface}
    ${sm_status}
    ${sm_obsolete}
    )

add_library(sm_materials
    ${LIB_TYPE}
    ${sm_materials}
    )