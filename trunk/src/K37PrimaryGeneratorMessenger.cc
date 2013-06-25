// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37PrimaryGeneratorMessenger.hh"


// ----------------------------------

K37PrimaryGeneratorMessenger::K37PrimaryGeneratorMessenger(
                                  K37PrimaryGeneratorAction* Gun)
  :action_(Gun) {
  gunDir = new G4UIdirectory("/K37/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");

  RndmCmd = new G4UIcmdWithAString("/K37/gun/betaDistribution", this);
  RndmCmd->SetGuidance("Shoot with beta distribution the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice", true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  set_pol_cmd_ = new G4UIcmdWithADouble("/K37/gun/setPolarization", this);
  set_pol_cmd_ -> SetGuidance("Enter a new polarization (-1 -> 1)");
  set_pol_cmd_ -> SetParameterName("Polarization", false);
  set_pol_cmd_ -> SetDefaultValue(1.0);

  set_ali_cmd_ = new G4UIcmdWithADouble("/K37/gun/setAlignment", this);
  set_ali_cmd_ -> SetGuidance("Enter a new alignment (-1 -> 1)");
  set_ali_cmd_ -> SetParameterName("Alignment", false);
  set_ali_cmd_ -> SetDefaultValue(1.0);

  set_recoil_charge_cmd_ = new G4UIcmdWithAnInteger("/K37/gun/setRecoilCharge",
                                                    this);
  set_recoil_charge_cmd_ -> SetGuidance("Enter charge state of recoil Ar.");
  set_recoil_charge_cmd_ -> SetGuidance("Available charge states: +1 -> +3");
  set_recoil_charge_cmd_ -> SetParameterName("Charge state", true);
  set_recoil_charge_cmd_ -> SetDefaultValue(1);
}

// ----------------------------------

K37PrimaryGeneratorMessenger::~K37PrimaryGeneratorMessenger() {
  delete RndmCmd;
  delete gunDir;
}

// ----------------------------------

void K37PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue) {
  if (command == RndmCmd) {
    action_ -> setRandomFlag(newValue);
  }
  if (command == set_pol_cmd_) {
    action_ -> SetPolarization(set_pol_cmd_->GetNewDoubleValue(newValue));
  }
  if (command == set_ali_cmd_) {
    action_ -> SetAlignment(set_ali_cmd_->GetNewDoubleValue(newValue));
  }
  if (command == set_recoil_charge_cmd_) {
    action_ ->
        SetRecoilCharge(set_recoil_charge_cmd_ -> GetNewIntValue(newValue));
  }
}

// ----------------------------------

