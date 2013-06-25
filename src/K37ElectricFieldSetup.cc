// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ElectricFieldSetup.hh"
#include "K37FieldMessenger.hh"

#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

//--------------------------------------------------------------------
//
//  Constructors:

K37ElectricFieldSetup::K37ElectricFieldSetup()
  :fChordFinder(0), fStepper(0), fIntgrDriver(0), fFieldMessenger(0) {
  fElFieldValue = G4ThreeVector(0.0, 350.0*volt/cm, 0.0);
  fEMfield = new G4UniformElectricField(fElFieldValue);

  //  G4cout << "Making field!" << G4endl;
  fFieldMessenger = new K37FieldMessenger(this);
  fEquation = new G4EqMagElectricField(fEMfield);
  fMinStep     = 0.010*mm;  // minimal step of 10 microns
  fStepperType = 4;         // ClassicalRK4 -- the default stepper

  fFieldManager = GetGlobalFieldManager();
  UpdateField();
}

//--------------------------------------------------------------------

K37ElectricFieldSetup::K37ElectricFieldSetup(G4ThreeVector pFV)
  : fChordFinder(0), fStepper(0), fIntgrDriver(0) {
  fElFieldValue = pFV;
  fEMfield = new G4UniformElectricField(pFV);


  fFieldMessenger = new K37FieldMessenger(this);
  fEquation = new G4EqMagElectricField(fEMfield);
  fMinStep     = 0.010*mm;  // minimal step of 10 microns
  fStepperType = 4;         // ClassicalRK4 -- the default stepper

  fFieldManager = GetGlobalFieldManager();
  UpdateField();
}

//--------------------------------------------------------------------

K37ElectricFieldSetup::~K37ElectricFieldSetup() {
  if (fChordFinder)   delete fChordFinder;
  if (fStepper)       delete fStepper;
  if (fEquation)      delete fEquation;
  if (fEMfield)       delete fEMfield;
  if (fFieldMessenger)   delete fFieldMessenger;
}

//--------------------------------------------------------------------
//
// Register this field to 'global' Field Manager and
// Create Stepper and Chord Finder with predefined type, minstep (resp.)
//

void K37ElectricFieldSetup::UpdateField() {
  SetStepper();

  // G4cout << "The minimal step is equal to " << fMinStep/mm
  //        << " mm" << G4endl;

  fFieldManager -> SetDetectorField(fEMfield);

  if (fChordFinder) delete fChordFinder;
  // fChordFinder = new G4ChordFinder( fEMfield, fMinStep, fStepper);

  fIntgrDriver = new G4MagInt_Driver(fMinStep,
                                     fStepper,
                                     fStepper->GetNumberOfVariables() );

  fChordFinder = new G4ChordFinder(fIntgrDriver);

  fFieldManager -> SetChordFinder(fChordFinder);
}

//--------------------------------------------------------------------
//
// Set stepper according to the stepper type
//

void K37ElectricFieldSetup::SetStepper() {
  G4int nvar = 8;

  if (fStepper) delete fStepper;
  G4cout << "For electric field tracking...";
  switch (fStepperType) {
  case 0:
    fStepper = new G4ExplicitEuler(fEquation, nvar);
    G4cout << "G4ExplicitEuler is calledS" << G4endl;
    break;
  case 1:
    fStepper = new G4ImplicitEuler(fEquation, nvar);
    G4cout << "G4ImplicitEuler is called" << G4endl;
    break;
  case 2:
    fStepper = new G4SimpleRunge(fEquation, nvar);
    G4cout << "G4SimpleRunge is called" << G4endl;
    break;
  case 3:
    fStepper = new G4SimpleHeum(fEquation, nvar);
    G4cout << "G4SimpleHeum is called" << G4endl;
    break;
  case 4:
    fStepper = new G4ClassicalRK4(fEquation, nvar);
    G4cout << "G4ClassicalRK4 (default) is called" << G4endl;
    break;
  case 5:
    fStepper = new G4CashKarpRKF45(fEquation, nvar);
    G4cout << "G4CashKarpRKF45 is called" << G4endl;
    break;
  case 6:
    fStepper = 0;  // new G4RKG3_Stepper( fEquation, nvar );
    G4cout << "G4RKG3_Stepper is not currently working for Electric Field"
           << G4endl;
    break;
  case 7:
    fStepper = 0;  // new G4HelixExplicitEuler( fEquation );
    G4cout << "G4HelixExplicitEuler is not valid for Electric Field"
           << G4endl;
    break;
  case 8:
    fStepper = 0;  // new G4HelixImplicitEuler( fEquation );
    G4cout << "G4HelixImplicitEuler is not valid for Electric Field"
           << G4endl;
    break;
  case 9:
    fStepper = 0;  // new G4HelixSimpleRunge( fEquation );
    G4cout << "G4HelixSimpleRunge is not valid for Electric Field"
           << G4endl;
    break;
  default:
    fStepper = 0;
  }
}

//--------------------------------------------------------------------
//
// Set the value of the Global Field to fieldValue along Z
//

void K37ElectricFieldSetup::SetFieldValue(G4double fieldValue) {
  // G4ThreeVector fieldVector(0.0, 0.0, fieldValue);
  G4ThreeVector fieldVector(0.0, fieldValue, 0.0);
  SetFieldValue(fieldVector);
}

//--------------------------------------------------------------------
//
// Set the value of the Global Field value to fieldVector
//

void K37ElectricFieldSetup::SetFieldValue(G4ThreeVector fieldVector) {
  // Find the Field Manager for the global field
  //  G4FieldManager* fieldMgr= GetGlobalFieldManager();
  fElFieldValue = fieldVector;
  if (fieldVector != G4ThreeVector(0., 0., 0.)) {
    if (fEMfield) delete fEMfield;

    fEMfield = new  G4UniformElectricField(fieldVector);

    if (fEquation) delete fEquation;
    fEquation = new G4EqMagElectricField(fEMfield);
    // fEquation->SetFieldObj(fEMfield);  // must now point to the new field

    UpdateField();

    // fieldMgr->SetDetectorField(fEMfield);
  } else {
    // If the new field's value is Zero, then it is best to
    //  insure that it is not used for propagation.
    if (fEMfield) delete fEMfield;
    fEMfield = 0;
    // fEquation->SetFieldObj(fEMfield);   // As a double check ...
    if (fEquation) delete fEquation;
    fEquation = 0;
    UpdateField();
    // fieldMgr->SetDetectorField(fEMfield);
  }
}

G4ThreeVector K37ElectricFieldSetup::GetConstantFieldValue() {
  return fElFieldValue;
}
//--------------------------------------------------------------------
//
//  Utility method

G4FieldManager*  K37ElectricFieldSetup::GetGlobalFieldManager() {
  return G4TransportationManager::GetTransportationManager()
    ->GetFieldManager();
}
