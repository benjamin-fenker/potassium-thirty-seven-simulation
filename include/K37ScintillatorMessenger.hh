// Authors: Spencer Behling and Benjamin Fenker 2013
#ifndef K37ScintillatorMessenger_h 
#define K37ScintillatorMessenger_h 1

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIdirectory.hh>
#include <G4UImessenger.hh>

#include "K37ScintillatorDigitizer.hh"

class K37ScintillatorDigitizer;

class K37ScintillatorMessenger : public G4UImessenger {
 public:
  explicit K37ScintillatorMessenger(K37ScintillatorDigitizer *digitizer);
  ~K37ScintillatorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private :
  K37ScintillatorDigitizer *digitizer_;

  G4UIdirectory *dir_;
  G4UIcmdWithADoubleAndUnit *threshold_cmd_;
  G4UIcmdWithADoubleAndUnit *resolution_sigma0_cmd_;
  G4UIcmdWithADoubleAndUnit *resolution_lambda_cmd_;
  G4UIcmdWithABool *apply_resolution_cmd_;
};

#endif
