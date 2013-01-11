// Authors: Spencer Behling and Benjamin Fenker 2013

#include <stdio.h>
#include <sys/stat.h>

#include <vector>
#include <string>
#include <iomanip>

#include "K37AnnihilationPosition.hh"

K37AnnihilationPosition::K37AnnihilationPosition(std::string _filename) {
  filename = _filename;
  sizeOfVector = 1000;
  xPos.clear();
  yPos.clear();
  zPos.clear();
  this->deleteFile();
  usefulness = false;
}

K37AnnihilationPosition::K37AnnihilationPosition(std::string _filename,
                                                 int _sizeOfVector) {
  filename = _filename;
  sizeOfVector = _sizeOfVector;
  xPos.clear();
  yPos.clear();
  zPos.clear();
  this->deleteFile();
  usefulness = false;
}

K37AnnihilationPosition::~K37AnnihilationPosition() {
}

void K37AnnihilationPosition::touchFile() {
  fileOfK37AnnihilationPositions.open(filename.c_str(),
                                      std::ofstream::out |
                                      std::ofstream::trunc);
  fileOfK37AnnihilationPositions.close();
}

void K37AnnihilationPosition::deleteFile() {
  std::string  strFilename = filename;
  struct stat stFileInfo;
  int intStat;

  intStat = stat(strFilename.c_str(), &stFileInfo);
  if (intStat == 0) {
    remove(strFilename.c_str());
  }
}

void K37AnnihilationPosition::setPosition(G4double x, G4double y, G4double z) {
  xPos.push_back(x);
  yPos.push_back(y);
  zPos.push_back(z);
  this->checkIfPrintIsNeeded(false);
}

void K37AnnihilationPosition::clearList() {
  xPos.clear();
  yPos.clear();
  zPos.clear();
}

void K37AnnihilationPosition::deleteEnteryFromList() {
  xPos.pop_back();
  yPos.pop_back();
  zPos.pop_back();
}

void K37AnnihilationPosition::printPosition() {
  fileOfK37AnnihilationPositions.open(filename.c_str(),
                                      std::ofstream::out | std::ofstream::app);

  for (int i = 0; i < static_cast<int>(xPos.size()); i++) {
    fileOfK37AnnihilationPositions << std::setw(15) << std::left
                                   << xPos[static_cast<int>(i)]
                                   << std::setw(15)
                                   << yPos[static_cast<int>(i)]
                                   << std::setw(15)
                                   << zPos[static_cast<int>(i)] << G4endl;
  }

  fileOfK37AnnihilationPositions.close();
}

void K37AnnihilationPosition::checkIfPrintIsNeeded(G4bool isThisTheLastEvent) {
  if (isThisTheLastEvent == true) {
    this->printPosition();
    xPos.clear();
    yPos.clear();
    zPos.clear();
  } else {
    if (xPos.size() >= sizeOfVector) {
      this->printPosition();
      xPos.clear();
      yPos.clear();
      zPos.clear();
    }
  }
}

unsigned int K37AnnihilationPosition::checkSizeOfList() {
  return xPos.size();
}
