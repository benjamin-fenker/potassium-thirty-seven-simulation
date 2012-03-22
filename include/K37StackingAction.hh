
#ifndef K37StackingAction_h
#define K37StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "K37ScintillatorMinusZHit.hh"
#include "K37ScintillatorPlusZHit.hh"
#include "K37StripDetectorMinusZHit.hh"
#include "K37StripDetectorPlusZHit.hh"

class G4Track;

//-----------------------------------------------

class K37StackingAction : public G4UserStackingAction
{
  public:

    K37StackingAction();
   ~K37StackingAction();

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* );
    void NewStage();
    void PrepareNewEvent();

 private:
    K37ScintillatorPlusZHitsCollection* scintillatorPlusZHits;
    K37ScintillatorMinusZHitsCollection* scintillatorMinusZHits;
    K37StripDetectorPlusZHitsCollection* stripDetectorPlusZHits;
    K37StripDetectorMinusZHitsCollection* stripDetectorMinusZHits;
    G4VHitsCollection* GetCollection(G4String colName);

    G4bool GetPointersToHitCollections();
    
};

#endif
