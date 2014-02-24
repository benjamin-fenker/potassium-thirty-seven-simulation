// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37StackingActionMessenger.hh"

K37StackingActionMessenger::K37StackingActionMessenger(
    K37StackingAction *action)
    : action_(action) {
  stacking_dir_ = new G4UIdirectory("/K37/Stacking/");
  stacking_dir_ -> SetGuidance("Control stacking (which tracks to kill/keep");

  track_secondaries_cmd_ =
      new G4UIcmdWithABool("/K37/Stacking/setTrackSecondaries", this);
}

K37StackingActionMessenger::~K37StackingActionMessenger() {
  delete stacking_dir_;
  delete track_secondaries_cmd_;
}

void K37StackingActionMessenger::SetNewValue(G4UIcommand *command,
                                        G4String new_value) {
  if (command == track_secondaries_cmd_) {
    action_ -> SetTrackSecondaries(track_secondaries_cmd_ ->
                                   GetNewBoolValue(new_value));
    G4cout << new_value << G4endl;
  }
}
