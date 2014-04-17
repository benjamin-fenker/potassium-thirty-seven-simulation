// Authors: Spencer Behling and Benjamin Fenker 2014

#include "K37SortingDigitizer.hh"

K37SortingDigitizer::K37SortingDigitizer(const G4String &name) :
    G4VDigitizerModule(name), energy_dep_total_(0.0), energy_dep_electron_(0.0),
    energy_dep_positron_(0.0), energy_dep_gamma_(0.0), energy_dep_other_(0.0) {
}

K37SortingDigitizer::~K37SortingDigitizer() {}

void K37SortingDigitizer::SortEdepByParticle(const G4int &pdgCode_,
                                                  const G4double &energyHit_) {
  energy_dep_total_ += energyHit_;
  
  switch(pdgCode_)
  {
    case 11: //Electron
      {
        energy_dep_electron_ += energyHit_;
        //G4cout<<"Electron: "<<energyUpperScint_Electron<<G4endl;
        break;
      }
    case -11: //Positrons negative for anti-matter
      {
        energy_dep_positron_ += energyHit_;
        //G4cout<<"Positron: "<<energyUpperScint_Positron<<G4endl;
        break;
      }
    case 22: //Gamma 
      {
        energy_dep_gamma_ += energyHit_;
        //G4cout<<"Gamma: "<<energyUpperScint_Gamma<<G4endl;
        break;
      }
    default: //Everything else
      {
        energy_dep_other_ += energyHit_;
        //G4cout<<"Delta: "<<energyUpperScint_AllElse<<G4endl;
        break;
      }
  }
}

void K37SortingDigitizer::InitializeData() {
  energy_dep_total_ = 0.0;
  energy_dep_electron_ = 0.0;
  energy_dep_positron_ = 0.0;
  energy_dep_gamma_ = 0.0;
  energy_dep_other_ = 0.0;
}
