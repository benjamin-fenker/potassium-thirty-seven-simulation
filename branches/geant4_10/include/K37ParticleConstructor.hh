// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37ParticleConstructor_h
#define K37ParticleConstructor_h

#include "G4AntiNeutrinoE.hh"
#include "G4AntiProton.hh"
#include "G4ChargedGeantino.hh"
#include "G4ComptonScattering.hh"
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4GammaConversion.hh"
#include "G4Geantino.hh"
#include "G4IonConstructor.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4KaonMinus.hh"
#include "G4KaonPlus.hh"
#include "G4ParticleTypes.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4Positron.hh"
#include "G4ProcessManager.hh"
#include "G4Proton.hh"
#include "G4NeutrinoE.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4eplusAnnihilation.hh"
#include "G4hMultipleScattering.hh"
#include "G4ionIonisation.hh"
#include "globals.hh"

#include "K37ParticleConstructor.hh"

class K37ParticleConstructor : public G4VPhysicsConstructor {
 public:
  explicit K37ParticleConstructor(const G4String& name = "K37");
  ~K37ParticleConstructor();

  void ConstructParticle();
  void ConstructProcess() {}
};

#endif
