

#include "K37ScintillatorSD.hh"
#include "K37ScintillatorHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

K37ScintillatorSD::K37ScintillatorSD(G4String name)
:G4VSensitiveDetector(name)
{
  //G4cout << "K37ScintillatorSD::K37ScintillatorSD(" << name << ")     ";
  G4String HCname;
  //collectionName.insert(HCname="scintillatorMinusZHC");
  if( name == "/mydet/scintillatorPlusZ") {
    collectionName.insert(HCname="scintillatorPlusZHC");
    //G4cout << "setName: scintillatorPlusZHC" << G4endl;

  } else if(name == "/mydet/scintillatorMinusZ") {
    collectionName.insert(HCname="scintillatorMinusZHC");
    //G4cout << "setName: scintillatorMinusZHC" << G4endl;

  } else {
    //G4cout << "K37ScintillatorSD::K37ScintillatorSD(" << name << "):  ";
    G4cout << "Comparison Failed." << G4endl;
  }
    
}

K37ScintillatorSD::~K37ScintillatorSD(){;}

void K37ScintillatorSD::Initialize(G4HCofThisEvent* HCE)
{
  //G4cout << "K37ScintillatorSD::Initialize(G4HCofThisEvent *)" << G4endl;
  static int HCID = -1;
  //G4cout << "SensitiveDetectorName: " << SensitiveDetectorName << G4endl;
 
  //if(HCID<0)
  //  {
  //    HCID = GetCollectionID(0); 
  //  }

  if (SensitiveDetectorName == "scintillatorPlusZ") {
    HCID = 0;
  } else if( SensitiveDetectorName == "scintillatorMinusZ") {
    HCID = 1;
  }
  //G4cout << "HCID is: " << HCID << G4endl;
  //G4cout << "collectionName[0]: " << collectionName[0] << G4endl; 
  fullenergy1Collection = new K37ScintillatorHitsCollection (SensitiveDetectorName,collectionName[0]); 
  //G4cout << "About to HCE" << G4endl;
  HCE->AddHitsCollection(HCID,fullenergy1Collection);
}

G4bool K37ScintillatorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	K37ScintillatorHit* newHit = new K37ScintillatorHit();
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

void K37ScintillatorSD::EndOfEvent(G4HCofThisEvent*)
{;}

void K37ScintillatorSD::clear()
{;} 

void K37ScintillatorSD::DrawAll()
{;} 

void K37ScintillatorSD::PrintAll()
{;} 
