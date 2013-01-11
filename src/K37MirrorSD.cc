// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37MirrorSD.hh"
#include "K37MirrorHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

K37MirrorSD::K37MirrorSD(G4String name)
  :G4VSensitiveDetector(name) {
  G4String HCname;
  collectionName.insert(HCname="mirrorCollection");
}

K37MirrorSD::~K37MirrorSD() {
}

void K37MirrorSD::Initialize(G4HCofThisEvent* HCE) {
  static int HCID = -1;
  mirrorCollection = new K37MirrorHitsCollection(SensitiveDetectorName,
                                                 collectionName[0]);
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, mirrorCollection);
}



G4bool K37MirrorSD::ProcessHits(G4Step*aStep, G4TouchableHistory* ) {
  G4double edep = aStep->GetTotalEnergyDeposit();
  // pos       if(edep==0.) return false;

  K37MirrorHit* newHit = new K37MirrorHit();
  newHit->SetEdep(edep);

  newHit->SetPos1(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetPos2(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetTime(aStep->GetTrack()->GetGlobalTime());

  if (aStep->GetTrack()->GetParentID() == 0) {
    newHit->SetBound(2.);
  } else {
    newHit->SetBound(1.);
  }

  mirrorCollection->insert(newHit);

  return true;
}

void K37MirrorSD::clear() {
}

void K37MirrorSD::DrawAll() {
}

void K37MirrorSD::PrintAll() {
}

