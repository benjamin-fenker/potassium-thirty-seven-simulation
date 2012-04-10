
#include "K37StackingAction.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "K37ArMinusDefinition.hh"
#include "K37ArNeutralDefinition.hh"

//-----------------------------------------------

K37StackingAction::K37StackingAction()
:scintillatorPlusZHits(0), scintillatorMinusZHits(0), stripDetectorPlusZHits(0), stripDetectorMinusZHits(0)
{}

//-----------------------------------------------

K37StackingAction::~K37StackingAction()
{}

//-----------------------------------------------

G4ClassificationOfNewTrack K37StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ClassificationOfNewTrack  classification = fWaiting;
  
  // kill all secondaries
  //if(aTrack->GetParentID() != 0) classification = fKill;
  
	G4ParticleDefinition * particleType = aTrack->GetDefinition();

	if((particleType==G4Electron::ElectronDefinition())||(particleType==G4Positron::PositronDefinition())||(particleType==G4Gamma::GammaDefinition()))
	{
		classification = fUrgent;
	}
	if(particleType==K37Ar37Minus::Ar37MinusDefinition())
	{
		classification = fWaiting;
	}
	if(particleType==K37Ar37Neutral::Ar37NeutralDefinition())
	{
		classification = fKill;
	}

  return classification;
}

//-----------------------------------------------

G4VHitsCollection* K37StackingAction::GetCollection(G4String colName)
{
  //G4cout << "In K37StackingAction::GetCollection(" << colName << ")" << G4endl;

  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  G4RunManager* runMan = G4RunManager::GetRunManager();

  int colID = SDMan->GetCollectionID(colName);
  //G4cout << "SDMan->GetCollectionID(" << colName << ") returned " << colID << G4endl;

  if(colID>=0)
  {
    const G4Event* currentEvent = runMan->GetCurrentEvent();
    G4HCofThisEvent* HCE = currentEvent->GetHCofThisEvent();
    //G4cout << "HCE has " << HCE->GetNumberOfCollections() << " collections" << G4endl;
    //G4cout << "Passed colID>=0 and about to return:  " << HCE->GetHC(colID) << G4endl;
    return HCE->GetHC(colID);
  }
  return 0;
}

//-----------------------------------------------
G4bool K37StackingAction::GetPointersToHitCollections()
{
  //G4cout << "In K37StackingAction::GetPointersToHitsCollections()" << G4endl;
  if(!scintillatorPlusZHits )
    {
      scintillatorPlusZHits = (K37ScintillatorHitsCollection*)GetCollection("scintillatorPlusZHC");
      //G4cout << "I'm here, just assigned scintillatorPlusZHits!" << G4endl;
    }
  if(!scintillatorPlusZHits )
    {
      G4cerr << "SCINTILLATOR PLUS Z HITS COLLECTION NOT FOUND" << G4endl;
      return false;
    }
  else 
    {	 
      //G4cout << "It worked (plus Z Hits)!" << G4endl;
    }
  if(!scintillatorMinusZHits)
    {
      scintillatorMinusZHits= (K37ScintillatorHitsCollection*)GetCollection("scintillatorMinusZHC");
      //G4cout << "I'm here, just assigned scintillatorMinusZHits" << G4endl;
    }
  if(!scintillatorMinusZHits)
    {
      G4cerr << "SCINTILLATOR MINUS Z HITS COLLECTION NOT FOUND" << G4endl;
      return false;
    } else 
    {
      //G4cout << "It worked (minus Z Hits)!" << G4endl;
    }
  if(!stripDetectorMinusZHits)
    {
      stripDetectorMinusZHits= (K37StripDetectorHitsCollection*)GetCollection("dsssdMinusZHC");
    }
  if(!stripDetectorMinusZHits)
    {
      G4cerr << "STRIP DETECTOR MINUS Z HITS COLLECTION NOT FOUND" << G4endl;
      return false;
    }
  if(!stripDetectorPlusZHits)
    {
      stripDetectorPlusZHits= (K37StripDetectorHitsCollection*)GetCollection("dsssdPlusZHC");
    }
  if(!stripDetectorPlusZHits)
    {
      G4cerr << "STRIP DETECTOR PLUS Z HITS COLLECTION NOT FOUND" << G4endl;
      return false;
    }
  return true;
}

void K37StackingAction::NewStage()
{
  //G4cout << "In K37StackingAction::NewStage()" << G4endl;
  // Stage 0->1: Check to see if there are hits in a telescope
  // 		 If there are then the Ar- will be released. 
  if(!this->GetPointersToHitCollections()){return;}

  //G4cout<<"Number of hits -Z SD: "<<stripDetectorMinusZHits->entries()<<G4endl;
  //G4cout<<"Number of hits -Z Sc: "<<scintillatorMinusZHits->entries()<<G4endl;
  //G4cout<<"Number of hits +Z SD: "<<stripDetectorPlusZHits->entries()<<G4endl;
  //G4cout<<"Number of hits +Z Sc: "<<scintillatorPlusZHits->entries()<<G4endl;

  if(((stripDetectorMinusZHits->entries()>0) &&
     (scintillatorMinusZHits->entries()>0)) ||
     ((stripDetectorPlusZHits->entries()>0) &&
     (scintillatorPlusZHits->entries()>0)))
  {
	return;
  }

  stackManager->clear();

}

//-----------------------------------------------

void K37StackingAction::PrepareNewEvent()
{
	scintillatorPlusZHits= NULL;
	scintillatorMinusZHits= NULL;
	stripDetectorPlusZHits= NULL;
	stripDetectorMinusZHits= NULL;
}
