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
#include "K37AllPossibleEventInformation.hh"

K37SteppingAction::K37SteppingAction(K37ListOfVolumeNames* list,
                                     K37AllPossibleEventInformation* APEI)
    : listOfEnteredVolumes(list), AllTheInformation(APEI) {
}

K37SteppingAction::~K37SteppingAction() {}

void K37SteppingAction::UserSteppingAction(const G4Step * theStep) {
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

  if (AllTheInformation -> getShouldEventInformationBeRecorded()) {
    if (theParticleName == "e-" &&
        theTrack -> GetTrackStatus() != fStopAndKill) {
      G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
      G4String thePostPVname = thePostPoint->GetPhysicalVolume()->GetName();
      if (thePostPoint -> GetStepStatus() == fGeomBoundary &&
          thePostPVname == "SOED_phys") {
        AllTheInformation ->
            setEnteringShakeOffElectronDetector(theTrack -> GetPosition());
        AllTheInformation ->
            setEnteringShakeOffElectronDetectorTime(theTrack ->
                                                    GetGlobalTime());
      }
    }

    if (theParticleName == "e+" &&
        theTrack -> GetTrackStatus() != fStopAndKill) {
      G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
      G4String thePostPVname = thePostPoint->GetPhysicalVolume()->GetName();

      if (thePostPoint -> GetStepStatus() == fGeomBoundary &&
          (thePostPVname == "dedx_plusZ_phys" ||
           thePostPVname == "dedx_minusZ_phys")) {
        AllTheInformation -> setEnteringStripDetector(1,
                                                      theTrack->GetPosition());
      }

      if (thePostPoint -> GetStepStatus() == fGeomBoundary &&
          (thePostPVname == "scint_plusZ_phys" ||
           thePostPVname == "scint_minusZ_phys")) {
        AllTheInformation->setEnteringScintillator(1, theTrack->GetPosition());
      }
    }
    if (theTrack -> GetParentID() == 0 &&
        theTrack -> GetTrackStatus() != fStopAndKill) {
      G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
      G4String thePostPVname = thePostPoint->GetPhysicalVolume()->GetName();

      if (thePostPoint -> GetStepStatus() == fGeomBoundary &&
          (thePostPVname == "dedx_plusZ_phys" ||
           thePostPVname == "dedx_minusZ_phys")) {
        AllTheInformation ->
            setEnteringStripDetector(0, theTrack -> GetPosition());
      }

      if (thePostPoint -> GetStepStatus() == fGeomBoundary &&
          (thePostPVname == "scint_plusZ_phys" ||
           thePostPVname == "scint_minusZ_phys")) {
        AllTheInformation->setEnteringScintillator(0, theTrack->GetPosition());
      }
    }
  }
  // if (thePrePoint->GetStepStatus() == fGeomBoundary)

  // if (thePostPoint->GetPhysicalVolume()->GetName() == "hoop_phys")
  //    {
  // G4cout<< "\twhoop" << G4endl;
  //            eventAction->setScatterOffHoopFlag();
  // }


  /*
    if (theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "chamber_phys")
    {
    G4cout << "whoop!" << G4endl;

    }
  */
  /*
    G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
    G4VPhysicalVolume * thePostPV = thePostPoint->GetPhysicalVolume();
    G4String thePostPVname = thePostPV->GetName();

    G4cout<< thePostPVname << G4endl;
  */
  // hoop_phys

  // check if it is alive
  /*
    if(theTrack->GetTrackStatus()==fStopAndKill)
    {
    if((theTrack->GetPosition().x())/mm < -100 )
    {
    //G4cout << (theTrack->GetPosition().x())/mm <<G4endl;
    ofstream zpos;
    zpos.open ("alive.txt", ofstream::out | ofstream::app);
    zpos.setf( std::ios::fixed, std::ios::floatfield );
    zpos<< (theStep->GetPostStepPoint()->GetPosition().x())/mm << G4endl;
    zpos.close();

    eventAction->BackscatterFlag();
    }

    }
  */

  // return;



  /*
    if (theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "annihil")
    {

    //G4cout<< (theTrack->GetPosition().x())/cm<< " "<<(theTrack->GetPosition().y())/cm<< " "<<(theTrack->GetPosition().z())/cm<<G4endl;

    ofstream annihilate;
    annihilate.open ("annihilate.txt", ofstream::out | ofstream::app);
    annihilate.setf( std::ios::fixed, std::ios::floatfield );

    annihilate<< (theTrack->GetPosition().x())/cm << "\t"
    << (theTrack->GetPosition().y())/cm << "\t"
    << (theTrack->GetPosition().z())/cm <<G4endl;

    annihilate.close();

    }
  */

  // check if it is primary

  /*if(theTrack->GetParentID()!=0) { return; }

  // check if it is NOT muon
  G4ParticleDefinition * particleType = theTrack->GetDefinition();
  if((particleType==G4MuonPlus::MuonPlusDefinition())
  ||(particleType==G4MuonMinus::MuonMinusDefinition()))
  { return; }

  // check if it is entering to the calorimeter volume
  G4StepPoint * thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume * thePrePV = thePrePoint->GetPhysicalVolume();
  G4String thePrePVname = thePrePV->GetName();
  if(thePrePVname(0,4)=="calo") { return; }
  G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume * thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePostPVname = thePostPV->GetName();
  if(thePostPVname(0,4)!="calo") { return; }

  // then suspend the track
  theTrack->SetTrackStatus(fSuspend);
  */
}
