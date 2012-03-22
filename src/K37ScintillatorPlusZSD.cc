

#include "K37ScintillatorPlusZSD.hh"
#include "K37ScintillatorPlusZHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

K37ScintillatorPlusZSD::K37ScintillatorPlusZSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="fullenergy1Collection");
}

K37ScintillatorPlusZSD::~K37ScintillatorPlusZSD(){;}

void K37ScintillatorPlusZSD::Initialize(G4HCofThisEvent* HCE)
{
  static int HCID = -1;
  fullenergy1Collection = new K37ScintillatorPlusZHitsCollection (SensitiveDetectorName,collectionName[0]); 
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCE->AddHitsCollection(HCID,fullenergy1Collection);
}

G4bool K37ScintillatorPlusZSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	K37ScintillatorPlusZHit* newHit = new K37ScintillatorPlusZHit();
	G4Track * theTrack = aStep->GetTrack();
	
	if (theTrack->GetParentID()!=0)
		{
			newHit->SetPrimary(false);
		}
	else
		{
			newHit->SetPrimary(true);
		}
	
	// theTrack->SetTrackStatus(fStopAndKill);
	
	G4double edep = aStep->GetTotalEnergyDeposit();
	//if(edep==0.) return false;

  
  newHit->SetEdep( edep );
  
  newHit->SetPos1( aStep->GetPreStepPoint()->GetPosition() );
  newHit->SetPos2( aStep->GetPostStepPoint()->GetPosition() );
  newHit->SetTime( aStep->GetTrack()->GetGlobalTime() );
  
  if (aStep->GetTrack()->GetParentID() == 0 &&  aStep->GetPreStepPoint()-> GetStepStatus() == fGeomBoundary)
  {newHit->SetBound(2.);}
  else
  {newHit->SetBound(1.);}
  
  fullenergy1Collection->insert( newHit );

  return true;
}

void K37ScintillatorPlusZSD::EndOfEvent(G4HCofThisEvent*)
{;}

void K37ScintillatorPlusZSD::clear()
{;} 

void K37ScintillatorPlusZSD::DrawAll()
{;} 

void K37ScintillatorPlusZSD::PrintAll()
{;} 
