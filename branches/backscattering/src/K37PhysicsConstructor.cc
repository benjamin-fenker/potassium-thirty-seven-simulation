// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37PhysicsConstructor.hh"

K37PhysicsConstructor::K37PhysicsConstructor(const G4String& name)
    :  G4VPhysicsConstructor(name) {}

K37PhysicsConstructor::~K37PhysicsConstructor() {}

void K37PhysicsConstructor::ConstructProcess() {
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
      pmanager -> AddProcess(new G4StepLimiter(1.0*cm), -1, -1, 4);
    } else if (particleName == "Ar37Neutral" || particleName == "Ar37PlusOne") {
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
  G4Decay *theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      // G4cout << particle -> GetParticleName() << G4endl;
      // G4DecayTable *table = particle -> GetDecayTable();
      // if (table) table -> DumpInfo();
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}
