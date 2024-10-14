# elasticity
file(GLOB elas_SRCS Elasticity/*.C)
set(elasticity ${elas_SRCS})

# plasticity
file(GLOB plas_SRCS Plasticity/*.C)
set(plasticity ${plas_SRCS})

# 1D
file(GLOB D1_SRCS 1D/*.C)
set(D1 ${D1_SRCS})

# Large strain
file(GLOB largestrain_SRCS Largestrain/*.C)
set(largestrain ${largestrain_SRCS})

# Hyperelastic
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

# Multiscale
file(GLOB multiscale_SRCS Multiscale/*.C)
set(multiscale ${multiscale_SRCS})

# Lattice
file(GLOB lattice_SRCS Lattice/*.C)
set(lattice ${lattice_SRCS})

# Structural
file(GLOB structural_SRCS Structural/*.C)
list(REMOVE_ITEM structural_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Structural/structuralpythonmaterial.C")
set(structural ${structural_SRCS})

# Miscellaneous
file(GLOB misc_SRCS Misc/*.C)
list(REMOVE_ITEM misc_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Misc/mfrontusermaterial.C")
set(miscellaneous ${misc_SRCS})

if (USE_MFRONT)
    list (APPEND miscellaneous Misc/mfrontusermaterial.C)
endif ()
    
if (USE_BOOSTPYTHON_BINDINGS AND USE_PYTHON_EXTENSION)
    list (APPEND miscellaneous Structural/structuralpythonmaterial.C)
endif ()

# Interface
file(GLOB interface_SRCS Interface/*.C Interface/Deprecated/*.c)
set(interface ${interface_SRCS})

# obsolete
file(GLOB obsolete_SRCS Obsolete/*.C)
set(obsolete ${obsolete_SRCS})

set (sm_materials
    ${elasticity}
    ${hyperelastic}
    ${plasticity}
    ${damage}
    ${concrete}
    ${microplane}
    ${multiscale}
    ${lattice}
    ${structural}
    ${interface}
    ${sm_status}
    ${miscellaneous}
    ${largestrain}
    ${D1}
    ${obsolete}
    )

add_library(sm_materials
    ${LIB_TYPE}
    ${sm_materials}
    )