// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ElectronMCPSD.hh"
#include "K37ElectronMCPHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"

K37ElectronMCPSD::K37ElectronMCPSD(G4String name)
    :G4VSensitiveDetector(name) {
  collectionName.insert("eMCP_HC");
}

K37ElectronMCPSD::~K37ElectronMCPSD() {}

void K37ElectronMCPSD::Initialize(G4HCofThisEvent* hit_collection_this_event) {
  emcp_hit_collection_ =
      new K37ElectronMCPHitsCollection(SensitiveDetectorName,
                                       collectionName[0]);
  hit_collection_this_event -> AddHitsCollection(4, emcp_hit_collection_);
}

G4bool K37ElectronMCPSD::ProcessHits(G4Step *step, G4TouchableHistory*) {
  K37ElectronMCPHit *new_hit = new K37ElectronMCPHit();
  G4Track *track = step -> GetTrack();

  // Kill electrons once they hit this detector to avoid large number of bounces
  //  G4cout << track -> GetParticleDefinition() -> GetParticleName() << G4endl;
  if (track -> GetParticleDefinition() -> GetParticleType() == "e-") {
    track -> SetTrackStatus(fStopAndKill);
  }

  new_hit -> SetEnergy(step -> GetTotalEnergyDeposit());
  new_hit -> SetTime(track -> GetLocalTime());
  new_hit ->
      SetParticlePDG(track -> GetParticleDefinition() -> GetPDGEncoding());
  emcp_hit_collection_ -> insert(new_hit);
  return true;
}
