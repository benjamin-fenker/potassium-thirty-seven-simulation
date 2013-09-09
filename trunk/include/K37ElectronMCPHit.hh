// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37ElectronMCPHit_h
#define K37ElectronMCPHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class K37ElectronMCPHit : public G4VHit {
 private:
  G4double energy_;
  G4double time_;
  G4int particle_pdg_;
 public:
  K37ElectronMCPHit();
  ~K37ElectronMCPHit();
  void SetEnergy(G4double energy) {energy_ = energy;}
  void SetParticlePDG(G4int pdg) {particle_pdg_ = pdg;}
  void SetTime(G4double time) {time_ = time;}
  G4double GetEnergy() {return energy_;}
  G4int GetParticlePDG() {return particle_pdg_;}
  G4double GetTime() {return time_;}
};

typedef G4THitsCollection<K37ElectronMCPHit> K37ElectronMCPHitsCollection;

#endif
