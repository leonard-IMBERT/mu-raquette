# -----------------------------------------------------------------------------
# Project Setup
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(mu-raquette)

# -----------------------------------------------------------------------------
# Force cpp 17
set(CMAKE_CXX_STANDARD 17)

# -----------------------------------------------------------------------------
# Initialize Geant4 package, setting VIS and UI on by default
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find root

find_package(ROOT REQUIRED RIO Net Tree Hist)

# -----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})

message(${ROOT_INCLUDE_DIRS})

# Setup multi threading

set(G4MULTITHREADED ON)

# -----------------------------------------------------------------------------
# Locate sources and headers

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# -----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(muRaquette muRaquette.cc ${sources} ${headers})
target_link_libraries(muRaquette ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# -----------------------------------------------------------------------------
# Create symlink to scripts in the source folder

set(MURAQUETTE_SCRIPTS
  vis.mac
  )

foreach(_script ${MURAQUETTE_SCRIPTS})
  file(CREATE_LINK
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPY_ON_ERROR
    )
endforeach()

# -----------------------------------------------------------------------------
# If in dev mode, create compile_commands.json

if(DEFINED ENV{MURAQUETTE_DEV})
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# -----------------------------------------------------------------------------
# Install the executable to 'bin' under CMAKE_INSTALL_PREFIX

install(TARGETS muRaquette DESTINATION bin)


