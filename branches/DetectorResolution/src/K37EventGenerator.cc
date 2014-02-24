// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37EventGenerator.hh"
#include "K37FermiFunction.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"
#include "G4UnitsTable.hh"

K37EventGenerator::K37EventGenerator()
  :FF(0) {
  // G4Electron *ele = G4Electron::ElectronDefinition();
  // G4double Mass_e = ele -> GetPDGMass();

  FF = new K37FermiFunction();
  electron.Mass = 0.510998;           // MeV/c^2
  parent.Mass = 36.97337589*931.46;     // 37K (MeV/c^2)
  daughter.Mass = 36.96677632*931.46;   // 37Ar (MeV/c^2)
  neutrino.Mass = 0.0;

  Zhi = 2.69001;
  BigA = -0.573938;
  // BigA = -0.5;
  //  BigA = 0.0;
  // BigA = -1.0;
  BigB = -0.779145;
  // BigB = 0.0;
  LittleC = 0.205207;
  // LittleC = 0.20;
  // LittleC = 0.0;
  LittleA = 0.657989;
  // LittleA = 0.0;

  // All in MeV/c^2
  QValue = -1.0 * (daughter.Mass - parent.Mass  + 2. * electron.Mass);
  electron.MaxE = (pow(parent.Mass , 2.0) +
                   pow(electron.Mass, 2.0) -
                   pow((neutrino.Mass + daughter.Mass + electron.Mass), 2.0))
    / (2.0 * parent.Mass);
  neutrino.MaxE = (pow(parent.Mass, 2.0) +
                   pow(neutrino.Mass, 2.0) -
                   pow((electron.Mass + daughter.Mass + electron.Mass), 2.0))
    /(2.0 * parent.Mass);
  daughter.MaxE = (pow(parent.Mass, 2.0) +
                   pow((daughter.Mass + electron.Mass), 2.0) -
                   pow((electron.Mass+neutrino.Mass), 2.0))
    /(2.0 * parent.Mass);
  electron.MaxT = electron.MaxE - electron.Mass;
  neutrino.MaxT = neutrino.MaxE - neutrino.Mass;
  daughter.MaxT = daughter.MaxE - daughter.Mass - electron.Mass;

  v_over_c_running = 0.0;
  running_num = 0.0;
  running_denom = 0.0;
  min_cos_theta_ = 0.0;                 // Default is no bias
}

K37EventGenerator::~K37EventGenerator() {
  // G4cout << "BF LOOK HERE: total v/c = " << v_over_c_running << G4endl;
  // G4cout << "num = " << running_num << " denom: " << running_denom << G4endl;
  delete FF;
}

void K37EventGenerator::SetBigA(G4double BigA_)
{
  if ((BigA_ > 1) && (BigA_ < -1))
  {
     G4cout << "Warning! A_{beta} should be in range [-1,1]. " 
        << BigA_<<" is unacceptable!"<< G4endl;
    G4cout << "No action taken." << G4endl;
    return;
  }
  BigA = BigA_;
}

void K37EventGenerator::SetMinCosTheta(G4double mct) {
  if (mct < 0) {
    G4cout << "Warning! Minimum cos(theta) should be positive!" << G4endl;
    G4cout << "No action taken." << G4endl;
    return;
  }
  min_cos_theta_ = mct;
}

void K37EventGenerator::SetConeHalfAngle(G4double angle) {
  SetMinCosTheta(cos(angle));
}
