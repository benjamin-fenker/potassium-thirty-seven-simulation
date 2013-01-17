// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37AnnihilationPosition_H
#define K37AnnihilationPosition_H 1

#include <vector>
#include <string>
#include <fstream>

#include "globals.hh"
#include "K37AnnihilationPosition.hh"

class K37AnnihilationPosition {
 public:
  explicit K37AnnihilationPosition(std::string _filename);
  K37AnnihilationPosition(std::string _filename, int _sizeOfVector);
  ~K37AnnihilationPosition();

  void printPosition();
  void setPosition(G4double, G4double, G4double);
  void checkIfPrintIsNeeded(G4bool);
  void deleteEnteryFromList();
  void clearList();
  void touchFile();
  void deleteFile();
  G4bool getShouldAnnihilationBeRecorded() {
    return usefulness;
  };
  void setShouldAnnihilationBeRecorded() {
    usefulness = true;
  };
  unsigned int checkSizeOfList();

 private:
  std::vector< G4double > xPos;
  std::vector< G4double > yPos;
  std::vector< G4double > zPos;
  std::ofstream fileOfK37AnnihilationPositions;
  std::string filename;
  unsigned int sizeOfVector;
  G4bool usefulness;
};

#endif

