// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37FieldMessenger_h
#define K37FieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class K37ElectricFieldSetup;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3VectorAndUnit;


class K37FieldMessenger: public G4UImessenger {
 public:
  explicit K37FieldMessenger(K37ElectricFieldSetup* efs);
  ~K37FieldMessenger();

  void SetNewValue(G4UIcommand*, G4String);
  void SetNewValue(G4UIcommand*, G4int);

 private:
  K37ElectricFieldSetup*     fEFieldSetup;
  G4UIdirectory*             fieldDir;
  G4UIcmdWithAnInteger*      StepperCmd;
  G4UIcmdWithADoubleAndUnit* ElFieldCmd;
  G4UIcmdWithADoubleAndUnit* MinStepCmd;
  G4UIcmdWithoutParameter*   UpdateCmd;
  G4UIcmdWith3VectorAndUnit* changeFieldCmd;
};

#endif

