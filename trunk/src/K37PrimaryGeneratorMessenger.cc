// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37PrimaryGeneratorMessenger.hh"

#include "K37PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


// ----------------------------------

K37PrimaryGeneratorMessenger::K37PrimaryGeneratorMessenger(
                                  K37PrimaryGeneratorAction* Gun)
  :Action(Gun) {
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
    Action -> setRandomFlag(newValue);
  }
  if (command == set_pol_cmd_) {
    Action -> SetPolarization(set_pol_cmd_->GetNewDoubleValue(newValue));
  }
  if (command == set_ali_cmd_) {
    Action -> SetAlignment(set_ali_cmd_->GetNewDoubleValue(newValue));
  }
}

// ----------------------------------

