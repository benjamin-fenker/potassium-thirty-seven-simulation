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

//#include "K37EventAction.hh"
#include "K37SteppingAction.hh"
#include "K37SteppingActionMessenger.hh"

K37SteppingAction::K37SteppingAction()
:stepping_messenger(new K37SteppingActionMessenger(this)),
   ToTrack(false)
{;}

K37SteppingAction::~K37SteppingAction()
{
   delete stepping_messenger;
}

void K37SteppingAction::UserSteppingAction(const G4Step * theStep)
{

   if (ToTrack)
   {
      G4Track * theTrack = theStep->GetTrack();
      const G4ParticleDefinition* theDefinition =
         theTrack->GetParticleDefinition();

      G4String theParticleName = theDefinition->GetParticleName();
      if ((theParticleName == "e+") 
            && (theTrack -> GetTrackStatus() != fStopAndKill))
      {
         G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
         G4String thePostPVname = thePostPoint->GetPhysicalVolume()->GetName();

         if ((thePostPoint -> GetStepStatus() == fGeomBoundary)
               && (thePostPVname != "world_phys"))
         {
            (*active_channels)["VOLUME_LIST"] -> InsertData(
                  static_cast<std::string>(thePostPVname));
         }
      }
   }
}
