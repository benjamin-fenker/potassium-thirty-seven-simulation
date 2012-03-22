

#include "K37ScintillatorMinusZSD.hh"
#include "K37ScintillatorMinusZHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

K37ScintillatorMinusZSD::K37ScintillatorMinusZSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="fullenergy2Collection");
}

K37ScintillatorMinusZSD::~K37ScintillatorMinusZSD(){;}

void K37ScintillatorMinusZSD::Initialize(G4HCofThisEvent* HCE)
{
  static int HCID = -1;
  fullenergy2Collection = new K37ScintillatorMinusZHitsCollection (SensitiveDetectorName,collectionName[0]); 
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCE->AddHitsCollection(HCID,fullenergy2Collection);
}

G4bool K37ScintillatorMinusZSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	K37ScintillatorMinusZHit* newHit = new K37ScintillatorMinusZHit();
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
  
  fullenergy2Collection->insert( newHit );

  return true;
}

void K37ScintillatorMinusZSD::EndOfEvent(G4HCofThisEvent*)
{;}

void K37ScintillatorMinusZSD::clear()
{;} 

void K37ScintillatorMinusZSD::DrawAll()
{;} 

void K37ScintillatorMinusZSD::PrintAll()
{;} 
