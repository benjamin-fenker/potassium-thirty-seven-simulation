// Authors: Spencer Behling and Benjamin Fenker 2013

#include <math.h>

#include <iomanip>
#include <cmath>

#include "K37EventAction.hh"

#include "K37HistogramManager.hh"
#include "K37ScintillatorHit.hh"
#include "K37MirrorHit.hh"
#include "K37RunAction.hh"
#include "K37ListOfVolumeNames.hh"
#include "K37AllPossibleEventInformation.hh"
#include "K37ContainerForStripInformation.hh"
#include "K37AnalysisNumbering.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4Electron.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "K37Analysis.hh"

using std::pow;
using std::ofstream;
using std::setw;
using std::left;

extern G4bool fillAllSDData;

K37EventAction::K37EventAction(K37RunAction* run, K37ListOfVolumeNames* list,
                               K37AllPossibleEventInformation* APEI,
                               K37HistogramManager * his) :
  runAct(run), listOfEnteredVolumes(list), stripHandler(0),
  EventInformation(APEI), histograms(his) {
  stripHandler = new K37ContainerForStripInformation(EventInformation);

  spot.clear();
  start.clear();

  fullenergy1CollID = -1;
  fullenergy2CollID = -1;
  dedx1CollID = -1;
  dedx2CollID = -1;
  // mirrorCollID = -1;
  // Mount2ID = -1;

  // BTdedxID= -1;
  // BTscintillatorID= -1;

  energy_upper_scintillator = 0;
  energy_upper_scintillator_primaries = 0;
  energy_upper_scintillator_secondaries = 0;

  energy_lower_scintillator = 0;
  energy_lower_scintillator_primaries = 0;
  energy_lower_scintillator_secondaries = 0;

  time_upper_scintillator = 0.0;
  time_lower_scintillator = 0.0;

  energyDedx = 0;
  energyDedx_Primaries = 0;
  energyDedx_Secondaries = 0;

  energyDedx2 = 0;
  energyDedx2_Primaries = 0;
  energyDedx2_Secondaries = 0;

  // energyMount2=0;
  // energyBTdedx=0;
  // energyBTscintillator=0;

  // energyMirror=0;

  accepted = 0;
  interesting = 0;
  bs_flag = 0;

  X = 0;
  Y = 0;
  Z = 0;
  Radius = 0;

  isThereEnergySiliPrimary = false;
  isThereEnergySiliSecondary = false;
  isThereEnergySili = false;
  isThereEnergySili2Primary = false;
  isThereEnergySili2Secondary = false;
  isThereEnergySili2 = false;

  isThereEnergyDedxPrimary = false;
  isThereEnergyDedxSecondary = false;
  isThereEnergyDedx = false;
  isThereEnergyDedx2Primary = false;
  isThereEnergyDedx2Secondary = false;
  isThereEnergyDedx2 = false;

  VetoedEvent = false;
  primaryScatteredOffHoops = false;

  // vector< double > spot;
}

K37EventAction::~K37EventAction() {
  delete stripHandler;
}

void K37EventAction::BeginOfEventAction(const G4Event*) {
  // spot.clear();
  if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
    sizeOfListOfEnteredVolumes = listOfEnteredVolumes->checkSizeOfList();
  }

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  // if(fullenergy1CollID<0||dedx1CollID<0 ||mirrorCollID<0)
  if (fullenergy1CollID <0 || dedx1CollID < 0) {
    G4String colNam;
    fullenergy1CollID = SDman->GetCollectionID(colNam="scintillatorPlusZHC");
    fullenergy2CollID = SDman->GetCollectionID(colNam="scintillatorMinusZHC");
    dedx1CollID = SDman->GetCollectionID(colNam="dsssdPlusZHC");
    dedx2CollID = SDman->GetCollectionID(colNam="dsssdMinusZHC");
  }

  energy_upper_scintillator = 0;
  energy_upper_scintillator_primaries = 0;
  energy_upper_scintillator_secondaries = 0;

  energy_lower_scintillator = 0;
  energy_lower_scintillator_primaries = 0;
  energy_lower_scintillator_secondaries = 0;

  time_upper_scintillator = 0.0;
  time_lower_scintillator = 0.0;

  energyDedx = 0;
  energyDedx_Primaries = 0;
  energyDedx_Secondaries = 0;

  energyDedx2 = 0;
  energyDedx2_Primaries = 0;
  energyDedx2_Secondaries = 0;

  // energyMirror=0;

  // energyMount2=0;
  // energyBTdedx=0;
  // energyBTscintillator=0;

  interesting = 0;
  bs_flag = 0;
  isThereEnergySiliPrimary = false;
  isThereEnergySiliSecondary = false;
  isThereEnergySili = false;
  isThereEnergySili2Primary = false;
  isThereEnergySili2Secondary = false;
  isThereEnergySili2 = false;

  isThereEnergyDedxPrimary = false;
  isThereEnergyDedxSecondary = false;
  isThereEnergyDedx = false;
  isThereEnergyDedx2Primary = false;
  isThereEnergyDedx2Secondary = false;
  isThereEnergyDedx2 = false;

  primaryScatteredOffHoops = false;

  VetoedEvent = false;
  X = 0;
  Y = 0;
  Z = 0;
  Radius = 0;
  stripHandler->BeginingOfEventClear();
}

void K37EventAction::EndOfEventAction(const G4Event* evt) {
  G4AnalysisManager* anMan = G4AnalysisManager::Instance();
  if (listOfEnteredVolumes -> getShouldVolumeNamesBeRecorded()) {
    differenceInListSizeFromStartToEnd =
      static_cast<int> (listOfEnteredVolumes->checkSizeOfList() -
                        sizeOfListOfEnteredVolumes);
  }
  // G4cout << "Size of List" <<differenceInListSizeFromStartToEnd<< G4endl;

  if (bs_flag>0) {
    runAct->Add_bs();
  }

  // if(fullenergy1CollID<0||dedx1CollID<0||mirrorCollID<0) return;
  if (fullenergy1CollID <0 || dedx1CollID < 0) return;

  G4HCofThisEvent * hit_collection = evt->GetHCofThisEvent();
  K37StripDetectorHitsCollection* DEDX1HC = 0;
  K37StripDetectorHitsCollection* DEDX2HC = 0;
  K37ScintillatorHitsCollection* upper_scintillator_hit_collection = 0;
  K37ScintillatorHitsCollection* lower_scintillator_hit_collection = 0;

  // K37MirrorHitsCollection* MHC = 0;

  // energyMount2Map = 0;
  // energyBTdedxMap=0;
  // energyBTscintillatorMap=0;

  if (hit_collection) {
    upper_scintillator_hit_collection =
      static_cast<K37ScintillatorHitsCollection*>(hit_collection ->
                                                  GetHC(fullenergy1CollID));
    lower_scintillator_hit_collection =
      static_cast<K37ScintillatorHitsCollection*>(hit_collection ->
                                                  GetHC(fullenergy2CollID));
    DEDX1HC =
      static_cast<K37StripDetectorHitsCollection*>(hit_collection ->
                                                   GetHC(dedx1CollID));
    DEDX2HC =
      static_cast<K37StripDetectorHitsCollection*>(hit_collection ->
                                                   GetHC(dedx2CollID));
    // MHC = (K37MirrorHitsCollection*)(hit_collection->GetHC(mirrorCollID));
  }

  // If FullEnergy1HitCollection - PlusZ
  if (upper_scintillator_hit_collection) {
    int n_hit = upper_scintillator_hit_collection->entries();
    K37ScintillatorHit *hit;
    for (int i = 0; i < n_hit; i++) {
      // Get the hit
      hit = (*upper_scintillator_hit_collection)[i];

      energy_upper_scintillator += hit->GetEdep();
      if (hit->GetPrimary()) {
        energy_upper_scintillator_primaries += hit -> GetEdep();
      } else {
        energy_upper_scintillator_secondaries += hit -> GetEdep();
      }
    }
    if (energy_upper_scintillator>0) {
      time_upper_scintillator = hit -> GetTime();
      isThereEnergySili = true;
      if (energy_upper_scintillator_primaries == 0) {
        EventInformation->setGammaFiredScintillatorPlusZToTrue();
      }
    }
  }
  if (lower_scintillator_hit_collection) {
    int n_hit = lower_scintillator_hit_collection->entries();
    K37ScintillatorHit *hit;
    for (int i = 0; i < n_hit; i++) {
      hit = (*lower_scintillator_hit_collection)[i];
      energy_lower_scintillator += hit -> GetEdep();
      if (hit -> GetPrimary()) {
        energy_lower_scintillator_primaries += hit -> GetEdep();
      } else {
        energy_lower_scintillator_secondaries += hit -> GetEdep();
      }
    }
    if (energy_lower_scintillator>0) {
      time_lower_scintillator = hit -> GetEdep();
      isThereEnergySili2 = true;
      if (energy_lower_scintillator_primaries == 0) {
        EventInformation->setGammaFiredScintillatorMinusZToTrue();
      }
    }
  }

  K37ScintillatorHit *first_hit = 0;
  if (energy_upper_scintillator > 0.0) {
    first_hit = (*upper_scintillator_hit_collection)[0];
    time_upper_scintillator = first_hit -> GetTime();
  }
  if (energy_lower_scintillator > 0.0) {
    first_hit = (*lower_scintillator_hit_collection)[0];
    time_lower_scintillator = first_hit -> GetTime();
  }

  // Total energy stored in all the strips (each detector, each direction)
  // Pretty redundant, but a good check for bugs
  G4double SD_EDep_total_plusZ_X = 0.0;
  G4double SD_EDep_total_plusZ_Y = 0.0;
  G4double SD_EDep_total_minsZ_X = 0.0;
  G4double SD_EDep_total_minsZ_Y = 0.0;

  // Strip-by-strip information for each detector
  vector<G4double> SD_EDep_plusZ_X(40, 0.0), SD_EDep_plusZ_Y(40, 0.0);
  vector<G4double> SD_EDep_minsZ_X(40, 0.0), SD_EDep_minsZ_Y(40, 0.0);
  if (DEDX2HC) {  // Strip Detector Minus Z
    SD_EDep_minsZ_X = GetEDepVectorX(DEDX2HC);
    SD_EDep_minsZ_Y = GetEDepVectorY(DEDX2HC);
    for (int i = 0; i < 40; i++) SD_EDep_total_minsZ_X += SD_EDep_minsZ_X[i];
    for (int i = 0; i < 40; i++) SD_EDep_total_minsZ_Y += SD_EDep_minsZ_Y[i];
    if (fabs(SD_EDep_total_minsZ_X - SD_EDep_total_minsZ_Y) >
        pow(10.0, -3)*keV) {
      G4cout << "ERROR.  X-ENERGY != Y-ENERGY." << G4endl;
    }

    int n_hit = DEDX2HC->entries();
    for (G4int i = 0; i < n_hit; i++) {
      energyDedx2 += (*DEDX2HC)[i]->GetEdep();
      if ((*DEDX2HC)[i]->GetPrimary()) {
        energyDedx2_Primaries +=(*DEDX2HC)[i]->GetEdep();
        // GetPos2() gets 3-vector of post-step point
        stripHandler -> StoreStripInformation((*DEDX2HC)[i] -> GetPos2(),
                                              (*DEDX2HC)[i] -> GetEdep(),
                                              true, false);
      } else {
        energyDedx2_Secondaries +=(*DEDX2HC)[i]->GetEdep();
        stripHandler -> StoreStripInformation((*DEDX2HC)[i] -> GetPos2(),
                                              (*DEDX2HC)[i] -> GetEdep(),
                                              false, false);
      }
    }
    if (energyDedx2>0) {
      if (fabs(energyDedx2 - SD_EDep_total_minsZ_X) > pow(10.0, -3)*keV) {
        G4cout << "ERROR.  SPENCERS'S WAY != MY WAY" << G4endl;
      }

      isThereEnergyDedx2 = true;

      if (energyDedx2_Primaries ==0) {
        EventInformation->setGammaFiredStripDetectorMinusZToTrue();
      }
      // G4cout<< "energyDedx2: " << energyDedx2 << G4endl;
    }
  }
  if (DEDX1HC) {                // Strip detector plus Z
    SD_EDep_plusZ_X = GetEDepVectorX(DEDX1HC);
    SD_EDep_plusZ_Y = GetEDepVectorY(DEDX1HC);
    for (int i = 0; i < 40; i++) SD_EDep_total_plusZ_X += SD_EDep_plusZ_X[i];
    for (int i = 0; i < 40; i++) SD_EDep_total_plusZ_Y += SD_EDep_plusZ_Y[i];
    if (fabs(SD_EDep_total_plusZ_X - SD_EDep_total_plusZ_Y) >
        pow(10.0, -3)*keV) {
      G4cout << "ERROR.  X-ENERGY != Y-ENERGY." << G4endl;
    }
    int n_hit = DEDX1HC->entries();
    for (int i = 0; i < n_hit; i++) {
      energyDedx += (*DEDX1HC)[i]->GetEdep();
      if ((*DEDX1HC)[i]->GetPrimary()) {
        energyDedx_Primaries +=(*DEDX1HC)[i]->GetEdep();
        stripHandler -> StoreStripInformation((*DEDX1HC)[i] -> GetPos2(),
                                              (*DEDX1HC)[i] -> GetEdep(),
                                              true, true);
      } else {
        energyDedx_Secondaries +=(*DEDX1HC)[i]->GetEdep();
        stripHandler -> StoreStripInformation((*DEDX1HC)[i]->GetPos2(),
                                              (*DEDX1HC)[i]->GetEdep(),
                                              false, true);
      }
    }
    if (energyDedx>0) {
      if (fabs(energyDedx - SD_EDep_total_plusZ_X) > pow(10.0, -3)*keV) {
        G4cout << "ERROR.  SPENCERS'S WAY != MY WAY" << G4endl;
      }
      isThereEnergyDedx = true;
      if (energyDedx_Primaries ==0) {
        EventInformation->setGammaFiredStripDetectorPlusZToTrue();
      }
      // G4cout<< "energyDedx: " << energyDedx << G4endl;
    }
  }  // End strip detector plus z

  if ((isThereEnergyDedx == true && isThereEnergySili == true &&
       isThereEnergyDedx2 == false && isThereEnergySili2 == false) ||
      (isThereEnergyDedx == false && isThereEnergySili == false &&
       isThereEnergyDedx2 == true && isThereEnergySili2 == true)) {

    G4Electron *theElectron = G4Electron::ElectronDefinition();
    G4double emass = theElectron -> GetPDGMass();
    accepted++;
    anMan->FillNtupleIColumn(ntup_accepted, 1);
    runAct->SetAccepted();
    EventInformation->setThisIsAnAccepterEvent();
    anMan -> FillNtupleDColumn(ntup_tdc_scint_top,
                               time_upper_scintillator/ns);
    anMan -> FillNtupleDColumn(ntup_tdc_scint_bottom,
                               time_lower_scintillator/ns);
    // runAct->SetAcceptedPrimaryScatteredOffHoops();
    // Note: Dedx means strip detector and SiLi means scintillator
    // Fill all the ntuples with data from the vectors
    if (fillAllSDData) {
      fillSDNtuples(SD_EDep_plusZ_X, ntup_SD_plusZ_X_start);
      fillSDNtuples(SD_EDep_plusZ_Y, ntup_SD_plusZ_Y_start);
      fillSDNtuples(SD_EDep_minsZ_X, ntup_SD_minsZ_X_start);
      fillSDNtuples(SD_EDep_minsZ_Y, ntup_SD_minsZ_Y_start);
    }
    if (isThereEnergyDedx == true) {
      // stripHandler->PrintMaps();
      stripHandler->PassThePlusZDetectors();

      ofstream detectorADA;
      detectorADA.open("detectorADA.txt", ofstream::out | ofstream::app);
      detectorADA.setf(std::ios::fixed, std::ios::floatfield);
      detectorADA << setw(15) << left << energyDedx/keV << setw(15) << left
                  << energy_upper_scintillator/keV << G4endl;
      detectorADA.close();

      histograms->FillNtuple(1, 0, energy_upper_scintillator);
      histograms->FillNtuple(0, 0, energyDedx);
      histograms->AddRowNtuple(1);
      histograms->AddRowNtuple(0);

      //  fill histograms
      anMan->FillH1(hist_ADA_Scintillator, energy_upper_scintillator/keV);
      anMan->FillH1(hist_ADA_StripDetector, energyDedx/keV);

      runAct -> incrementPlusZ_vc(GetRelativisticFactor(emass,
                                                    energy_upper_scintillator));
      // Int-column 1 corresponds to plus or minus Z
      anMan -> FillNtupleIColumn(ntup_sign_z_hit, 1);
      anMan -> FillNtupleDColumn(ntup_v_over_c,
                               GetRelativisticFactor(emass,
                                                 energy_upper_scintillator));

      EventInformation->setTotalEnergyInScintillator(energy_upper_scintillator);
      EventInformation->setTotalEnergyInStripDetector(energyDedx);
      EventInformation->setPlusZDetectorsFiredToTrue();
      EventInformation->setStartStripDetectorTime((*DEDX1HC)[0]->GetTime());
      if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
        listOfEnteredVolumes -> setEnteredVolumeName("----------------------A",
                                                     0, 0);
      }
    }  // End energy in plusZ

    if (isThereEnergyDedx2 == true) {
      stripHandler->PassTheMinusZDetectors();

      ofstream detectorODA;
      detectorODA.open("detectorODA.txt", ofstream::out | ofstream::app);
      detectorODA.setf(std::ios::fixed, std::ios::floatfield);
      detectorODA << setw(15)<< left << energyDedx2/keV << setw(15) << left
                  << energy_lower_scintillator/keV << G4endl;
      detectorODA.close();

      histograms->FillNtuple(3, 0, energy_lower_scintillator);
      histograms->FillNtuple(2, 0, energyDedx2);
      histograms->AddRowNtuple(3);
      histograms->AddRowNtuple(2);

      //  Fill histograms
      anMan -> FillH1(hist_ODA_Scintillator, energy_lower_scintillator/keV);
      anMan -> FillH1(hist_ODA_StripDetector, energyDedx2/keV);
      runAct ->
          incrementMinusZ_vc(GetRelativisticFactor(emass,
                                                   energy_lower_scintillator));
      anMan -> FillNtupleIColumn(ntup_sign_z_hit, -1);
      anMan -> FillNtupleDColumn(ntup_v_over_c,
                               GetRelativisticFactor(emass,
                                                 energy_lower_scintillator));
      // Done filling histograms

      EventInformation ->
          setTotalEnergyInScintillator(energy_lower_scintillator);
      EventInformation->setTotalEnergyInStripDetector(energyDedx2);
      EventInformation->setStartStripDetectorTime((*DEDX2HC)[0]->GetTime());

      if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
        listOfEnteredVolumes -> setEnteredVolumeName("----------------------O",
                                                     0, 0);
      }
    }  // End energy in minusZ
    // G4cout<< start[0].theta()<<G4endl;
    /*
      if(spot.size() >= 1)
      {
      std::ofstream thetaOut;
      thetaOut.open ("theta.txt", std::ofstream::out | std::ofstream::app);
      thetaOut << setw (15) << left << start[0].theta()<< setw (15) << left << spot[0].theta()<< G4endl;
      thetaOut.close();

      //G4cout<<start[0].theta()<< " "<<spot[0].theta()<< G4endl;
      }
    */

    // listOfEnteredVolumes->setEnteredVolumeName("----------------------" );

    // Add a new row here to add a new row for only accpeted events where
    // either there was energy in the plus or minus z detector, but not both!
    anMan -> AddNtupleRow();
  } else {  // End is it a good event
    if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
      listOfEnteredVolumes ->
        deleteEnteriesFromList(differenceInListSizeFromStartToEnd);
    }
  }
  if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
    listOfEnteredVolumes->checkIfPrintIsNeeded(false);
  }
  if ((evt->GetEventID())%50000 == 0) {
    G4cout << ">>> End of Event " << evt->GetEventID() << G4endl;
  }
  spot.clear();
  start.clear();
  if (EventInformation->getShouldEventInformationBeRecorded()) {
    if (EventInformation->getIsThisAnAcceptedEvent()) {
      EventInformation->EndOfEventActions();
      EventInformation->printEventInformation();
    }
    EventInformation->clearEventInformation();
  }
  // PrintEvent(evt);
  //  G4cout << "------------------------------" << G4endl;
  // Add a new row here to add a new row for EVERY EVENT, even events that were
  // not "accepted."
  // anMan -> AddNtupleRow();
}  // End of event action
//----------------
void K37EventAction::setEnteringDedx(G4ThreeVector enteringPosition) {
  spot.push_back(enteringPosition);
}

void K37EventAction::setStartingDirection(G4ThreeVector startingPosition) {
  start.push_back(startingPosition);
}

void K37EventAction::fillSDNtuples(vector<G4double> EDep_strip,
                                   G4int ntuple_number_start) {
  // After filling up the total energy hit-by-hit, write out the event
  // total energy for each strip.
  bool debug = false;
  G4AnalysisManager *anMan = G4AnalysisManager::Instance();
  for (G4int i = 0; i < 40; i++) {
    anMan->FillNtupleDColumn(ntuple_number_start + i,
                             EDep_strip[i]/keV);
    if (EDep_strip[i] > 0 && debug) {
      G4cout << "Strip " << i << " with " << EDep_strip[i]/keV << G4endl;
    }
  }
}

G4double K37EventAction::GetRelativisticFactor(G4double particleMass,
                                               G4double totalE) {
  return sqrt(totalE/(particleMass+totalE));
}

vector<G4double> K37EventAction::GetEDepVector(
                                 K37StripDetectorHitsCollection *collection,
                                 G4int coordinate) {
  // Coordinate == 1 --> x-strips
  // Coordinate == 2 --> y-strips
  // Coordinate == other --> error
  int n_hit = collection -> entries();
  vector<G4double> totalEdep(40, 0.0);
  for (G4int i = 0; i < n_hit; i++) {
    map<G4int, G4double> Edep;
    if (coordinate == 1) {
      Edep = (*collection)[i]->getXStripsEdep();
    } else if (coordinate == 2) {
      Edep = (*collection)[i]->getYStripsEdep();
    } else {
      G4cerr << "ERROR.  ASKING FOR DIMENSION 3 FROM STRIP DETECTOR" << G4endl;
    }

    map<G4int, G4double>::iterator it;
    // This runs every hit (>= 1 hit/event) and gets total energy
    // for the entire event for each strip
    for (G4int i = 1; i <= 40; i++) {
      it = Edep.find(i);
      // If find doesn't return the element, no energy was in that strip
      if (it != Edep.end()) {
        // The local arrays and ntuple numbering is zero-based while strips are
        // numbered starting from 1 in the K37StripDetectorSD class.
        totalEdep[i-1] += Edep.find(i)->second;
      }
    }
  }
  return totalEdep;
}

vector<G4double>K37EventAction::GetEDepVectorX(
                                K37StripDetectorHitsCollection *collection) {
  return GetEDepVector(collection, 1);
}

vector<G4double>K37EventAction::GetEDepVectorY(
                                K37StripDetectorHitsCollection *collection) {
  return GetEDepVector(collection, 2);
}

void K37EventAction::PrintEvent(const G4Event *event) {
  G4TrajectoryContainer *trajectory_container =
      event -> GetTrajectoryContainer();
  if (!trajectory_container) {
    G4cout << "Failed to get trajectory container!" << G4endl;
    return;
  }
  G4int number_vertices = event -> GetNumberOfPrimaryVertex();
  G4int number_trajectories = trajectory_container -> entries();
  G4int charge, parent_id, id;
  G4ParticleDefinition *p_def = 0;
  G4Trajectory *trajectory = 0;
  G4String name;


  G4cout << "************************************" << G4endl;
  G4cout << "Event ID " << event -> GetEventID()  << " has "
         << number_vertices << " primary vertices" << " and "
         << number_trajectories/* << "(" << trajectory_container -> size()
                                  << ") trajectories" */<< G4endl;
  for (G4int i = 0; i < number_vertices; i++) {
    p_def = event -> GetPrimaryVertex(i) -> GetPrimary(0) -> GetG4code();
    name = p_def -> GetParticleName();
    charge = p_def -> GetPDGCharge();
    G4cout << "Vertex " << i << " has primary particle: " << name
           << " (" << charge << ") " << G4endl;
  }
  G4cout << G4endl;
  for (G4int i = 0; i < number_trajectories; i++) {
    trajectory = (G4Trajectory*)((*trajectory_container)[i]);
    // trajectory -> ShowTrajectory();
    name = trajectory -> GetParticleName();
    charge = trajectory -> GetCharge();
    id = trajectory -> GetTrackID();
    parent_id = trajectory -> GetParentID();
    G4cout << "Trajectory " << id << " --- " << name << " (" << charge << ") "
           << "came from PID " << parent_id << G4endl;
  }
  G4cout << "************************************" << G4endl;
  // G4int j;
  // G4cin >> j;
}
