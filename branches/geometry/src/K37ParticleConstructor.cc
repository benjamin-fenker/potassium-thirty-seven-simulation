// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ParticleConstructor.hh"

K37ParticleConstructor::K37ParticleConstructor(const G4String& name)
    :  G4VPhysicsConstructor(name) {}

K37ParticleConstructor::~K37ParticleConstructor() {}

void K37ParticleConstructor::ConstructParticle() {
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

  // Necessary for livermore, penelope simulation
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}


