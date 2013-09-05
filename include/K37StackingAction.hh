// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37StackingAction_h
#define K37StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

#include "K37ScintillatorHit.hh"
#include "K37StackingActionMessenger.hh"
#include "K37StripDetectorHit.hh"

class G4Track;
class K37StackingActionMessenger;

// -----------------------------------------------

class K37StackingAction : public G4UserStackingAction {
 public:
  K37StackingAction();
  ~K37StackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
  void NewStage();
  void PrepareNewEvent();
  void SetTrackSecondaries(G4bool track) {track_secondaries_ = track;}

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

  K37StackingActionMessenger *messenger_;
  G4bool track_secondaries_;
};

#endif
