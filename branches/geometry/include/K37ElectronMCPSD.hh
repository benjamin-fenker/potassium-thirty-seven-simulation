// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37ElectronMCPSD_h
#define K37ElectronMCPSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37ElectronMCPHit.hh"

class K37ElectronMCPSD : public G4VSensitiveDetector {
 public:
  explicit K37ElectronMCPSD(G4String name);
  ~K37ElectronMCPSD();
  void Initialize(G4HCofThisEvent *hit_collection_this_event);
  G4bool ProcessHits(G4Step *a_step, G4TouchableHistory* history);
  //   void EndOfEvent(G4HCofThisEvent *hit_collection_this_event);

 private:
  K37ElectronMCPHitsCollection *emcp_hit_collection_;
};

#endif
