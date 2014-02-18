// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37PrimaryGeneratorMessenger_h
#define K37PrimaryGeneratorMessenger_h 1

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "K37CloudSize.hh"
#include "K37EventGenerator.hh"
#include "K37PrimaryGeneratorAction.hh"

//----------------------------------

class K37PrimaryGeneratorMessenger: public G4UImessenger {
 public:
  explicit  K37PrimaryGeneratorMessenger(K37PrimaryGeneratorAction* pga);
  virtual ~K37PrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37PrimaryGeneratorAction* action_;
  G4UIdirectory*          gunDir;
  //  G4UIcmdWithAString*     RndmCmd;
  G4UIcmdWithADouble *set_pol_cmd_;
  G4UIcmdWithADouble *set_ali_cmd_;
  G4UIcmdWithAnInteger *set_recoil_charge_cmd_;
  G4UIcmdWith3VectorAndUnit *set_cloud_temperature_v_;
  G4UIcmdWithADoubleAndUnit *set_cloud_temperature_d_;
  G4UIcmdWith3VectorAndUnit *set_initial_cloud_size_v_;
  G4UIcmdWithADoubleAndUnit *set_initial_cloud_size_d_;
  G4UIcmdWith3VectorAndUnit *set_cloud_center_;
  G4UIcmdWithABool *set_make_beta_;
  G4UIcmdWithABool *set_make_recoil_;
  G4UIcmdWithABool *set_make_shakeoff_electrons_;
  G4UIcmdWith3VectorAndUnit *set_cloud_sail_velocity_v_;
  G4UIcmdWithADoubleAndUnit *set_cloud_sail_velocity_d_;
  G4UIcmdWithADouble *set_minimum_cos_theta_cmd_;
  G4UIcmdWithADoubleAndUnit *set_cone_half_angle_cmd_;
  G4UIcmdWithADouble *set_BigA;
};

//----------------------------------

#endif

