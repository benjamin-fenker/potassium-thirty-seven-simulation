#!/bin/bash
K37SOURCEDIRECTORY=/where/you/checkedOut/K37?; #where the source code is 
K37BUILDDIRECTORY=/where/you/plan/to/build/K37?; #where will you put the application
PLACEOFGeant4ConfigCmake=/where/did/you/build/geant?/lib/Geant4-9.5.0 #the directory that contains Geant4Config.cmake
cd $K37SOURCEDIRECTORY;
K37_VERSION=`svn info |grep Revision: | cut -c11-`;
export K37_VERSION;
cd $K37BUILDDIRECTORY ;
outputDir="runFiles"
if [ ! -d "$outputDir" ]; then
     # Control will enter here if $DIRECTORY doesn't exist.
     mkdir "$outputDir";
fi
O_CPP_FLAGS="-O3 -Wall" #optimized build compiler options
D_CPP_FLAGS="-g -O0 -Wall" #debug build compiler options 
#ReleaseType="DEBUG" #you can choose to use the debug build options
ReleaseType="RELEASE" #you can choose to use the release build options 
cmake CMAKE_BINARY_DIR=$K37BUILDDIRECTORY -DGeant4_DIR=$PLACEOFGeant4ConfigCmake $K37SOURCEDIRECTORY -DCMAKE_CXX_FLAGS_DEBUG:STRING="$D_CPP_FLAGS" -DCMAKE_CXX_FLAGS_RELEASE:STRING="$O_CPP_FLAGS" -DCMAKE_BUILD_TYPE:STRING="$ReleaseType";
make;
./K37;
