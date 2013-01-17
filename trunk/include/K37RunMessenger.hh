// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37RunMessenger_h
#define K37RunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

//---------------------------------

class K37RunAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

//---------------------------------

class K37RunMessenger: public G4UImessenger {
 public:
  explicit K37RunMessenger(K37RunAction* ra);
  ~K37RunMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37RunAction*                 runAction;
  G4UIdirectory*                RunDirectory;
  G4UIcmdWithAnInteger*         SaveFilesCommand;
  G4UIcmdWithAString*           printEnergyLossTable;
  G4UIcmdWithABool*             printAnnihilation;
  G4UIcmdWithABool*             printTheVolumeNames;
};

#endif

