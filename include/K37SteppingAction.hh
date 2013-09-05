// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37SteppingAction_H
#define K37SteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class K37EventAction;
class K37ListOfVolumeNames;
class K37AnnihilationPosition;

class K37SteppingAction : public G4UserSteppingAction {
 public:
  K37SteppingAction(K37ListOfVolumeNames*);
  virtual ~K37SteppingAction();

  virtual void UserSteppingAction(const G4Step*);

 private:
  //  K37EventAction*             eventAction;
  K37ListOfVolumeNames*       listOfEnteredVolumes;
  //  K37AnnihilationPosition* annihilationPosition;
};

#endif

