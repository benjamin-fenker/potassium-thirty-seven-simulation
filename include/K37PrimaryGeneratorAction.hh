// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37PrimaryGeneratorAction_h
#define K37PrimaryGeneratorAction_h 1

#include <map>
#include <string>
#include <vector>

#include "Aggregator.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4RandomDirection.hh"
#include "K37_Data.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4VPrimaryGenerator;
class G4Event;
class G4SingleParticleSource;
class K37DetectorConstruction;
class K37PrimaryGeneratorMessenger;
class G4ParticleDefinition;
class G4PrimaryVertex;
class G4DecayTable;
class G4PhaseSpaceDecayChannel;
class K37CloudSize;
class K37EventGenerator;
class K37EventGeneratorNoRecoilOrderEffects;

using AGG::Aggregator;
using K37_ABC::K37_Data;
using std::map;
using std::string;
using std::vector;

class K37PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  K37PrimaryGeneratorAction(K37DetectorConstruction*,
                            K37EventGenerator*, 
                            K37EventGeneratorNoRecoilOrderEffects*);
  ~K37PrimaryGeneratorAction();

 public:
  void GeneratePrimaries(G4Event* anEvent);
  // G4ParticleGun* GetParticleGun() {return particleGun;};
  G4double getVelocity(G4double kineticEnergy,
                       G4double massOfParticle = 0.510998);
  void SetRecoil(G4String val) {use_recoil_ = val;}
  void setBetaVertex();
  void setDaughterVertex(G4double recoil_charge);
  void SetSOelectronVertices(G4Event *ev, G4int num_so_electron);
  void SetPhotoionizationVertices(G4Event *ev);
  void SetGammaVertex(G4Event *ev);
  void SetPolarization(G4double pol);
  G4double GetPolarization() {return polarization_;}
  void SetAlignment(G4double ali);
  G4double GetAlignment() {return alignment_;}
  void SetRecoilCharge(G4int charge);
  G4int GetRecoilCharge() {return recoil_charge_;}
  K37CloudSize* GetCloudSize() {return cloud;}
  void SetAggregator(Aggregator *aggregator) {the_aggregator_ = aggregator;}
  void SetActiveChannels(map<string, K37_Data*> *active_channels) {
    active_channels_ = active_channels;
  }
  void SetMakeBeta(G4bool flag) {make_beta_ = flag;}
  void SetMakeRecoil(G4bool flag) {make_recoil_ = flag;}
  void SetMakeShakeoffElectrons(G4bool flag) {make_shakeoff_electrons_ = flag;}
  K37EventGenerator* GetEventGenerator() {return evGenerator;}
  void ToggleUseGPS();
  void SetUseGPS(bool b);
 private:
  bool use_gps;
  void NormalizeChargeStateRatio();
  void NormalizeBranchingRatio();
  G4bool TwoPercentEvent();
  G4double      distanceToTrap;

  //  G4SingleParticleSource*       particleGun;
  G4VPrimaryGenerator *particleGun;
  G4double      rho;
  //  G4double      phi;
  //  G4double      theta;
  G4double      insideCollimator;
  G4ParticleDefinition* electron;
  G4ParticleDefinition* positron;
  G4ParticleDefinition* gamma;
  G4ParticleDefinition* Ar37MinusParticle;

  G4double polarization_;
  G4double alignment_;
  G4double recoil_charge_;               // Also determines number of SOE
  // Ratios of recoil charge distribution...
  vector<G4double> charge_state_ratio_;      // Ar0 -> Ar+7
  vector<G4double> branching_ratio; //Just contains mainbrnach and 2% now. 
                         //The other branches enter in at the 0.04% level. 

  G4String use_recoil_;
  K37DetectorConstruction* detector;
  // K37EventMaker*     evMaker;
  K37PrimaryGeneratorMessenger* gunMessenger;   // messenger of this class
  // Container that gathers all of the event information from the most
  // convient sources.
  G4ThreeVector v;
  G4PrimaryVertex* vertex;
  G4ThreeVector EventVertex;
  G4ParticleDefinition *K37Neutral;
  G4ParticleDefinition *K37Minus;
  G4DecayTable *decayTableAr37Minus;
  G4PhaseSpaceDecayChannel *K37MinusDecayMode;
  K37CloudSize *cloud;
  K37EventGenerator *evGenerator;
  K37EventGeneratorNoRecoilOrderEffects *twoPercent;

  G4double GetChargeStateThisEvent();
  Aggregator *the_aggregator_;
  map<string, K37_Data*> *active_channels_;
  G4ThreeVector GetMomentumIsotropic(G4double kinetic_energy, G4double mass);
  G4bool make_beta_;
  G4bool make_recoil_;
  G4bool make_shakeoff_electrons_;
  G4bool makeTwoPercent;
  G4bool thisEventIsATwoPercent;
};

#endif


