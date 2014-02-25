// Authors: Spencer Behling and Benjamin Fenker 2014
#include <G4DigiManager.hh>
#include <Randomize.hh>

#include "K37ScintillatorDigitizer.hh"
#include "K37ScintillatorHit.hh"

K37ScintillatorDigitizer::K37ScintillatorDigitizer(const G4String &name) :
    G4VDigitizerModule(name), energy_dep_total_(0.0), energy_dep_electron_(0.0),
    energy_dep_positron_(0.0), energy_dep_gamma_(0.0), energy_dep_other_(0.0),
    particle_code_(0), hit_time_(0.0), apply_finite_resolution_(true),
    resolution_sigma0(1.8*keV), resolution_mu(1.57*keV), threshold_(100*keV) {
  //  G4cout << "Constructor threshold: " << threshold_/keV << " keV" << G4endl;
  messenger_ = new K37ScintillatorMessenger(this);
}

K37ScintillatorDigitizer::~K37ScintillatorDigitizer() {}

void K37ScintillatorDigitizer::Digitize() {
  //  G4cout << "K37ScintillatorDigitizer::Digitize()" << G4endl;
  InitializeData();                     // Zeroes everything out

  // Setup the hits collection
  G4DigiManager *digitizer_manager(G4DigiManager::GetDMpointer());
  G4int hit_collection_id(digitizer_manager -> GetHitsCollectionID(
      moduleName + "HC"));
  const K37ScintillatorHitsCollection* hits_collection(
      static_cast<const K37ScintillatorHitsCollection*> (
          digitizer_manager -> GetHitsCollection(hit_collection_id)));

  if (hits_collection) {
    int n_hit = hits_collection->entries();
    K37ScintillatorHit *hit;
    for (int i = 0; i < n_hit; i++) {
      hit = (*hits_collection)[i];
      
      SortEdepByParticle(hit->GetParticlePDG(), hit->GetEdep());

      if (i == 0) {                     // First hit - get particle
        particle_code_ = hit -> GetParticlePDG();
      }

    } // End for loop

    // Apply the blurring BEFORE comparing to threshold
    if (apply_finite_resolution_) ApplyResolution();

    G4cout << "E: " << energy_dep_total_/keV << " keV...";
    if (energy_dep_total_ > threshold_ && n_hit > 0) {
      G4cout << "Getting hit time";
      K37ScintillatorHit *first_hit = (*hits_collection)[0];
      hit_time_ = first_hit -> GetTime();
      //      G4cout << "Hit time should be " << first_hit->GetTime()/ns  << " ns " << G4endl;
    }
    G4cout << G4endl;
  }

}

void K37ScintillatorDigitizer::InitializeData() {
  //  G4cout << "K37ScintillatorDigitizer::InitializeData()" << G4endl;
  energy_dep_total_ = 0.0;
  energy_dep_electron_ = 0.0;
  energy_dep_positron_ = 0.0;
  energy_dep_gamma_ = 0.0;
  energy_dep_other_ = 0.0;
  particle_code_ = 0;
  hit_time_ = 0.0;
}

void K37ScintillatorDigitizer::ApplyResolution() {
  // This function "blurs" the true energy by an amount related to the
  // measured detector resolution.  It overwrites the energy_dep_*
  // variables.  After this function, information about the true
  // energy of the event is lost

  // Resolution of detector at given energy is equal to...
  // res^2 = sigma0^2 + mu*Energy
  G4double res_total = sqrt(pow(resolution_sigma0, 2.0) +
                            (resolution_mu*energy_dep_total_));
  G4cout << "E = " << energy_dep_total_/keV << " keV -----> ";
  if (energy_dep_total_ > 0.0) {
    energy_dep_total_ = G4RandGauss::shoot(energy_dep_total_, res_total);
  }
  G4cout << energy_dep_total_/keV << " keV" << G4endl;

  // The energy deposited by each particle will not be blurred because
  // there is no way to compare it to data anyways
}

void K37ScintillatorDigitizer::SortEdepByParticle(const G4int &pdgCode_,
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
