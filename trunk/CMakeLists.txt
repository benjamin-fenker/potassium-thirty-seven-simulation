#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(K37)

#It is best to set the build type from the build.sh
SET( CMAKE_CXX_COMPILER "g++" )
#SET( CMAKE_CXX_COMPILER "clang++" )
#SET( CMAKE_CXX_COMPILER "c++" )

FUNCTION(WarningAsError)
   IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
   ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
   ENDIF()
ENDFUNCTION()

OPTION(K37_WARNINGS_AS_ERROR "Force compilation ot treat warnings as errors." OFF)

IF(K37_WARNINGS_AS_ERROR)
   WarningAsError()
ENDIF(K37_WARNINGS_AS_ERROR)

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

SET (CONFIGFILES_ "\"${CMAKE_BINARY_DIR}/configurationFiles/\"")

execute_process(COMMAND svnversion
                COMMAND cut -c1-3
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                OUTPUT_VARIABLE K37_VERSION)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/K37Config.hh.in"
	"${PROJECT_SOURCE_DIR}/include/K37Config.hh"
	)

configure_file (
	"${PROJECT_SOURCE_DIR}/IOconfiguration.mac.in"
	"${PROJECT_SOURCE_DIR}/IOconfiguration.mac"
	)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/external/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/external/*.hh)

#----------------------------------------------------------------------------
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/src/auxiliary aux)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

# AFTER the G4/ROOT setups, add the necessary c++ flags
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
#SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
#SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O3 -DNDEBUG" )

add_executable(K37 K37.cc ${sources} ${headers})
target_link_libraries(K37 ${Geant4_LIBRARIES} Core RIO Tree)

#----------------------------------------------------------------------------
 # Copy all scripts to the build directory, i.e. the directory in which we
# build K37. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(K37_SCRIPTS
  vis.mac
  icons.mac
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

#Copy The Isotope Definitions ==========
set(CONFIGFILES
  K_37_INPUT.txt
  upper_strip_detector_x.res
  upper_strip_detector_y.res
  lower_strip_detector_x.res
  lower_strip_detector_y.res
  )

foreach(_file ${CONFIGFILES})
  configure_file(
    ${CMAKE_SOURCE_DIR}/configurationFiles/${_file}
    ${CMAKE_BINARY_DIR}/configurationFiles/${_file}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------


