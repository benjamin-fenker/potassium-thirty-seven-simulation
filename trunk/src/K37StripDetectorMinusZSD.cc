

#include "K37StripDetectorMinusZSD.hh"
#include "K37StripDetectorMinusZHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

K37StripDetectorMinusZSD::K37StripDetectorMinusZSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="dedx2Collection");
}

K37StripDetectorMinusZSD::~K37StripDetectorMinusZSD()
{;}

void K37StripDetectorMinusZSD::Initialize(G4HCofThisEvent* HCE)
{
  static int HCID = -1;
  dedx2Collection = new K37StripDetectorMinusZHitsCollection (SensitiveDetectorName,collectionName[0]); 
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCE->AddHitsCollection(HCID,dedx2Collection);

}



G4bool K37StripDetectorMinusZSD::ProcessHits(G4Step*aStep,G4TouchableHistory* )
{
	K37StripDetectorMinusZHit* newHit = new K37StripDetectorMinusZHit();
  	
	G4Track * theTrack = aStep->GetTrack();
	
	if (theTrack->GetParentID()!=0)
		{
			newHit->SetPrimary(false);
		}
	else
		{
			newHit->SetPrimary(true);
		}
	
	G4double edep = aStep->GetTotalEnergyDeposit();
	//pos	if(edep==0.) return false;


	newHit->SetEdep( edep );
  
	newHit->SetPos1( aStep->GetPreStepPoint()->GetPosition() );
	newHit->SetPos2( aStep->GetPostStepPoint()->GetPosition() );
	newHit->SetTime( aStep->GetTrack()->GetGlobalTime() );
  
	if (aStep->GetTrack()->GetParentID() == 0 &&  aStep->GetPreStepPoint()-> GetStepStatus() == fGeomBoundary)
		{newHit->SetBound(2.);}
	else
		{newHit->SetBound(1.);}
  
	dedx2Collection->insert( newHit );

	return true;
}

void K37StripDetectorMinusZSD::EndOfEvent(G4HCofThisEvent*HCE)
{;}

void K37StripDetectorMinusZSD::clear()
{;} 

void K37StripDetectorMinusZSD::DrawAll()
{;} 

void K37StripDetectorMinusZSD::PrintAll()
{;} 

