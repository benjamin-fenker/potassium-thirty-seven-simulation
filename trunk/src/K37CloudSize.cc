// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37CloudSize.hh"
#include "globals.hh"
#include "Randomize.hh"

K37CloudSize::K37CloudSize(G4double _cloudTemperature, G4double _cloudSizeStart)
  : cloudTemperature(_cloudTemperature), cloudSizeStart(_cloudSizeStart),
    boltzmannConstant(8.61733e-11), massOfAr37(34434.330043344002),
    speedOfLight(2.99792458e6), cycleTime(4.0) {
  sigma = sqrt(cloudTemperature*boltzmannConstant*speedOfLight*speedOfLight/
               massOfAr37);
}

K37CloudSize::~K37CloudSize() {
}

void K37CloudSize::makeEvent() {
  startX = G4RandGauss::shoot(0, cloudSizeStart);
  startY = G4RandGauss::shoot(0, cloudSizeStart);
  startZ = G4RandGauss::shoot(0, cloudSizeStart);

  decayTime = cycleTime*G4UniformRand();

  velocityX = G4RandGauss::shoot(0, sigma);
  velocityY = G4RandGauss::shoot(0, sigma);
  velocityZ = G4RandGauss::shoot(0, sigma);

  endX = startX + velocityX* decayTime;
  endY = startY + velocityY* decayTime;
  endZ = startZ + velocityZ* decayTime;
}
