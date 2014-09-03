// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37StripDetectorSD.hh"
#include "K37StripDetectorHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

using std::pair;

K37StripDetectorSD::K37StripDetectorSD(G4String name)
  :G4VSensitiveDetector(name) {
  G4String HCname;
  if (name == "/mydet/dsssdPlusZ") {
    collectionName.insert(HCname="dsssdPlusZHC");
  } else if (name == "/mydet/dsssdMinusZ") {
    collectionName.insert(HCname="dsssdMinusZHC");
  } else {
    G4cout << "K37StripDetectorSD::K37StripDetectorSD(" << name << "):  ";
    G4cout << "Comparison Failed." << G4endl;
  }
  // collectionName.insert(HCname="dedx1Collection");
  X = 0.;
  Y = 0.;
  Z = 0.;
}

K37StripDetectorSD::~K37StripDetectorSD() {
}

void K37StripDetectorSD::Initialize(G4HCofThisEvent* HCE) {
  //  static int HCID = -1;
  dedx1Collection =
    new K37StripDetectorHitsCollection(SensitiveDetectorName,
                                       collectionName[0]);
  // if(HCID<0)
  // { HCID = GetCollectionID(0); }
  // if (SensitiveDetectorName == "dsssdPlusZ") {
  //   HCID = 2;
  // } else if (SensitiveDetectorName == "dsssdMinusZ") {
  //   HCID = 3;
  // }
  HCE->AddHitsCollection(G4SDManager::GetSDMpointer() ->
                         GetCollectionID(collectionName[0]), dedx1Collection);
}



G4bool K37StripDetectorSD::ProcessHits(G4Step*aStep, G4TouchableHistory*) {
  X = 0.;
  Y = 0.;
  Z = 0.;
  K37StripDetectorHit* newHit = new K37StripDetectorHit();
  G4Track * theTrack = aStep->GetTrack();
  if (theTrack->GetParentID() != 0) {
    newHit->SetPrimary(false);
  } else {
    newHit->SetPrimary(true);
  }
  G4double edep = aStep->GetTotalEnergyDeposit();
  // pos       if(edep==0.) return false;

  newHit->SetEdep(edep);

  newHit->SetPos1(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetPos2(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetTime(theTrack->GetGlobalTime());

  // X= aStep->GetPreStepPoint()->GetPosition().x();
  // Y= aStep->GetPreStepPoint()->GetPosition().y();
  // Z= aStep->GetPreStepPoint()->GetPosition().z();

  // G4cout << "X: "<< X << " Y: "<< Y << " Z: "<< Z << G4endl;

  // My new stuff *****
  G4ThreeVector preStep = aStep -> GetPreStepPoint() -> GetPosition();
  G4ThreeVector postStep = aStep -> GetPostStepPoint() -> GetPosition();
  G4ThreeVector preStepLocal = preStep - placementInWorld;
  G4ThreeVector postStepLocal = postStep - placementInWorld;

  // First get how many strips away from center (sign is direction)
  // Then renumber the strips from 1 starting on the negative side
  G4int stripHitX_pre = floor(preStepLocal.x()/stripWidth);
  stripHitX_pre += (numStrips/2) + 1;
  G4int stripHitX_post = floor(postStepLocal.x()/stripWidth);
  stripHitX_post += (numStrips/2) + 1;

  // Repeat in Y-direction
  G4int stripHitY_pre = floor(preStepLocal.y()/stripWidth);
  stripHitY_pre += (numStrips/2) + 1;
  G4int stripHitY_post = floor(postStepLocal.y()/stripWidth);
  stripHitY_post += (numStrips/2) + 1;

  // X-Strips
  map<G4int, G4double> xMap = MapOfEDep(stripHitX_pre, stripHitX_post,
                                        preStepLocal.x(), postStepLocal.x(),
                                        edep);
  newHit -> setXMap(xMap);
  // Y-Strips
  map<G4int, G4double> yMap = MapOfEDep(stripHitY_pre, stripHitY_post,
                                        preStepLocal.y(), postStepLocal.y(),
                                        edep);
  newHit -> setYMap(yMap);

  // *******SPENCER'S STUFF******************
  // The second condition here requires that this be the first step in the
  // strip detector.
  if (theTrack->GetParentID() == 0 &&
      aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
    newHit->SetBound(2.);

    X= aStep->GetPreStepPoint()->GetPosition().x();
    Y= aStep->GetPreStepPoint()->GetPosition().y();
    Z = X*X+Y*Y;

    if (Z >= 156.25) {
      newHit->SetSoftwareVeto(true);
    } else if (Z < 156.25) {
      newHit->SetSoftwareVeto(false);
    }

  } else {
    newHit->SetBound(1.);
    newHit->SetSoftwareVeto(false);
  }
  // **************END SPENCER'S STUFF
  newHit -> SetParticlePDG(theTrack ->
                           GetParticleDefinition() -> GetPDGEncoding());
  dedx1Collection->insert(newHit);  // Add the hit to the hits collection
  return true;
}

// void K37StripDetectorSD::EndOfEvent(G4HCofThisEvent*HCE) {
// }

void K37StripDetectorSD::clear() {
}

void K37StripDetectorSD::DrawAll() {
}

void K37StripDetectorSD::PrintAll() {
}

void K37StripDetectorSD::SetupParameters(G4ThreeVector pos, G4int num,
                                         G4double width) {
  placementInWorld = pos;
  numStrips = num;
  stripWidth = width;
}

map<G4int, G4double> K37StripDetectorSD::MapOfEDep(G4int preStrip,
                                                   G4int postStrip,
                                                   G4double prePos,
                                                   G4double postPos,
                                                   G4double totalEDep) {
  // *************************************************
  // This function uses a [cute] trick to get the path length in the first and
  // last strip of each event.  It uses floor(pos) or ceiling(pos) to compare
  // position to this location which is at the strip edge.
  // *************************************************

  // The simplest case will be a hit that deposits all of its energy into
  // a single strip.  To check this, make sure x-in strip and x-out strip
  // are the same
  bool debug = false;
  map<G4int, G4double> hitMap;
  if (debug) {
    G4cout << "Strip number:\t E_dep [keV]" << G4endl;
    G4cout << " total\t " << totalEDep/keV << G4endl;
  }
  if (totalEDep > 0.0) {
    if (preStrip == postStrip) {
      // All energy for this hit in one strip
      hitMap.insert(pair<G4int, G4double>(postStrip, totalEDep));
      if (debug) G4cout << " " << postStrip << "\t" << totalEDep/keV << G4endl;
    } else {                // This hit crossed a strip-strip boundary
      G4double totalPathLength = fabs(postPos - prePos);
      if (debug) G4cout << "totalPathLength: " << totalPathLength/mm << G4endl;
      if (preStrip < postStrip) {  // Particle moving to right
        G4double stripPathLength = 0.0;
        for (G4int s = preStrip; s <= postStrip; s++) {
          if (s == preStrip) {  // First strip it enters
            // Gets how far the particle goes in the first x-strip
            // ceil(prePos) gets coordinate of nearest strip edge to the right
            stripPathLength = stripWidth*(ceil(prePos/stripWidth)) - prePos;
            if (debug) {
              G4cout << "first strip\t ";
              G4cout << "length in strip: " << stripPathLength/mm << G4endl;
            }
          } else if (s == postStrip) {  // Last strip in the sequence
            stripPathLength = stripWidth*(floor(postPos/stripWidth)) - postPos;
            if (debug) {
              G4cout << "last strip\t ";
              G4cout << "length in strip: " << stripPathLength/mm << G4endl;
            }
          } else {          // Middle strip in the sequence
            stripPathLength = stripWidth;
            if (debug) {
              G4cout << "middle strip\t ";
              G4cout << "length in strip: " << stripPathLength/mm << G4endl;
            }
          }
          stripPathLength = fabs(stripPathLength);
          G4double stripE = totalEDep*(stripPathLength/totalPathLength);
          hitMap.insert(pair<G4int, G4double>(s, stripE));
          if (debug) G4cout << " " << s << "\t" << stripE/keV << G4endl;
        }      // End for all the strips it hit
      } else {              // Particle moving to the left
        G4double stripPathLength = 0.0;
        for (G4int s = preStrip; s >= postStrip; s--) {
          if (s == preStrip) {  // First strip it enters
            // Gets how far the particle goes in the first x-strip
            stripPathLength = stripWidth*(floor(prePos/stripWidth)) - prePos;
          } else if (s == postStrip) {  // Last strip in the sequence
            stripPathLength = stripWidth*(ceil(postPos/stripWidth)) - postPos;
          } else {          // Middle strip in sequence
            stripPathLength = stripWidth;
          }
          stripPathLength = fabs(stripPathLength);
          G4double stripE = totalEDep*(stripPathLength/totalPathLength);
          hitMap.insert(pair<G4int, G4double>(s, stripE));
          if (debug) G4cout << " " << s << "\t" << stripE/keV << G4endl;
        }  // End for all the strips it hit
      }    // End if particle moving to the left
    }      // End if particle crossed more than one x-strip

    if (debug) {
      // int junk;
      // G4cout << "enter to continue." << G4endl;
      // G4cin >> junk;
      G4cout << " --- " << G4endl;
    }
  }
  return hitMap;
}

