

#include "K37StripDetectorSD.hh"
#include "K37StripDetectorHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

K37StripDetectorSD::K37StripDetectorSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  if( name == "/mydet/dsssdPlusZ") {
    collectionName.insert(HCname="dsssdPlusZHC");
  } else if(name == "/mydet/dsssdMinusZ") {
    collectionName.insert(HCname="dsssdMinusZHC");
  } else {
    G4cout << "K37StripDetectorSD::K37StripDetectorSD(" << name << "):  ";
    G4cout << "Comparison Failed." << G4endl;
  }
  //collectionName.insert(HCname="dedx1Collection");
  X=0.;
  Y=0.;
  Z=0.;
	
}

K37StripDetectorSD::~K37StripDetectorSD()
{;}

void K37StripDetectorSD::Initialize(G4HCofThisEvent* HCE)
{
  static int HCID = -1;
  dedx1Collection = new K37StripDetectorHitsCollection (SensitiveDetectorName,collectionName[0]); 
  //if(HCID<0)
  //{ HCID = GetCollectionID(0); }
  if( SensitiveDetectorName == "dsssdPlusZ") {
    HCID = 2;
  } else if( SensitiveDetectorName == "dsssdMinusZ") {
    HCID = 3;
  }
  HCE->AddHitsCollection(HCID,dedx1Collection);

}



G4bool K37StripDetectorSD::ProcessHits(G4Step*aStep,G4TouchableHistory* )
{
	X=0.;
	Y=0.;
	Z=0.;
	
	K37StripDetectorHit* newHit = new K37StripDetectorHit();
  	
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
	
	//X= aStep->GetPreStepPoint()->GetPosition().x();
	//Y= aStep->GetPreStepPoint()->GetPosition().y();
	//Z= aStep->GetPreStepPoint()->GetPosition().z();
	
	//G4cout << "X: "<< X << " Y: "<< Y << " Z: "<< Z << G4endl;

	
	if (aStep->GetTrack()->GetParentID() == 0 &&  aStep->GetPreStepPoint()-> GetStepStatus() == fGeomBoundary)
	{
		newHit->SetBound(2.);
		
		X= aStep->GetPreStepPoint()->GetPosition().x();
		Y= aStep->GetPreStepPoint()->GetPosition().y();
		Z = X*X+Y*Y ;
		//G4cout<< " Before SD: "<< " X: "<< X <<" Y: " << Y << " X*X+Y*Y: " << (X*X+Y*Y) << " Z: "<< Z << " "<< newHit->GetSoftwareVeto() << G4endl;
		
		if ( Z >= 156.25)
		{
			newHit->SetSoftwareVeto(true);
		//	G4cout<< "SD Data: "<< " X: "<< X/cm <<" Y: " << Y/cm << " X*X+Y*Y: " << (X*X+Y*Y)/(cm*cm)<< " true " << G4endl;
		}
		else if ( Z < 156.25)
		{
			newHit->SetSoftwareVeto(false);
		//	G4cout<< "SD Data: "<< " X: "<< X/cm <<" Y: " << Y/cm << " X*X+Y*Y: " << (X*X+Y*Y)/(cm*cm)<< " false " << G4endl;
		}
		
	}
	else
	{
		newHit->SetBound(1.);
		newHit->SetSoftwareVeto(false);
	}
  
	dedx1Collection->insert( newHit );

	return true;
}

void K37StripDetectorSD::EndOfEvent(G4HCofThisEvent*HCE)
{;}

void K37StripDetectorSD::clear()
{;} 

void K37StripDetectorSD::DrawAll()
{;} 

void K37StripDetectorSD::PrintAll()
{;} 

