#ifndef K37SteppingActionMessenger_h
#define K37SteppingActionMessenger_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"


#include "K37SteppingAction.hh"

class G4UIdirectory;
class K37SteppingAction;
//---------------------------------

class K37SteppingActionMessenger: public G4UImessenger
{
 public:
  explicit K37SteppingActionMessenger(K37SteppingAction* stepping_action_);
  ~K37SteppingActionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37SteppingAction *stepping_action;
  G4UIdirectory *stepping_directory;
  G4UIdirectory *main_directory;
  G4UIcmdWithABool *set_track_volumes;
};

#endif

