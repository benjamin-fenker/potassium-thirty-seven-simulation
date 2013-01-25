// Authors: Spencer Behling and Benjamin Fenker 2013

#include "globals.hh"

#include "K37FieldMessenger.hh"

#include "K37ElectricFieldSetup.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UnitsTable.hh"
//------------------------------------------------

K37FieldMessenger::K37FieldMessenger(K37ElectricFieldSetup* pEMfield)
  :fEFieldSetup(pEMfield) {
  new  G4UnitDefinition("kilovolt/cm", "kV/cm", "Electric field", kilovolt/cm);

  fieldDir = new G4UIdirectory("/K37/field/");
  fieldDir->SetGuidance("F02 field tracking control.");

  StepperCmd = new G4UIcmdWithAnInteger("/K37/field/setStepperType", this);
  StepperCmd->SetGuidance("Select stepper type for electric field");
  StepperCmd->SetParameterName("choice", true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


  UpdateCmd = new G4UIcmdWithoutParameter("/K37/field/update", this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  ElFieldCmd = new G4UIcmdWithADoubleAndUnit("/K37/field/setFieldZ", this);
  ElFieldCmd->SetGuidance("Define uniform Electric field.");
  ElFieldCmd->SetGuidance("Electric field will be in Z direction.");
  ElFieldCmd->SetGuidance("Value of Electric field has to be given in volt/m");
  ElFieldCmd->SetParameterName("Ez", false, false);
  ElFieldCmd->SetDefaultUnit("volt/m");
  ElFieldCmd->AvailableForStates(G4State_Idle);

  MinStepCmd = new G4UIcmdWithADoubleAndUnit("/K37/field/setMinStep", this);
  MinStepCmd->SetGuidance("Define minimal step");
  MinStepCmd->SetParameterName("min step", false, false);
  MinStepCmd->SetDefaultUnit("mm");
  MinStepCmd->AvailableForStates(G4State_Idle);

  changeFieldCmd = new G4UIcmdWith3VectorAndUnit("/K37/field/changeField",
                                                 this);
  changeFieldCmd -> SetGuidance("The field can be changed by inputting");
  changeFieldCmd -> SetGuidance("a ThreeVector with a unit");
  changeFieldCmd -> SetParameterName("fieldValueX", "fieldValueY",
                                     "fieldValueZ", false, false);
  // The .hh file give the purpose of the booleans ----
  // (const char * theNameX,const char * theNameY,const char * theNameZ, G4bool
  // omittable,G4bool currentAsDefault)
  changeFieldCmd -> SetDefaultUnit("kV/cm");
  changeFieldCmd -> AvailableForStates(G4State_PreInit, G4State_Idle);
}

//------------------------------------------------
K37FieldMessenger::~K37FieldMessenger() {
  delete StepperCmd;
  delete ElFieldCmd;
  delete MinStepCmd;
  delete fieldDir;
  delete UpdateCmd;
  delete changeFieldCmd;
}

//------------------------------------------------

void K37FieldMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == StepperCmd) {
    fEFieldSetup->SetStepperType(StepperCmd->GetNewIntValue(newValue));
  }
  if (command == UpdateCmd) {
    fEFieldSetup->UpdateField();
  }
  if (command == ElFieldCmd) {
    fEFieldSetup->SetFieldValue(ElFieldCmd->GetNewDoubleValue(newValue));
  }
  if (command == MinStepCmd) {
    fEFieldSetup->SetMinStep(MinStepCmd->GetNewDoubleValue(newValue));
  }
  if (command == changeFieldCmd) {
    fEFieldSetup->SetFieldValue(changeFieldCmd->GetNew3VectorValue(newValue));
  }
}

//------------------------------------------------
