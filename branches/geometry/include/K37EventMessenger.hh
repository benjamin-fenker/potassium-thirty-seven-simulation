// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37EventMessenger_h
#define K37EventMessenger_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"


#include "K37EventAction.hh"

class G4UIdirectory;
class K37EventAction;
//---------------------------------

class K37EventMessenger: public G4UImessenger {
 public:
  explicit K37EventMessenger(K37EventAction* event_action);
  ~K37EventMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37EventAction* event_action_;
  G4UIdirectory * event_directory_;

  G4UIcmdWithADoubleAndUnit *set_upper_scintillator_threshold_cmd_;
  G4UIcmdWithADoubleAndUnit *set_lower_scintillator_threshold_cmd_;
  G4UIcmdWithADoubleAndUnit *set_electron_mcp_threshold_cmd_;
  G4UIcmdWithABool *set_count_backscatters_cmd_;
};

#endif

