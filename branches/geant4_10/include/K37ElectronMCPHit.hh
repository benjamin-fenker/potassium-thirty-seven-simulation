// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37ElectronMCPHit_h
#define K37ElectronMCPHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class K37ElectronMCPHit : public G4VHit {
 public:
  K37ElectronMCPHit();
  ~K37ElectronMCPHit();
  K37ElectronMCPHit(const K37ElectronMCPHit &right);
  const K37ElectronMCPHit& operator=(const K37ElectronMCPHit &right);
  G4int operator==(const K37ElectronMCPHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  inline void SetEnergy(G4double energy_) {energy = energy_;}
  inline G4double GetEnergy() {return energy;}
  // ----------------------------------------------
  inline void SetTime(G4double time_) {time = time_;}
  inline G4double GetTime() {return time;}
  // ----------------------------------------------
  inline void SetParticlePDG(G4int pdg_) {particle_pdg = pdg_;}
  inline G4int GetParticlePDG() {return particle_pdg;}
 private:
  G4double energy;
  G4double time;
  G4int particle_pdg;
};

typedef G4THitsCollection<K37ElectronMCPHit> K37ElectronMCPHitsCollection;

extern G4Allocator<K37ElectronMCPHit> K37ElectronMCPHitHitAllocator;

inline void* K37ElectronMCPHit::operator new(size_t) {
  void *aHit;
  aHit = static_cast<void *>(K37ElectronMCPHitHitAllocator.MallocSingle());
  return aHit;
}

inline void K37ElectronMCPHit::operator delete(void *aHit) {
  K37ElectronMCPHitHitAllocator.
      FreeSingle(static_cast<K37ElectronMCPHit*>(aHit));
}
#endif
