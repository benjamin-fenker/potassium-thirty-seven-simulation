// Authors: Spencer Behling and Benjamin Fenker 2014

#ifndef K37SortingDigitizer_hh 
#define K37SortingDigitizer_hh 1

#include <G4VDigitizerModule.hh>

class K37SortingDigitizer : public G4VDigitizerModule {
 public:
  explicit K37SortingDigitizer(const G4String &name);
  ~K37SortingDigitizer();
 
  void InitializeData();

  // Getters
  G4double GetEnergyTotal() {return energy_dep_total_;}
  G4double GetEnergyElectron() {return energy_dep_electron_;}
  G4double GetEnergyPositron() {return energy_dep_positron_;}
  G4double GetEnergyGamma() {return energy_dep_gamma_;}
  G4double GetEnergyOther() {return energy_dep_other_;}

 protected:
  void SortEdepByParticle(const G4int &pdgCode_, const G4double &energyHit_);

  G4double energy_dep_total_;
  G4double energy_dep_electron_;
  G4double energy_dep_positron_;
  G4double energy_dep_gamma_;
  G4double energy_dep_other_;
};

#endif
