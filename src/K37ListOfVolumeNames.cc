// Authors: Spencer Behling and Benjamin Fenker 2013

#include <stdio.h>
#include <sys/stat.h>

#include <vector>
#include <string>
#include <iomanip>

#include "K37ListOfVolumeNames.hh"

K37ListOfVolumeNames::K37ListOfVolumeNames(std::string _filename) {
  filename = _filename;
  sizeOfVector = 1000;
  listOfVolumeNames.clear();
  ThetaHit.clear();
  startTheta.clear();
  this->deleteFile();
  shouldVolumeNamesBeRecorded = false;
}

K37ListOfVolumeNames::K37ListOfVolumeNames(std::string _filename,
                                           int _sizeOfVector) {
  filename = _filename;
  sizeOfVector = _sizeOfVector;
  listOfVolumeNames.clear();
  ThetaHit.clear();
  startTheta.clear();
  this->deleteFile();
  shouldVolumeNamesBeRecorded = false;
}

K37ListOfVolumeNames::~K37ListOfVolumeNames() {
}

void K37ListOfVolumeNames::setEnteredVolumeName(std::string volumeName,
                                                G4double hitTheta,
                                                G4double vertexTheta) {
  listOfVolumeNames.push_back(volumeName);
  ThetaHit.push_back(hitTheta);
  startTheta.push_back(vertexTheta);
}

void K37ListOfVolumeNames::clearList() {
  listOfVolumeNames.clear();
  ThetaHit.clear();
  startTheta.clear();
}
void K37ListOfVolumeNames::touchFile() {
  fileOfVolumeNames.open(filename.c_str(),
                         std::ofstream::out | std::ofstream::trunc);
  fileOfVolumeNames.close();
}

void K37ListOfVolumeNames::deleteFile() {
  std::string  strFilename = filename;
  struct stat stFileInfo;
  int intStat;

  intStat = stat(strFilename.c_str(), &stFileInfo);
  if (intStat == 0) {
    remove(strFilename.c_str());
    // G4cout<<"File "<<strFilename<<" removed."<<G4endl;
  }
}

void K37ListOfVolumeNames::deleteEnteriesFromList(int numberToDelete) {
  for (int i = 0; i < numberToDelete; i++) {
    listOfVolumeNames.pop_back();
    ThetaHit.pop_back();
    startTheta.pop_back();
  }
}

void K37ListOfVolumeNames::printNames() {
  fileOfVolumeNames.open(filename.c_str(),
                         std::ofstream::out | std::ofstream::app);

  for (int i = 0; i < static_cast<int>(listOfVolumeNames.size()); i++) {
    fileOfVolumeNames << std::setw(25) << listOfVolumeNames[i] << std::setw(15)
                      << ThetaHit[i] << std::setw(15) << startTheta[i]
                      << G4endl;
    // G4cout<<listOfVolumeNames[i]<<G4endl;
  }

  fileOfVolumeNames.close();
}

void K37ListOfVolumeNames::checkIfPrintIsNeeded(G4bool isThisTheLastEvent) {
  if (isThisTheLastEvent == true) {
    this->printNames();
    listOfVolumeNames.clear();
    ThetaHit.clear();
    startTheta.clear();
  } else {
    if (listOfVolumeNames.size() >= sizeOfVector) {
      this->printNames();
      listOfVolumeNames.clear();
      ThetaHit.clear();
      startTheta.clear();
    }
  }
}

unsigned int K37ListOfVolumeNames::checkSizeOfList() {
  return listOfVolumeNames.size();
}
