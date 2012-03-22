#!/bin/bash
K37SOURCEDIRECTORY=/where/you/checkedOut/K37?; #where the source code is 
K37BUILDDIRECTORY=/where/you/plan/to/build/K37?; #where will you put the application
PLACEOFGeant4ConfigCmake=/where/did/you/build/geant?/lib/Geant4-9.5.0 #the directory that contains Geant4Config.cmake
cd $K37SOURCEDIRECTORY;
K37_VERSION=`svn info |grep Revision: | cut -c11-`;
export K37_VERSION;
cd $K37BUILDDIRECTORY ;
cmake CMAKE_BINARY_DIR=$K37BUILDDIRECTORY -DGeant4_DIR=$PLACEOFGeant4ConfigCmake $K37SOURCEDIRECTORY;
make;
./K37;
