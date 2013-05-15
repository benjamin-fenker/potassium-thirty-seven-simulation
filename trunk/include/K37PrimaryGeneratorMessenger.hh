// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37PrimaryGeneratorMessenger_h
#define K37PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class K37PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;

//----------------------------------

class K37PrimaryGeneratorMessenger: public G4UImessenger {
 public:
  explicit  K37PrimaryGeneratorMessenger(K37PrimaryGeneratorAction* pga);
  virtual ~K37PrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37PrimaryGeneratorAction* Action;
  G4UIdirectory*          gunDir;
  G4UIcmdWithAString*     RndmCmd;
};

//----------------------------------

#endif
