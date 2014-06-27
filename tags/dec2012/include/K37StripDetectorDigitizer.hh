// Authors: Spencer Behling and Benjamin Fenker 2014

#ifndef K37StripDetectorDigitizer_hh
#define K37StripDetectorDigitizer_hh 1

#include <vector>

#include <G4VDigitizerModule.hh>

#include "K37SortingDigitizer.hh"
#include "K37StripDetectorHit.hh"
#include "K37StripDetectorMessenger.hh"

class K37StripDetectorMessenger;

// Class to handle digitization of strip detector signals.  Sorts
// energy deposited into strips and into energy deposited by which
// particle.
class K37StripDetectorDigitizer : public K37SortingDigitizer {
 public:
  explicit K37StripDetectorDigitizer(const G4String &name,
                                     const char *res_name_x,
                                     const char *res_name_y);
  ~K37StripDetectorDigitizer();

  void Digitize();
  void InitializeData();

  std::vector<G4double> GetEnergyXstrip() {return energy_dep_Xstrip_;}
  std::vector<G4double> GetEnergyYstrip() {return energy_dep_Ystrip_;}
  G4int GetParticleCode() {return particle_code_;}
  void SetApplyFiniteResolution(G4bool apply) {
    apply_finite_resolution_ = apply;
  }

  // Wraps GetResolution and aborts if file doesn't exist.  Intended
  // to be used by messenger classes.
  void SetResolutionX(const char *fname);
  void SetResolutionY(const char *fname);

 private:
  std::vector<G4double> energy_dep_Xstrip_;
  std::vector<G4double> energy_dep_Ystrip_;
  std::vector<G4double> resolution_Xstrip_;
  std::vector<G4double> resolution_Ystrip_;
  G4int particle_code_;
  G4bool apply_finite_resolution_;

  std::vector<G4double> GetEDepVector(
      const K37StripDetectorHitsCollection *sdhc, G4int coordinate);
  std::vector<G4double> GetEDepVectorX(
      const K37StripDetectorHitsCollection *sdhc);
  std::vector<G4double> GetEDepVectorY(
      const K37StripDetectorHitsCollection *sdhc);

  std::vector<G4double> GetResolution(const char* fname);
  // Blur the energy in both X and Y.
  // Resolution(E_0) = sqrt(E_0*lambda)
  // True energy will be gaussian centered at E_0 with width = resolution(E_0)
  void ApplyResolution();

  K37StripDetectorMessenger *messenger_;
};

#endif
