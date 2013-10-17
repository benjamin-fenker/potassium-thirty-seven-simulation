// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37RecoilMCPHit_h
#define K37RecoilMCPHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class K37RecoilMCPHit : public G4VHit {
 public:
  K37RecoilMCPHit();
  ~K37RecoilMCPHit();
  K37RecoilMCPHit(const K37RecoilMCPHit &right);
  const K37RecoilMCPHit& operator=(const K37RecoilMCPHit &right);
  G4int operator==(const K37RecoilMCPHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  inline void SetEnergy(G4double energy_) {energy = energy_;}
  inline G4double GetEnergy() {return energy;}
  // ----------------------------------------------
  inline void SetTime(G4double time_) {time = time_;}
  inline G4double GetTime() {return time;}
  // ----------------------------------------------
  inline void SetXPos(G4double x_pos_) {x_pos = x_pos_;}
  inline G4double GetXPos() {return x_pos;}
  // ----------------------------------------------
  inline void SetZPos(G4double z_pos_) {z_pos = z_pos_;}
  inline G4double GetZPos() {return z_pos;}
  // ----------------------------------------------
  inline void SetParticlePDG(G4int pdg_) {particle_pdg = pdg_;}
  inline G4int GetParticlePDG() {return particle_pdg;}
  // ----------------------------------------------
  inline void SetParticleCharge(G4int charge_) {particle_charge = charge_;}
  inline G4int GetParticleCharge() {return particle_charge;}

 private:
  G4double energy;
  G4double time;
  G4double x_pos;
  G4double z_pos;
  G4int particle_pdg;
  G4int particle_charge;
};

typedef G4THitsCollection<K37RecoilMCPHit> K37RecoilMCPHitsCollection;

extern G4Allocator<K37RecoilMCPHit> K37RecoilMCPHitAllocator;

inline void* K37RecoilMCPHit::operator new(size_t) {
  void *aHit;
  aHit = static_cast<void *>(K37RecoilMCPHitAllocator.MallocSingle());
  return aHit;
}

inline void K37RecoilMCPHit::operator delete(void *aHit) {
  K37RecoilMCPHitAllocator.
      FreeSingle(static_cast<K37RecoilMCPHit*>(aHit));
}

#endif
