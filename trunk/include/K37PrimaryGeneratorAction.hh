// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37PrimaryGeneratorAction_h
#define K37PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"

class G4VPrimaryGenerator;
class G4Event;
class G4SingleParticleSource;
class K37DetectorConstruction;
class K37PrimaryGeneratorMessenger;
class K37AllPossibleEventInformation;
class G4ParticleDefinition;
class G4PrimaryVertex;
class G4DecayTable;
class G4PhaseSpaceDecayChannel;
class K37CloudSize;
class K37EventGenerator;

class K37PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  K37PrimaryGeneratorAction(K37DetectorConstruction*,
                            K37AllPossibleEventInformation*,
                            K37EventGenerator*);
  ~K37PrimaryGeneratorAction();

 public:
  void GeneratePrimaries(G4Event* anEvent);
  // G4ParticleGun* GetParticleGun() {return particleGun;};
  G4double getVelocity(G4double kineticEnergy,
                       G4double massOfParticle = 0.510998);
  void setRandomFlag(G4String val) {
    randomFlag = val;
  }
  void setBetaVertex();
  void setDaughterVertex();

 private:
  G4double      distanceToTrap;
  // G4ParticleGun*      particleGun;
  G4SingleParticleSource*       particleGun;
  G4double      rho;
  G4double      phi;
  G4double      theta;
  G4double      insideCollimator;
  G4ParticleDefinition* electron;
  G4ParticleDefinition* positron;
  G4ParticleDefinition* Ar37MinusParticle;
  G4ParticleDefinition* ion;

  K37DetectorConstruction* detector;
  // K37EventMaker*     evMaker;
  G4String      randomFlag;  // flag for a random momentum direction
  K37PrimaryGeneratorMessenger* gunMessenger;   // messenger of this class
  // Container that gathers all of the event information from the most
  // convient sources.
  K37AllPossibleEventInformation* EventInformation;
  G4ThreeVector v;
  G4PrimaryVertex* vertex;
  G4ThreeVector EventVertex;
  G4ParticleDefinition *K37Neutral;
  G4ParticleDefinition *K37Minus;
  G4DecayTable *decayTableAr37Minus;
  G4PhaseSpaceDecayChannel *K37MinusDecayMode;
  K37CloudSize *cloud;
  K37EventGenerator *evGenerator;
};

#endif


