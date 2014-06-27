// Authors: Spencer Behling and Benjamin Fenker 2014

#include <algorithm>
#include <fstream>

#include <G4DigiManager.hh>
#include <G4EventManager.hh>
#include <G4UnitsTable.hh>

#include "K37StripDetectorDigitizer.hh"
#include "K37StripDetectorHit.hh"
#include "K37StripDetectorMessenger.hh"

using std::ifstream;
using std::max;
using std::vector;

K37StripDetectorDigitizer::K37StripDetectorDigitizer(const G4String &name,
                                                     const char *res_name_x,
                                                     const char *res_name_y) :
    K37SortingDigitizer(name),
    energy_dep_Xstrip_(40, 0.0), energy_dep_Ystrip_(40, 0.0), particle_code_(0),
    apply_finite_resolution_(true) {

  resolution_Xstrip_ = GetResolution(res_name_x);
  resolution_Ystrip_ = GetResolution(res_name_y);

  messenger_ = new K37StripDetectorMessenger(this);
}

K37StripDetectorDigitizer::~K37StripDetectorDigitizer() {}

void K37StripDetectorDigitizer::Digitize() {
  InitializeData();                     // Zeroes everything out
  
  G4DigiManager *digitizer_manager(G4DigiManager::GetDMpointer());
  G4int hit_collection_id(digitizer_manager -> GetHitsCollectionID(
      moduleName + "HC"));
  //  G4cout << moduleName << " ---> ID: " << hit_collection_id << G4endl;
  const K37StripDetectorHitsCollection *hits_collection(
      static_cast<const K37StripDetectorHitsCollection*> (
          digitizer_manager -> GetHitsCollection(hit_collection_id)));

  // Process the hits and store the energy deposited
  if (hits_collection) {

    int n_hit = hits_collection->entries();
    K37StripDetectorHit *hit;
    for (int i = 0; i < n_hit; ++i)
    {
      hit = (*hits_collection)[i];
      SortEdepByParticle(hit->GetParticlePDG(), hit->GetEdep());

      if (i == 0) particle_code_ = hit -> GetParticlePDG();
    }

    // Each hit in the hit collection contains a map of strip-number
    // to energy deposited.  Go through all the hits and incremement
    // each strip for each hit to get the total energy in each strip,
    // zero included!
    energy_dep_Xstrip_ = GetEDepVectorX(hits_collection);
    energy_dep_Ystrip_ = GetEDepVectorY(hits_collection);

    // Check if E_X = E_Y
    G4double total_X = 0.0, total_Y = 0.0;
    for (int i = 0; i < 40; i++) {
      total_X += energy_dep_Xstrip_[i];
      total_Y += energy_dep_Ystrip_[i];
    }

    if (fabs(total_X-total_Y) > 0.001*keV) {
      G4cout << "ERROR.  X-ENERGY != Y-ENERGY." << G4endl;
      G4cout << "Energy X: " << G4BestUnit(total_X, "Energy") << G4endl;
      G4cout << "Energy Y: " << G4BestUnit(total_Y, "Energy") << G4endl;
      G4EventManager::GetEventManager()->KeepTheCurrentEvent();
    }
    // if (fabs(total_X - energy_dep_total_) > 0.001 * keV) {
    //   G4cout << "ERROR.  X-ENERGY != Total Energy." << G4endl;
    //   G4cout << "Energy X    : " << G4BestUnit(total_X, "Energy") << G4endl;
    //   G4cout << "Total Energy: " << G4BestUnit(energy_dep_total_, "Energy") << G4endl;
    //   G4EventManager::GetEventManager()->KeepTheCurrentEvent();
    // }
  }
  if (apply_finite_resolution_) ApplyResolution();
}

void K37StripDetectorDigitizer::InitializeData() {
  K37SortingDigitizer::InitializeData();
  for (unsigned int i = 0; i < energy_dep_Xstrip_.size(); i++) {
    energy_dep_Xstrip_[i] = 0.0;
    energy_dep_Ystrip_[i] = 0.0;
  }
}

vector<G4double>K37StripDetectorDigitizer::GetEDepVectorX(
    const K37StripDetectorHitsCollection *collection) {
  return GetEDepVector(collection, 1);
}

vector<G4double>K37StripDetectorDigitizer::GetEDepVectorY(
    const K37StripDetectorHitsCollection *collection) {
  return GetEDepVector(collection, 2);
}

vector<G4double> K37StripDetectorDigitizer::GetEDepVector(
    const K37StripDetectorHitsCollection *collection, G4int coordinate) {
  // Coordinate == 1 --> x-strips
  // Coordinate == 2 --> y-strips
  // Coordinate == other --> error
  int n_hit = collection -> entries();
  vector<G4double> totalEdep(40, 0.0);
  for (G4int i = 0; i < n_hit; i++) {
    map<G4int, G4double> Edep;
    if (coordinate == 1) {
      Edep = (*collection)[i]->getXStripsEdep();
    } else if (coordinate == 2) {
      Edep = (*collection)[i]->getYStripsEdep();
    } else {
      G4cerr << "ERROR.  ASKING FOR DIMENSION 3 FROM STRIP DETECTOR" << G4endl;
      G4EventManager::GetEventManager()->KeepTheCurrentEvent();
    }

    map<G4int, G4double>::iterator it;
    // This runs every hit (>= 1 hit/event) and gets total energy
    // for the entire event for each strip
    for (G4int i = 1; i <= 40; i++) {
      it = Edep.find(i);
      // If find doesn't return the element, no energy was in that strip
      if (it != Edep.end()) {
        // The local arrays and ntuple numbering is zero-based while strips are
        // numbered starting from 1 in the K37StripDetectorSD class.
        totalEdep[i-1] += Edep.find(i)->second;
      }
    }
  }
  return totalEdep;
}

std::vector<G4double> K37StripDetectorDigitizer::GetResolution(
    const char* fname) {
  vector<G4double> res_strip(40, 0.0);
  ifstream file;
  file.open(fname, std::ifstream::in);
  if (file.is_open()) {
    G4String dummy;
    getline(file, dummy);
    G4int s = 0;
    G4int strip;
    G4double res;
    while (s < 40) {
      if (!file.good()) {
        G4cout << "File " << fname << " has too few lines" << G4endl;
        G4cout << "Detector resolution set to infinite." << G4endl;
        file.close();
        return vector<G4double>(40, 0.0); // Breaks out of the loop
      }

      file >> strip >> res;
      //      G4cout << strip << "\t" << res << G4endl;
      if (strip != s+1) {               // Strip numbers start at 1
        G4cout << "File " << fname << " has wrong format" << G4endl;
        G4cout << "Detector resolution set to infinite." << G4endl;
        file.close();
        return vector<G4double>(40, 0.0); // Breaks out of the loop
      }
      
      res_strip[s] = res * keV;
      s++;
      
    }

  } else {
    G4cout << "Could not open file " << fname << G4endl;
    G4cout << "Detector resolution set to infinite." << G4endl;
    file.close();
    return vector<G4double>(40, 0.0);
  }
  file.close();
  return res_strip;
}

void K37StripDetectorDigitizer::SetResolutionX(const char *fname) {
  ifstream file;
  file.open(fname);
  if (file.is_open()) {
    resolution_Xstrip_ = GetResolution(fname);
  } else {
    G4cout << "File " << fname << " does not exist, no action taken."
           << G4endl;
  }
  file.close();
}

void K37StripDetectorDigitizer::SetResolutionY(const char *fname) {
  ifstream file;
  file.open(fname);
  if (file.is_open()) {
    resolution_Ystrip_ = GetResolution(fname);
  } else {
    G4cout << "File " << fname << " does not exist, no action taken."
           << G4endl;
  }
  file.close();
}

void K37StripDetectorDigitizer::ApplyResolution() {
  G4double res;
  for (unsigned int i = 0; i < 40; i++) {
    if (energy_dep_Xstrip_[i] > 0.0) {
      // Blur energy from gaussian centered at true energy
      // Require positive energy, but allow zero energy

      res = sqrt(energy_dep_Xstrip_[i] * resolution_Xstrip_[i]);
      energy_dep_Xstrip_[i] =
          max(G4RandGauss::shoot(energy_dep_Xstrip_[i], res), 0.0);

    }
    if (energy_dep_Ystrip_[i] > 0.0) {
      // Blur energy from gaussian centered at true energy
      // Require positive energy, but allow zero energy

      res = sqrt(energy_dep_Ystrip_[i] * resolution_Ystrip_[i]);
      //      G4cout << moduleName << " strip " << i+1 << " res: " << res/keV << G4endl;
      energy_dep_Ystrip_[i] =
          max(G4RandGauss::shoot(energy_dep_Ystrip_[i], res), 0.0);
    }
  }
}
