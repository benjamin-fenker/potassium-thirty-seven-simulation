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

#include "K37PhysicsList.hh"
#include "K37ArNeutralDefinition.hh"
#include "K37ArMinusDefinition.hh"

//----------------------------------------------------------------------------

K37PhysicsList::K37PhysicsList()
  : G4VUserPhysicsList(), theDecayProcess(0) {
  defaultCutValue = 1.0*cm;
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
  // In this method, ensures that e+/e- nu_e/bar{nu_e} and gamma are created

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();
  //  e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // nu_e
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();

  // Ar37Neutral && Ar37Minus
  K37Ar37Neutral:: Ar37NeutralDefinition();
  K37Ar37Minus:: Ar37MinusDefinition();

  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
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
  // Add Decay Process
  theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}


void K37PhysicsList::ConstructEM() {
  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      // gamma
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);

    } else if (particleName == "e-") {
      // electron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);

    } else if (particleName == "e+") {
      // positron
      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation,    0, -1, 4);
    } else if (particleName == "GenericIon") {
      // OBJECT may be dynamically created as either a GenericIon or nucleus
      // G4Nucleus exists and therefore has particle type nucleus
      // genericIon:
      // multiple scattering
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      // ionisation
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 20*um);
      pmanager->AddProcess(ionIoni, -1, 2, 2);
    } else if (particleName == "Ar37Neutral") {
      // OBJECT may be dynamically created as either a GenericIon or nucleus
      // G4Nucleus exists and therefore has particle type nucleus
      // genericIon:
      // multiple scattering
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      // ionisation
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 20*um);
      pmanager->AddProcess(ionIoni, -1, 2, 2);
    } else if (particleName == "Ar37Minus") {
      // OBJECT may be dynamically created as either a GenericIon or nucleus
      // G4Nucleus exists and therefore has particle type nucleus
      // genericIon:
      // multiple scattering
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      // ionisation
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 20*um);
      pmanager->AddProcess(ionIoni, -1, 2, 2);
    }
  }
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

  SetCutsWithDefault();

  if (verboseLevel>0) DumpCutValuesTable();
}

//----------------------------------------------------------------------------

