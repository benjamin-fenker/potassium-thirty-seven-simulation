// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4Trajectory.hh"

void K37TrackingAction::PreUserTrackingAction(const G4Track* aTrack) {
  G4String name = aTrack -> GetDefinition() -> GetParticleName();
  // G4cout << "Name is " << name << G4endl;
  G4Trajectory *trajectory = new G4Trajectory(aTrack);

  // G4cout << "Adding trajectory with particle "
  //        << trajectory -> GetParticleName() << G4endl;


  fpTrackingManager->SetStoreTrajectory(true);
  fpTrackingManager->SetTrajectory(trajectory);
}


