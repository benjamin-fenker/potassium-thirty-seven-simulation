// Authors: Spencer Behling and Benjamin Fenker 2013

#include <iomanip>
#include "globals.hh"
#include "K37ContainerForStripInformation.hh"
#include "G4ThreeVector.hh"

K37ContainerForStripInformation::K37ContainerForStripInformation() {
  InitializeDetectorMap();
  DetectorPlusZ_X_secondary.clear();
  DetectorPlusZ_Y_secondary.clear();
  DetectorPlusZ_X_beta.clear();
  DetectorPlusZ_Y_beta.clear();
  DetectorMinusZ_X_secondary.clear();
  DetectorMinusZ_Y_secondary.clear();
  DetectorMinusZ_X_beta.clear();
  DetectorMinusZ_Y_beta.clear();
  listPlusZ_Y_secondary.clear();
  listPlusZ_Y_beta.clear();
  listMinusZ_Y_secondary.clear();
  listMinusZ_Y_beta.clear();
  listPlusZ_X_secondary.clear();
  listPlusZ_X_beta.clear();
  listMinusZ_X_secondary.clear();
  listMinusZ_X_beta.clear();
}

//-----------------------------------------------

K37ContainerForStripInformation::~K37ContainerForStripInformation()
{}


void K37ContainerForStripInformation::BeginingOfEventClear() {
  DetectorPlusZ_X_secondary.clear();
  DetectorPlusZ_Y_secondary.clear();
  DetectorPlusZ_X_beta.clear();
  DetectorPlusZ_Y_beta.clear();
  DetectorMinusZ_X_secondary.clear();
  DetectorMinusZ_Y_secondary.clear();
  DetectorMinusZ_X_beta.clear();
  DetectorMinusZ_Y_beta.clear();
  listPlusZ_X_beta.clear();
  listPlusZ_X_secondary.clear();
  listPlusZ_Y_beta.clear();
  listPlusZ_Y_secondary.clear();
  listMinusZ_X_secondary.clear();
  listMinusZ_X_beta.clear();
  listMinusZ_Y_secondary.clear();
  listMinusZ_Y_beta.clear();
}
//-----------------------------------------------

void K37ContainerForStripInformation::StoreStripInformation(
                                          G4ThreeVector hitPosition,
                                          G4double energyDeposited,
                                          G4bool trueIfPrimary,
                                          G4bool trueIfPlusZDetector) {
  if (energyDeposited <= 0) {
    return;
  }
  G4int xTemp = this->FindStripNumberForHit(hitPosition.x());
  G4int yTemp = this->FindStripNumberForHit(hitPosition.y());

  if (trueIfPrimary) {
    if (trueIfPlusZDetector) {
      DetectorPlusZ_X_beta.
        insert(std::pair<G4int, G4double>(xTemp, energyDeposited));
      DetectorPlusZ_Y_beta.
        insert(std::pair<G4int, G4double>(yTemp, energyDeposited));
      listPlusZ_X_beta.push_back(xTemp);
      listPlusZ_Y_beta.push_back(yTemp);
    } else {
      DetectorMinusZ_X_beta.
        insert(std::pair<G4int, G4double>(xTemp, energyDeposited));
      DetectorMinusZ_Y_beta.
        insert(std::pair<G4int, G4double>(yTemp, energyDeposited));
      listMinusZ_X_beta.push_back(xTemp);
      listMinusZ_Y_beta.push_back(yTemp);
    }
  } else {
    if (trueIfPlusZDetector) {
      DetectorPlusZ_X_secondary.
        insert(std::pair<G4int, G4double>(xTemp, energyDeposited));
      DetectorPlusZ_Y_secondary.
        insert(std::pair<G4int, G4double>(yTemp, energyDeposited));
      listPlusZ_X_secondary.push_back(xTemp);
      listPlusZ_Y_secondary.push_back(yTemp);
    } else {
      DetectorMinusZ_X_secondary.
        insert(std::pair<G4int, G4double>(xTemp, energyDeposited));
      DetectorMinusZ_Y_secondary.
        insert(std::pair<G4int, G4double>(yTemp, energyDeposited));
      listMinusZ_X_secondary.push_back(xTemp);
      listMinusZ_Y_secondary.push_back(yTemp);
    }
  }
}

void K37ContainerForStripInformation::PrintMaps() {
  listPlusZ_X_beta.sort();
  listPlusZ_X_beta.unique();

  listPlusZ_X_secondary.sort();
  listPlusZ_X_secondary.unique();

  listPlusZ_Y_beta.sort();
  listPlusZ_Y_beta.unique();

  listPlusZ_Y_secondary.sort();
  listPlusZ_Y_secondary.unique();

  listMinusZ_X_secondary.sort();
  listMinusZ_X_secondary.unique();

  listMinusZ_X_beta.sort();
  listMinusZ_X_beta.unique();

  listMinusZ_Y_secondary.sort();
  listMinusZ_Y_secondary.unique();

  listMinusZ_Y_beta.sort();
  listMinusZ_Y_beta.unique();


  // for (G4int ch=0; ch<=41; ch++)
  // {
  G4double energyTemp;
  for (listIterator = listMinusZ_X_beta.begin();
       listIterator != listMinusZ_X_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_X_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listMinusZ_Y_beta.begin();
       listIterator != listMinusZ_Y_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_Y_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first;
         iteratorOfValues != ret.second; ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_X_beta.begin();
       listIterator != listPlusZ_X_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_X_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_Y_beta.begin();
       listIterator != listPlusZ_Y_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_Y_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listMinusZ_X_secondary.begin();
       listIterator != listMinusZ_X_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_X_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listMinusZ_Y_secondary.begin();
       listIterator != listMinusZ_Y_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_Y_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_X_secondary.begin();
       listIterator != listPlusZ_X_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_X_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_Y_secondary.begin();
       listIterator != listPlusZ_Y_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_Y_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }
}

G4int K37ContainerForStripInformation::FindStripNumberForHit(
                                           G4double PositionOfHit) {
  for (iteratorOfDetectorMap = mapOfDetectorStrips.begin();
       iteratorOfDetectorMap != mapOfDetectorStrips.end();
       iteratorOfDetectorMap++) {
    if ((*iteratorOfDetectorMap).second < PositionOfHit) {
      break;
    }
  }

  if (iteratorOfDetectorMap == mapOfDetectorStrips.begin()) {
    return 0;
  } else {
    G4int temp;
    temp = (*iteratorOfDetectorMap).first;
    temp--;
    return temp;
  }
}

void K37ContainerForStripInformation::InitializeDetectorMap() {
  mapOfDetectorStrips[0 ] =  30;
  mapOfDetectorStrips[1 ] =  20;
  mapOfDetectorStrips[2 ] =  19;
  mapOfDetectorStrips[3 ] =  18;
  mapOfDetectorStrips[4 ] =  17;
  mapOfDetectorStrips[5 ] =  16;
  mapOfDetectorStrips[6 ] =  15;
  mapOfDetectorStrips[7 ] =  14;
  mapOfDetectorStrips[8 ] =  13;
  mapOfDetectorStrips[9 ] =  12;
  mapOfDetectorStrips[10] =  11;
  mapOfDetectorStrips[11] =  10;
  mapOfDetectorStrips[12] =  9;
  mapOfDetectorStrips[13] =  8;
  mapOfDetectorStrips[14] =  7;
  mapOfDetectorStrips[15] =  6;
  mapOfDetectorStrips[16] =  5;
  mapOfDetectorStrips[17] =  4;
  mapOfDetectorStrips[18] =  3;
  mapOfDetectorStrips[19] =  2;
  mapOfDetectorStrips[20] =  1;
  mapOfDetectorStrips[21] =  0;
  mapOfDetectorStrips[22] = -1;
  mapOfDetectorStrips[23] = -2;
  mapOfDetectorStrips[24] = -3;
  mapOfDetectorStrips[25] = -4;
  mapOfDetectorStrips[26] = -5;
  mapOfDetectorStrips[27] = -6;
  mapOfDetectorStrips[28] = -7;
  mapOfDetectorStrips[29] = -8;
  mapOfDetectorStrips[30] = -9;
  mapOfDetectorStrips[31] = -10;
  mapOfDetectorStrips[32] = -11;
  mapOfDetectorStrips[33] = -12;
  mapOfDetectorStrips[34] = -13;
  mapOfDetectorStrips[35] = -14;
  mapOfDetectorStrips[36] = -15;
  mapOfDetectorStrips[37] = -16;
  mapOfDetectorStrips[38] = -17;
  mapOfDetectorStrips[39] = -18;
  mapOfDetectorStrips[40] = -19;
  mapOfDetectorStrips[41] = -20;
}

void K37ContainerForStripInformation::PassTheMinusZDetectors() {
  listMinusZ_X_secondary.sort();
  listMinusZ_X_secondary.unique();

  listMinusZ_X_beta.sort();
  listMinusZ_X_beta.unique();

  listMinusZ_Y_secondary.sort();
  listMinusZ_Y_secondary.unique();

  listMinusZ_Y_beta.sort();
  listMinusZ_Y_beta.unique();

  G4double energyTemp;
  for (listIterator = listMinusZ_X_beta.begin();
       listIterator != listMinusZ_X_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_X_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listMinusZ_Y_beta.begin();
       listIterator != listMinusZ_Y_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_Y_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listMinusZ_X_secondary.begin();
       listIterator != listMinusZ_X_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_X_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listMinusZ_Y_secondary.begin();
       listIterator != listMinusZ_Y_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorMinusZ_Y_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }
}

void K37ContainerForStripInformation::PassThePlusZDetectors() {
  listPlusZ_X_secondary.sort();
  listPlusZ_X_secondary.unique();

  listPlusZ_X_beta.sort();
  listPlusZ_X_beta.unique();

  listPlusZ_Y_secondary.sort();
  listPlusZ_Y_secondary.unique();

  listPlusZ_Y_beta.sort();
  listPlusZ_Y_beta.unique();

  G4double energyTemp;
  for (listIterator = listPlusZ_X_beta.begin();
       listIterator != listPlusZ_X_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_X_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_Y_beta.begin();
       listIterator != listPlusZ_Y_beta.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_Y_beta.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_X_secondary.begin();
       listIterator != listPlusZ_X_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_X_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }

  for (listIterator = listPlusZ_Y_secondary.begin();
       listIterator != listPlusZ_Y_secondary.end(); listIterator++) {
    energyTemp = 0;
    ret = DetectorPlusZ_Y_secondary.equal_range((*listIterator));
    for (iteratorOfValues = ret.first; iteratorOfValues != ret.second;
         ++iteratorOfValues) {
      // G4cout << " " << (*iteratorOfValues).second;
      energyTemp += (*iteratorOfValues).second;
    }
    // G4cout << (*listIterator)<< " =>"<<energyTemp/keV<<G4endl;
  }
}
