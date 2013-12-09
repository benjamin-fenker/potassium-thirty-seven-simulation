#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(K37)

#It is best to set the build type from the build.sh
#SET( CMAKE_CXX_COMPILER "g++" )
#SET( CMAKE_CXX_COMPILER "clang++" )
#SET( CMAKE_CXX_COMPILER "c++" )


#----------------------------------------------------------------------------
# Find Root so that we can link the aggregator class to it
INCLUDE(FindROOT.cmake)
INCLUDE_DIRECTORIES (${ROOT_INCLUDE_DIR})
LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/external)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/external/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/external/*.hh)

#----------------------------------------------------------------------------
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/src/auxiliary aux)
#----------------------------------------------------------------------------

set (K37_VERSION $ENV{K37_VERSION})
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/K37Config.hh.in"
	"${PROJECT_SOURCE_DIR}/K37Config.hh"
	)

configure_file (
	"${PROJECT_SOURCE_DIR}/IOconfiguration.mac.in"
	"${PROJECT_SOURCE_DIR}/IOconfiguration.mac"
	)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(K37 K37.cc ${sources} ${headers})
target_link_libraries(K37 ${Geant4_LIBRARIES} Core RIO Tree)

#----------------------------------------------------------------------------
 # Copy all scripts to the build directory, i.e. the directory in which we
# build K37. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(K37_SCRIPTS
  vis.mac
  novis.mac
  gui.mac
  IOconfiguration.mac
  run.pl
  field.dat
  )

foreach(_script ${K37_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------


