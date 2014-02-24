// Authors: Spencer Behling and Benjamin Fenker 2013

#include "globals.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4Decay.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4hMultipleScattering.hh"
#include "G4IonConstructor.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"
#include "G4DecayTable.hh"

#include "K37PhysicsList.hh"

//----------------------------------------------------------------------------

K37PhysicsList::K37PhysicsList()
  : G4VModularPhysicsList(), theDecayProcess(0) {
  //  defaultCutValue = 1.0*cm;
  defaultCutValue = 10*micrometer;
  SetVerboseLevel(1);
  stepLimiter = new G4StepLimiter();
}

//----------------------------------------------------------------------------

K37PhysicsList::~K37PhysicsList() {
  delete theDecayProcess;
  delete stepLimiter;
}

//----------------------------------------------------------------------------

void K37PhysicsList::ConstructParticle() {
  G4cout << " ******************************       " << G4endl;
  G4cout << "              WARNING                 " << G4endl;
  G4cout << " K37PhysicsList::ConstructParticle is ";
  G4cout << " obsolete.  You should not have       ";
  G4cout << " reached this pont.  Enter to continue";
  G4cout << " ******************************       " << G4endl;
  G4cin.ignore();
}

//----------------------------------------------------------------------------

void K37PhysicsList::ConstructProcess() {
  AddTransportation();
  ConstructEM();
  ConstructDecay();
  AddStepMax();
}

//----------------------------------------------------------------------------

void K37PhysicsList::ConstructDecay() {
  G4cout << " ******************************    " << G4endl;
  G4cout << "              WARNING              " << G4endl;
  G4cout << " K37PhysicsList::ConstructDeacy is ";
  G4cout << " obsolete.  You should not have    ";
  G4cout << " reached this pont.  Enter to      ";
  G4cout << " continue" << G4endl;
  G4cout << " ******************************    " << G4endl;
  G4cin.ignore();
}


void K37PhysicsList::ConstructEM() {
  G4cout << " ****************************** " << G4endl;
  G4cout << "              WARNING           " << G4endl;
  G4cout << " K37PhysicsList::ConstructEM is ";
  G4cout << " obsolete.  You should not have ";
  G4cout << " reached this pont.  Enter to   ";
  G4cout << " continue" << G4endl;
  G4cout << " ****************************** " << G4endl;
  G4cin.ignore();
}



//----------------------------------------------------------------------------

void K37PhysicsList::AddStepMax() {
  // Step limitation seen as a process
  // G4StepLimiter* stepLimiter = new G4StepLimiter();
  // G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();

  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (particle->GetPDGCharge() != 0.0) {
      pmanager ->AddDiscreteProcess(stepLimiter);
      // pmanager ->AddDiscreteProcess(userCuts);
    }
  }
}

//----------------------------------------------------------------------------

void K37PhysicsList::SetCuts() {
  // G4VUserPhysicsList::SetCutsWithDefault method sets
  // the default cut value for all particle types
  //  G4cout << "K37PhysicsList::SetCuts()" << G4endl;
  SetCutsWithDefault();
  if (verboseLevel>0) DumpCutValuesTable();
}

//----------------------------------------------------------------------------

