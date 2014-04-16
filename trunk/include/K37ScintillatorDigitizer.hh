// Authors: Spencer Behling and Benjamin Fenker 2014

#ifndef K37ScintillatorDigitizer_hh 
#define K37ScintillatorDigitizer_hh 1

#include <G4VDigitizerModule.hh>

#include "K37ScintillatorMessenger.hh"
#include "K37SortingDigitizer.hh"

class K37ScintillatorMessenger;
// Class to handle digitization of scintillator signals.  For now,
// this class will collect scintillator hits and optionally apply a
// resolution to blurr the energy.  In the future, this V792 hardware
// can be simulated in this module.  
class K37ScintillatorDigitizer : public K37SortingDigitizer {
 public:
  explicit K37ScintillatorDigitizer(const G4String &name);
  ~K37ScintillatorDigitizer();

  void Digitize();
  void InitializeData();

  // Getters
  G4int GetParticleCode() {return particle_code_;}
  G4double GetHitTime() {return hit_time_;}
  G4bool IsTriggered() {return energy_dep_total_ > threshold_;}
  G4double GetThreshold() {return threshold_;}

  // Setters
  void SetApplyFiniteResolution(G4bool apply) {
    apply_finite_resolution_ = apply;
  }
  void SetThreshold(G4double th) {threshold_ = th;}
  void SetResolutionSigma0(G4double s0) {resolution_sigma0 = s0;}
  void SetResolutionLambda(G4double lambda) {resolution_lambda = lambda;}

 private:
  void ApplyResolution();
  G4int particle_code_;
  G4double hit_time_;

  G4bool apply_finite_resolution_;

  G4double resolution_sigma0; // Constant intrinsic resolution
  G4double resolution_lambda;     // Energy dependend resolution

  G4double threshold_;           // Time will only be read if Etot > this

  K37ScintillatorMessenger *messenger_;
};

#endif
