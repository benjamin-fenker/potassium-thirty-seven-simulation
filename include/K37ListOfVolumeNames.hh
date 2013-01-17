// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37ListOfVolumeNames_H
#define K37ListOfVolumeNames_H 1

#include <vector>
#include <string>
#include <fstream>

#include "globals.hh"
#include "K37ListOfVolumeNames.hh"

class K37ListOfVolumeNames {
 public:
  explicit K37ListOfVolumeNames(std::string _filename);
  K37ListOfVolumeNames(std::string _filename, int _sizeOfVector);
  ~K37ListOfVolumeNames();

  void printNames();
  void setEnteredVolumeName(std::string, G4double, G4double);
  void checkIfPrintIsNeeded(G4bool b);
  void deleteEnteriesFromList(int i);
  void clearList();
  void touchFile();
  void deleteFile();
  G4bool getShouldVolumeNamesBeRecorded() {
    return shouldVolumeNamesBeRecorded;
  };
  void setShouldVolumeNamesBeRecorded() {
    shouldVolumeNamesBeRecorded= true;
  };
  unsigned int checkSizeOfList();

 private:
  std::vector< std::string > listOfVolumeNames;
  std::vector< G4double > ThetaHit;
  std::ofstream fileOfVolumeNames;
  std::vector< G4double > startTheta;
  unsigned int sizeOfVector;
  G4bool shouldVolumeNamesBeRecorded;
  std::string filename;
};

#endif

