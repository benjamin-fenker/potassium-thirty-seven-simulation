// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37RecoilMCPHit_h
#define K37RecoilMCPHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class K37RecoilMCPHit : public G4VHit {
 private:
  G4double energy_;
  G4double time_;
  G4double x_pos_;
  G4double z_pos_;

 public:
  K37RecoilMCPHit();
  ~K37RecoilMCPHit();
  void SetEnergy(G4double energy) {energy_ = energy;}
  void SetTime(G4double time) {time_ = time;}
  void SetXPos(G4double x_pos) {x_pos_ = x_pos;}
  void SetZPos(G4double z_pos) {z_pos_ = z_pos;}
  G4double GetEnergy() {return energy_;}
  G4double GetTime() {return time_;}
  G4double GetXPos() {return x_pos_;}
  G4double GetZPos() {return z_pos_;}
};

typedef G4THitsCollection<K37RecoilMCPHit> K37RecoilMCPHitsCollection;

#endif
