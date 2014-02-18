// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37SteppingAction_H
#define K37SteppingAction_H 1

#include <map>
#include <string>
#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "K37_Data.hh"

//class K37EventAction;
class K37SteppingActionMessenger;

class K37SteppingAction : public G4UserSteppingAction {
 public:
  K37SteppingAction();
  virtual ~K37SteppingAction();

  virtual void UserSteppingAction(const G4Step*);

  void SetActiveChannels(std::map<std::string,
        K37_ABC::K37_Data*> *active_channels_)
  {
    active_channels = active_channels_;
  }

  void TrackNames(const bool &ToTrack_)
  {
     ToTrack = ToTrack_;
  }

 private:
  std::map<std::string, K37_ABC::K37_Data*> *active_channels;
  K37SteppingActionMessenger *stepping_messenger;
  bool ToTrack;
};

#endif

