// Authors: Spencer Behling and Benjamin Fenker 2014

#ifndef K37ScintillatorDigitizer_hh 
#define K37ScintillatorDigitizer_hh 1

#include <G4VDigitizerModule.hh>

#include "K37ScintillatorMessenger.hh"

class K37ScintillatorMessenger;
// Class to handle digitization of scintillator signals.  For now,
// this class will collect scintillator hits and optionally apply a
// resolution to blurr the energy.  In the future, this V792 hardware
// can be simulated in this module.  
class K37ScintillatorDigitizer : public G4VDigitizerModule {
 public:
  explicit K37ScintillatorDigitizer(const G4String &name);
  ~K37ScintillatorDigitizer();

  void Digitize();
  void InitializeData();

  // Getters
  G4double GetEnergyTotal() {return energy_dep_total_;}
  G4double GetEnergyElectron() {return energy_dep_electron_;}
  G4double GetEnergyPositron() {return energy_dep_positron_;}
  G4double GetEnergyGamma() {return energy_dep_gamma_;}
  G4double GetEnergyOther() {return energy_dep_other_;}
  G4int GetParticleCode() {return particle_code_;}
  G4double GetHitTime() {return hit_time_;}
  
  // Setters
  void SetApplyFiniteResolution(G4bool apply) {
    apply_finite_resolution_ = apply;
  }
  void SetThreshold(G4double th) {threshold_ = th;}
  void SetResolutionSigma0(G4double s0) {resolution_sigma0 = s0;}
  void SetResolutionMu(G4double mu) {resolution_mu = mu;}

 private:
  void SortEdepByParticle(const G4int &pdgCode_, const G4double &energyHit_);
  void ApplyResolution();
  G4double energy_dep_total_;
  G4double energy_dep_electron_;
  G4double energy_dep_positron_;
  G4double energy_dep_gamma_;
  G4double energy_dep_other_;
  G4int particle_code_;
  G4double hit_time_;

  G4bool apply_finite_resolution_;

  G4double resolution_sigma0; // Constant intrinsic resolution
  G4double resolution_mu;     // Energy dependend resolution

  G4double threshold_;           // Time will only be read if Etot > this

  K37ScintillatorMessenger *messenger_;
};

#endif
