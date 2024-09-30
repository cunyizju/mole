# elasticity
file(GLOB elas_SRCS Elasticity/*.C)
set(elasticity ${elas_SRCS})

# plasticity
file(GLOB plas_SRCS Plasticity/*.C)
set(plasticity ${plas_SRCS})

set (sm_material
    ABAQUS/abaqususermaterial.C
    structuralmaterial.C
    structuralfe2material.C
    structuralslipfe2material.C
    dustmat.C
    lsmastermat.C
    lsmastermatgrad.C
    simplevitrificationmaterial.C
    structmatsettable.C
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

# Lattice
file(GLOB lattice_SRCS Lattice/*.C)
set(lattice ${lattice_SRCS})

if (USE_MFRONT)
    list (APPEND sm_material mfrontusermaterial.C)
endif ()
    
if (USE_BOOSTPYTHON_BINDINGS AND USE_PYTHON_EXTENSION)
    list (APPEND sm_material structuralpythonmaterial.C)
endif ()

set (sm_status
    structuralms.C
    InterfaceMaterials/structuralinterfacematerialstatus.C
    deformationtheorymaterial.C
    )

set (sm_interface_materials
    InterfaceMaterials/structuralinterfacematerial.C
    InterfaceMaterials/bondceb.C
    InterfaceMaterials/cohint.C
    InterfaceMaterials/intmatelastic.C
    InterfaceMaterials/intmatbilinczfagerstrom.C
    InterfaceMaterials/intmatbilinczjansson.C
    InterfaceMaterials/intmatbilinearcz.C
    InterfaceMaterials/intmatbilinczelastic.C
    InterfaceMaterials/intmatbilinczfagerstromrate.C
    InterfaceMaterials/intmatisodamage.C
    InterfaceMaterials/intmatisodamagetable.C
    InterfaceMaterials/intmatcoulombcontact.C
    InterfaceMaterials/intmatdummycz.C    
    InterfaceMaterials/Deprecated/isointerfacedamage01.C
    InterfaceMaterials/Deprecated/isointerfacedamage02.C
    InterfaceMaterials/Deprecated/simpleinterfacemat.C
    InterfaceMaterials/masonry02.C
    InterfaceMaterials/mat_cebfip90.C
    InterfaceMaterials/linkslip.C
    )

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
    ${lattice}
    ${sm_interface_materials}
    ${sm_status}
    ${sm_obsolete}
    )

add_library(sm_materials
    ${LIB_TYPE}
    ${sm_materials}
    )