
#include "K37PrimaryGeneratorMessenger.hh"

#include "K37PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//----------------------------------

K37PrimaryGeneratorMessenger::K37PrimaryGeneratorMessenger(K37PrimaryGeneratorAction* Gun)
    :Action(Gun) {
    gunDir = new G4UIdirectory("/K37/gun/");
    gunDir->SetGuidance("PrimaryGenerator control");

    RndmCmd = new G4UIcmdWithAString("/K37/gun/betaDistribution",this);
    RndmCmd->SetGuidance("Shoot with beta distribution the incident particle.");
    RndmCmd->SetGuidance("  Choice : on(default), off");
    RndmCmd->SetParameterName("choice",true);
    RndmCmd->SetDefaultValue("on");
    RndmCmd->SetCandidates("on off");
    RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//----------------------------------

K37PrimaryGeneratorMessenger::~K37PrimaryGeneratorMessenger() {
    delete RndmCmd;
    delete gunDir;
}

//----------------------------------

void K37PrimaryGeneratorMessenger::SetNewValue( G4UIcommand* command, G4String newValue) {
    if( command == RndmCmd ) {
        Action-> setRandomFlag(newValue);
    }
}

//----------------------------------

