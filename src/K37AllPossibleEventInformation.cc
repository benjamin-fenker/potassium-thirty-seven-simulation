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
void K37AllPossibleEventInformation::touchFile() {
  fileOfVolumeEventInformation.open(filename.c_str(),
                                     std::ofstream::out | std::ofstream::trunc);
  fileOfVolumeEventInformation << "---------------------------------" << G4endl;
  fileOfVolumeEventInformation.close();
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

void K37AllPossibleEventInformation:: printEventInformation() {
  fileOfVolumeEventInformation.open(filename.c_str(),
                                    std::ofstream::out | std::ofstream::app);
  fileOfVolumeEventInformation << "metaStableTimeOfDeath: "
                               << metaStableTimeOfDeath << G4endl;
  fileOfVolumeEventInformation << "stripDetectorStartTime: "
                               << stripDetectorStartTime/ns << G4endl;
  fileOfVolumeEventInformation << "originalTheta: " << originalTheta << G4endl;
  fileOfVolumeEventInformation << "vertex position: " << vertexPosition
                               << G4endl;
  fileOfVolumeEventInformation << "daughterMomentum: " << daughterMomentum
                               << G4endl;
  fileOfVolumeEventInformation << "PlusZDetectorsFired: " << std::boolalpha
                               << PlusZDetectorsFired << G4endl;
  fileOfVolumeEventInformation << "GammaFiredStripDetector+Z: "
                               << gammaFiredStripDetectorPlusZ << G4endl;
  fileOfVolumeEventInformation << "GammaFiredStripDetector-Z: "
                               << gammaFiredStripDetectorMinusZ << G4endl;
  fileOfVolumeEventInformation << "GammaFiredScintillator+Z: "
                               << gammaFiredScintillatorPlusZ << G4endl;
  fileOfVolumeEventInformation << "GammaFiredScintillator-Z: "
                               << gammaFiredScintillatorMinusZ << G4endl;
  fileOfVolumeEventInformation << "measuredTheta: " << measuredTheta << G4endl;
  fileOfVolumeEventInformation << "doubleHitInfinitePrecision: "
                               << doubleHitInfinitePrecision << G4endl;
  fileOfVolumeEventInformation << "totalEnergyInStripDetector: "
                               << totalEnergyInStripDetector/keV << G4endl;
  fileOfVolumeEventInformation << "totalEnergyInScintillator: "
                               << totalEnergyInScintillator/keV << G4endl;

  std::multimap<G4int, G4ThreeVector>::iterator it;
  fileOfVolumeEventInformation <<"Strip Detector"<< G4endl;
  for (G4int species = 0 ; species <= 1; species++) {
    fileOfVolumeEventInformation << "There are "
                                 << static_cast<int>(enteringStripDetector.
                                                     count(species));
    fileOfVolumeEventInformation << " elements with key " << species << ":";
    for (it = enteringStripDetector.equal_range(species).first;
         it != enteringStripDetector.equal_range(species).second; ++it) {
      fileOfVolumeEventInformation << " " << (*it).second;
    }
    fileOfVolumeEventInformation << G4endl;
  }

  std::map<G4int, G4double>::iterator stripIt;
  if (PlusZDetectorsFired == false) {
    fileOfVolumeEventInformation <<"Strip Detector Breakdown Betas"<< G4endl;
    for (stripIt = stripEnergyFromBeta_X_MinusZ.begin();
         stripIt != stripEnergyFromBeta_X_MinusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "XStrip-number-beta and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20)
                                   << ((*stripIt).second)/keV << G4endl;
    }
    for (stripIt = stripEnergyFromBeta_Y_MinusZ.begin();
         stripIt != stripEnergyFromBeta_Y_MinusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "YStrip-number-beta and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20)<< ((*stripIt).second)/keV
                                   << G4endl;
    }
    fileOfVolumeEventInformation << "Strip Detector Breakdown Secondaries"
                                 << G4endl;
    for (stripIt = stripEnergyFromGamma_X_MinusZ.begin();
         stripIt != stripEnergyFromGamma_X_MinusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "XStrip-number-second and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20) << ((*stripIt).second)/keV
                                   << G4endl;
    }
    for (stripIt = stripEnergyFromGamma_Y_MinusZ.begin();
         stripIt != stripEnergyFromGamma_Y_MinusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "YStrip-number-second and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20) << ((*stripIt).second)/keV
                                   << G4endl;
    }
  }
  if (PlusZDetectorsFired != false) {
    fileOfVolumeEventInformation << "Strip Detector Breakdown Betas" << G4endl;
    for (stripIt = stripEnergyFromBeta_X_PlusZ.begin();
         stripIt != stripEnergyFromBeta_X_PlusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "XStrip-number-beta and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20) << ((*stripIt).second)/keV
                                   << G4endl;
    }
    for (stripIt = stripEnergyFromBeta_Y_PlusZ.begin();
         stripIt != stripEnergyFromBeta_Y_PlusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "YStrip-number-beta and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20) << ((*stripIt).second)/keV
                                   << G4endl;
    }
    fileOfVolumeEventInformation << "Strip Detector Breakdown Secondaries"
                                 << G4endl;
    for (stripIt = stripEnergyFromGamma_X_PlusZ.begin();
         stripIt != stripEnergyFromGamma_X_PlusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "XStrip-number-second and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20)<< ((*stripIt).second)/keV
                                   << G4endl;
    }
    for (stripIt = stripEnergyFromGamma_Y_PlusZ.begin();
         stripIt != stripEnergyFromGamma_Y_PlusZ.end(); stripIt++) {
      fileOfVolumeEventInformation << "YStrip-number-second and energy[kev]: "
                                   << std::setw(5) << (*stripIt).first
                                   << std::setw(20) << ((*stripIt).second)/keV
                                   << G4endl;
    }
  }

  fileOfVolumeEventInformation <<"Scintillator"<< G4endl;
  for (G4int species = 0 ; species <= 1; species++) {
    fileOfVolumeEventInformation << "There are "
                                 << static_cast<int>(enteringScintillator.
                                                     count(species));
    fileOfVolumeEventInformation << " elements with key " << species << ":";
    for (it = enteringScintillator.equal_range(species).first;
         it != enteringScintillator.equal_range(species).second; ++it) {
      fileOfVolumeEventInformation << " " << (*it).second;
    }
    fileOfVolumeEventInformation << G4endl;
  }
  std::vector<G4ThreeVector>::const_iterator enteringPositionIterator;

  fileOfVolumeEventInformation << "# of hits on ShakeOff: "
                               << enteringShakeOffElectronDetector.size()
                               << G4endl;
  for (enteringPositionIterator = enteringShakeOffElectronDetector.begin();
       enteringPositionIterator != enteringShakeOffElectronDetector.end();
       enteringPositionIterator++) {
    fileOfVolumeEventInformation << "entering ShakeOff: "
                                 << (* enteringPositionIterator) << G4endl;
  }

  std::vector<G4double>::const_iterator enteringTimeIterator;

  for (enteringTimeIterator = enteringShakeOffElectronDetectorTime.begin();
       enteringTimeIterator != enteringShakeOffElectronDetectorTime.end();
       enteringTimeIterator++) {
    fileOfVolumeEventInformation << "Time Entering ShakeOff[ns]: "
                                 << (*enteringTimeIterator)/ns << G4endl;
  }

  fileOfVolumeEventInformation << "---------------------------------" << G4endl;
  fileOfVolumeEventInformation.close();
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
