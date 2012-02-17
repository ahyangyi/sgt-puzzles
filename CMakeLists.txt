# Project Needs a name ofcourse
project(Knotplasm)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(
    Knotplasm_SRCS
    Knotplasm.cpp Knotwrap.cpp Knotconfig.cpp
    Knotrenderer-primitive.cpp
    drawing.c dsf.c
    list.c malloc.c midend.c misc.c no-icon.c printing.c ps.c random.c version.c
    galaxies.c
    )
# We will migrate to using list.c after finishing playing with galaxies.
add_definitions (-DCOMBINED -DKNOTPLASM_DEBUG)

# set(galaxies_SRC drawing dsf galaxies malloc midend misc no-icon printing ps random version)
# add_library(galaxies STATIC ${galaxies_SRC})
include("Knotplasm.dep")

# Now make sure all files get to the right place
kde4_add_plugin(Knotplasm ${Knotplasm_SRCS})

target_link_libraries(Knotplasm 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
 
install(TARGETS Knotplasm
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES Knotplasm.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
