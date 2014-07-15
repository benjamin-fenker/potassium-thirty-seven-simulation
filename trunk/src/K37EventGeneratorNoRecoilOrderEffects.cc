// Authors: Spencer Behling
// The point of this class is to hack in a quick 2% branch. It is 
// following the venerable software practice of cut and paste programming.

#include "K37EventGeneratorNoRecoilOrderEffects.hh"
#include "K37FermiFunction.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"
#include "G4UnitsTable.hh"
#include "K37Config.hh"

K37EventGeneratorNoRecoilOrderEffects::K37EventGeneratorNoRecoilOrderEffects()
  :FF(0)
{
  // G4Electron *ele = G4Electron::ElectronDefinition();
  // G4double Mass_e = ele -> GetPDGMass();

  FF = new K37FermiFunction();
  electron.Mass = 0.510998;           // MeV/c^2
  parent.Mass = 36.97337589*931.46;     // 37K (MeV/c^2)
  // 37Ar 5/2^+ excited state (MeV/c^2)
  daughter.Mass = (36.96677632*931.46)+2.796;
  neutrino.Mass = 0.0;
  
  //These are the values for the transition to the 2% branch. 
  //In this class these values have meaning because they are not 
  //overridden at event time by energy dependent values from the 
  //recoil order effects. The values in
  //K37EventGeneratorNoRecoilOrderEffects.cc are overiden at event time. 
  Zhi     =  2.0;
  BigA    = -3.0/5.0;
  BigB    =  3.0/5.0;
  LittleC = -1.0/5.0;
  LittleA = -1.0/3.0;

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

K37EventGeneratorNoRecoilOrderEffects::~K37EventGeneratorNoRecoilOrderEffects() {
  // G4cout << "BF LOOK HERE: total v/c = " << v_over_c_running << G4endl;
  // G4cout << "num = " << running_num << " denom: " << running_denom << G4endl;
  delete FF;
}

void K37EventGeneratorNoRecoilOrderEffects::SetBigA(G4double BigA_)
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

void K37EventGeneratorNoRecoilOrderEffects::SetMinCosTheta(G4double mct) {
  if (mct < 0) {
    G4cout << "Warning! Minimum cos(theta) should be positive!" << G4endl;
    G4cout << "No action taken." << G4endl;
    return;
  }
  min_cos_theta_ = mct;
}

void K37EventGeneratorNoRecoilOrderEffects::SetConeHalfAngle(G4double angle) {
  SetMinCosTheta(cos(angle));
}
