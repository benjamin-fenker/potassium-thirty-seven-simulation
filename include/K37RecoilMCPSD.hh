// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37RecoilMCPSD_h
#define K37RecoilMCPSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37RecoilMCPHit.hh"

class K37RecoilMCPSD : public G4VSensitiveDetector {
 public:
  explicit K37RecoilMCPSD(G4String name);
  ~K37RecoilMCPSD();
  void Initialize(G4HCofThisEvent *hit_collection_this_event);
  G4bool ProcessHits(G4Step *a_step, G4TouchableHistory* history);
  //   void EndOfEvent(G4HCofThisEvent *hit_collection_this_event);

 private:
  K37RecoilMCPHitsCollection *rmcp_hit_collection_;
};

#endif
