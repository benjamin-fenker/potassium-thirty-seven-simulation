// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37StackingActionMessenger_h
#define K37StackingActionMessenger_h 1

#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

#include "K37StackingAction.hh"

class K37StackingAction;

class K37StackingActionMessenger: public G4UImessenger {
 public:
  explicit K37StackingActionMessenger(K37StackingAction* action);
  ~K37StackingActionMessenger();
  void SetNewValue(G4UIcommand* command, G4String new_value);

 private:
  K37StackingAction *action_;
  G4UIdirectory *stacking_dir_;
  G4UIcmdWithABool *track_secondaries_cmd_;
};

#endif
