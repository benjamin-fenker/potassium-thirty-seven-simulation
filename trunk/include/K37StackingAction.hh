// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37StackingAction_h
#define K37StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "K37ScintillatorHit.hh"
#include "K37StripDetectorHit.hh"
#include "G4ParticleDefinition.hh"

class G4Track;

// -----------------------------------------------

class K37StackingAction : public G4UserStackingAction {
 public:
  K37StackingAction();
  ~K37StackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
  void NewStage();
  void PrepareNewEvent();

 private:
  K37ScintillatorHitsCollection* scintillatorPlusZHits;
  K37ScintillatorHitsCollection* scintillatorMinusZHits;
  K37StripDetectorHitsCollection* stripDetectorPlusZHits;
  K37StripDetectorHitsCollection* stripDetectorMinusZHits;
  G4VHitsCollection* GetCollection(G4String colName);

  G4bool GetPointersToHitCollections();
  G4ParticleDefinition *electron;
  G4ParticleDefinition *positron;
  G4ParticleDefinition *gamma;
  G4ParticleDefinition *ArMinus;
  G4ParticleDefinition *ArNeutral;
  G4ParticleDefinition *ArPlus;
};

#endif
