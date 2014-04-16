// Authors: Spencer Behling and Benjamin Fenker 2013
#ifndef K37StripDetectorMessenger_h 
#define K37StripDetectorMessenger_h 1

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

#include "K37StripDetectorDigitizer.hh"

class K37StripDetectorDigitizer;

class K37StripDetectorMessenger : public G4UImessenger {
 public :
  explicit K37StripDetectorMessenger(K37StripDetectorDigitizer *digitizer);
  ~K37StripDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37StripDetectorDigitizer *digitizer_;

  G4UIdirectory *dir_;
  G4UIcmdWithAString *resolution_fnameX_cmd_;
  G4UIcmdWithAString *resolution_fnameY_cmd_;
  G4UIcmdWithABool *apply_resolution_cmd_;
};

#endif
