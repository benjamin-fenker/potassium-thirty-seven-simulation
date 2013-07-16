// Authors: Spencer Behling and Benjamin Fenker 2013

#include <stdio.h>
#include <sys/stat.h>

#include <vector>
#include <string>
#include <iomanip>
#include <map>

#include "G4ThreeVector.hh"
#include "K37AllPossibleEventInformation.hh"

K37AllPossibleEventInformation::K37AllPossibleEventInformation(
                                    std::string _filename) {
  filename = _filename;
  doubleHitInfinitePrecision = false;
  acceptedEvent = false;
  doubleHitDifferentStrip = false;
  PlusZDetectorsFired = false;
  IsTheVertexSet = false;
  gammaFiredStripDetectorPlusZ = false;
  gammaFiredStripDetectorMinusZ = false;
  gammaFiredScintillatorPlusZ = false;
  gammaFiredScintillatorMinusZ = false;
  stripEnergyFromBeta_X_MinusZ.clear();
  stripEnergyFromGamma_X_MinusZ.clear();
  stripEnergyFromBeta_Y_MinusZ.clear();
  stripEnergyFromGamma_Y_MinusZ.clear();
  stripEnergyFromBeta_X_PlusZ.clear();
  stripEnergyFromGamma_X_PlusZ.clear();
  stripEnergyFromBeta_Y_PlusZ.clear();
  stripEnergyFromGamma_Y_PlusZ.clear();
  listOfVolumeNames.clear();
  enteringStripDetector.clear();
  enteringScintillator.clear();
  enteringShakeOffElectronDetector.clear();
  enteringShakeOffElectronDetectorTime.clear();
  originalTheta = 0;
  measuredTheta = 0;
  energyFromBetaInScintillator = 0;
  stripDetectorStartTime = 0;
  energyFromGammaInScintillator = 0;
  totalEnergyInScintillator = 0;
  totalEnergyInStripDetector = 0;
  annihilationPositionX = 0;
  annihilationPositionY = 0;
  annihilationPositionZ = 0;
  vertexPosition.set(0, 0, 0);
  daughterMomentum.set(0, 0, 0);
  this->deleteFile();
}


K37AllPossibleEventInformation::~K37AllPossibleEventInformation() {
}

void K37AllPossibleEventInformation::clearEventInformation() {
  doubleHitInfinitePrecision = false;
  acceptedEvent = false;
  doubleHitDifferentStrip = false;
  PlusZDetectorsFired = false;
  IsTheVertexSet = false;
  gammaFiredStripDetectorPlusZ = false;
  gammaFiredStripDetectorMinusZ = false;
  gammaFiredScintillatorPlusZ = false;
  gammaFiredScintillatorMinusZ = false;
  stripEnergyFromBeta_X_MinusZ.clear();
  stripEnergyFromGamma_X_MinusZ.clear();
  stripEnergyFromBeta_Y_MinusZ.clear();
  stripEnergyFromGamma_Y_MinusZ.clear();
  stripEnergyFromBeta_X_PlusZ.clear();
  stripEnergyFromGamma_X_PlusZ.clear();
  stripEnergyFromBeta_Y_PlusZ.clear();
  stripEnergyFromGamma_Y_PlusZ.clear();
  listOfVolumeNames.clear();
  enteringStripDetector.clear();
  enteringScintillator.clear();
  enteringShakeOffElectronDetector.clear();
  enteringShakeOffElectronDetectorTime.clear();
  originalTheta = 0;
  measuredTheta = 0;
  energyFromBetaInScintillator = 0;
  stripDetectorStartTime = 0;
  energyFromGammaInScintillator = 0;
  totalEnergyInScintillator = 0;
  totalEnergyInStripDetector = 0;
  annihilationPositionX = 0;
  annihilationPositionY = 0;
  annihilationPositionZ = 0;
  vertexPosition.set(0, 0, 0);
  daughterMomentum.set(0, 0, 0);
}

void K37AllPossibleEventInformation::deleteFile() {
  std::string  strFilename = filename;
  struct stat stFileInfo;
  int intStat;

  intStat = stat(strFilename.c_str(), &stFileInfo);
  if (intStat == 0) {
    remove(strFilename.c_str());
    // G4cout<<"File "<<strFilename<<" removed."<<G4endl;
  }
  // else
  // {
  // G4cout<<"File "<<strFilename<<" did not exist."<<G4endl;
  // }
}

void K37AllPossibleEventInformation::EndOfEventActions() {
  std::multimap<G4int, G4ThreeVector>::iterator it;
  if (gammaFiredStripDetectorPlusZ || gammaFiredStripDetectorMinusZ) {
    it = enteringStripDetector.equal_range(1).first;
    measuredTheta= ((*it).second).theta();
    // measuredTheta = (*(enteringStripDetector.equal_range(1).first).second).
    //   theta();
  } else {
    it = enteringStripDetector.equal_range(0).first;
    measuredTheta= ((*it).second).theta();
    // measuredTheta = (*(enteringStripDetector.equal_range(1).first).second).
    //   theta();
  }

  // if(enteringStripDetector.count(0)== 0)
  // {
  // measuredTheta = enteringStripDetector
  // }
  // switch (enteringStripDetector.size())
  // {
  // case 0:
  // measuredTheta = 0;
  // break;
  // case 1:
  // measuredTheta = enteringStripDetector[0].theta();
  // break;
  // default:
  // doubleHitInfinitePrecision = true;
  // }
}


void K37AllPossibleEventInformation::setEnteringStripDetector(G4int species,
                                         G4ThreeVector enteringPosition) {
  enteringStripDetector.insert(std::make_pair(species, enteringPosition));
  // enteringStripDetector.push_back(enteringPosition);
}
void K37AllPossibleEventInformation::setEnteringScintillator(G4int species,
                                         G4ThreeVector enteringPosition) {
  enteringScintillator.insert(std::make_pair(species, enteringPosition));
  // enteringScintillator.push_back(enteringPosition);
}

void K37AllPossibleEventInformation::setEnteringShakeOffElectronDetector(
                                         G4ThreeVector enteringPosition) {
  enteringShakeOffElectronDetector.push_back(enteringPosition);
}

void K37AllPossibleEventInformation::setEnteringShakeOffElectronDetectorTime(
                                         G4double enteringTime) {
  enteringShakeOffElectronDetectorTime.push_back(enteringTime);
}

void K37AllPossibleEventInformation::setMinusZStrip_X_beta(G4int strip,
                                                           G4double energy) {
  stripEnergyFromBeta_X_MinusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setMinusZStrip_Y_beta(G4int strip,
                                                           G4double energy) {
  stripEnergyFromBeta_Y_MinusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setMinusZStrip_X_gamma(G4int strip,
                                                            G4double energy) {
  stripEnergyFromGamma_X_MinusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setMinusZStrip_Y_gamma(G4int strip,
                                                            G4double energy) {
  stripEnergyFromGamma_Y_MinusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setPlusZStrip_X_beta(G4int strip,
                                                          G4double energy) {
  stripEnergyFromBeta_X_PlusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setPlusZStrip_Y_beta(G4int strip,
                                                          G4double energy) {
  stripEnergyFromBeta_Y_PlusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setPlusZStrip_X_gamma(G4int strip,
                                                           G4double energy) {
  stripEnergyFromGamma_X_PlusZ.insert(std::make_pair(strip, energy));
}
void K37AllPossibleEventInformation::setPlusZStrip_Y_gamma(G4int strip,
                                                           G4double energy) {
  stripEnergyFromGamma_Y_PlusZ.insert(std::make_pair(strip, energy));
}
