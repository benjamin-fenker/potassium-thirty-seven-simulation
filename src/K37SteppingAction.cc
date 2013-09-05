// Authors: Spencer Behling and Benjamin Fenker 2013

#include <iomanip>
#include <string>

#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "K37EventAction.hh"
#include "K37SteppingAction.hh"
#include "K37ListOfVolumeNames.hh"
#include "K37AnnihilationPosition.hh"

K37SteppingAction::K37SteppingAction(K37ListOfVolumeNames* list)
    : listOfEnteredVolumes(list)
{;}

K37SteppingAction::~K37SteppingAction() {}

void K37SteppingAction::UserSteppingAction(const G4Step * theStep) {
  /* DELETE HERE DELETE HERE */
  // G4ThreeVector pos = theStep -> GetPreStepPoint() -> GetPosition();
  // G4cout << "Particle "
  //        << theStep -> GetTrack() -> GetDefinition() -> GetParticleName()
  //        << "\t Position Pre = " << pos.x()/mm << " mm,  " << pos.y()/mm
  //        << " mm,  " << pos.z()/mm << " mm\t Time = "
  //        << theStep -> GetPreStepPoint() -> GetGlobalTime()/ns << " n "
  //        << G4endl;

  G4Track * theTrack = theStep->GetTrack();
  const G4ParticleDefinition* theDefinition = theTrack->GetParticleDefinition();
  G4String theParticleName = theDefinition->GetParticleName();

  if (listOfEnteredVolumes -> getShouldVolumeNamesBeRecorded()) {
    if (theParticleName == "e+" &&
        theTrack -> GetTrackStatus() != fStopAndKill) {
      G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
      G4String thePostPVname = thePostPoint->GetPhysicalVolume()->GetName();

      if (thePostPoint -> GetStepStatus() == fGeomBoundary &&
          thePostPVname != "world_phys") {
        // G4cout<< " Theata: "<< theTrack->GetPosition().theta()<<G4endl;
        listOfEnteredVolumes ->
            setEnteredVolumeName(thePostPVname,
                                 (theTrack -> GetPosition().theta()),
                                 (theTrack ->
                                  GetVertexMomentumDirection().theta()));
        // G4cout<<"sent stuff to listOfEnteredVolumes"<< G4endl;
      }
    }
  }

}
