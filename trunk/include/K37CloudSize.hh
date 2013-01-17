// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37CloudSize_h
#define K37CloudSize_h 1

#include "globals.hh"

class K37CloudSize {
  ///
  /// \class K37CloudSize  K37CloudSize.hh K37CloudSize
  /// \brief This is a class to change the cload size based on temperature
  /// \author Spencer Behling
  /// \date Jan. 5 2012
  /// \since version 1.0
  ///
 public:
  K37CloudSize(G4double, G4double);
  ~K37CloudSize();
  void makeEvent();
  G4double xFinal() {
    return endX;
  };
  G4double yFinal() {
    return endY;
  };
  G4double zFinal() {
    return endZ;
  };

 private:
  G4double cloudTemperature;  // K
  G4double cloudSizeStart;    // sigma in mm
  G4double sigma;
  G4double boltzmannConstant;  // MeV_per_K
  G4double massOfAr37;  // MeV_per_c^2
  G4double speedOfLight;  // mm_per_us
  G4double startX;  // mm
  G4double startY;  // mm
  G4double startZ;  // mm
  G4double endX;  // mm
  G4double endY;  // mm
  G4double endZ;  // mm
  G4double velocityX;  // mm_per_s
  G4double velocityY;  // mm_per_s
  G4double velocityZ;  // mm_per_s
  G4double decayTime;  // us
  G4double cycleTime;  // us
  G4double distanceTraveled;  // mm
};

#endif
