// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37RecoilMCPSD.hh"
#include "K37RecoilMCPHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"

K37RecoilMCPSD::K37RecoilMCPSD(G4String name)
    :G4VSensitiveDetector(name) {
  collectionName.insert("rMCP_HC");
}

K37RecoilMCPSD::~K37RecoilMCPSD() {}

void K37RecoilMCPSD::Initialize(G4HCofThisEvent* hit_collection_this_event) {
  rmcp_hit_collection_ =
      new K37RecoilMCPHitsCollection(SensitiveDetectorName,
                                       collectionName[0]);
  hit_collection_this_event -> AddHitsCollection(5, rmcp_hit_collection_);
}

G4bool K37RecoilMCPSD::ProcessHits(G4Step *step, G4TouchableHistory*) {
  K37RecoilMCPHit *new_hit = new K37RecoilMCPHit();
  G4Track *track = step -> GetTrack();

  new_hit -> SetEnergy(step -> GetTotalEnergyDeposit());
  new_hit -> SetTime(track -> GetGlobalTime());
  // PreStep and PostStep both exist.  The examples all use PreStep so that's
  // what I'll go with
  new_hit -> SetXPos(step -> GetPreStepPoint() -> GetPosition().x());
  new_hit -> SetZPos(step -> GetPreStepPoint() -> GetPosition().z());
  rmcp_hit_collection_ -> insert(new_hit);
  return true;
}
