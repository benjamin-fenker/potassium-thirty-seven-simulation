// Authors: Spencer Behling and Benjamin Fenker 2013

#include <math.h>

#include <algorithm>
#include <iomanip>
#include <cmath>

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
#include "G4UnitsTable.hh"

#include "K37AllPossibleEventInformation.hh"
#include "K37Analysis.hh"
#include "K37AnalysisNumbering.hh"
#include "K37Config.hh"
#include "K37ContainerForStripInformation.hh"
#include "K37ElectronMCPHit.hh"
#include "K37EventAction.hh"
#include "K37HistogramManager.hh"
#include "K37ListOfVolumeNames.hh"
#include "K37MirrorHit.hh"
#include "K37RecoilMCPHit.hh"
#include "K37RunAction.hh"
#include "K37ScintillatorHit.hh"

using std::min;
using std::pow;
using std::ofstream;
using std::setw;
using std::left;

extern G4bool fillAllSDData;

K37EventAction::K37EventAction(K37RunAction* run, K37ListOfVolumeNames* list,
                               K37AllPossibleEventInformation* APEI,
                               K37HistogramManager * his) :
    v1190_factor_ns(0.09765625), runAct(run), listOfEnteredVolumes(list),
    stripHandler(0),
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
  recoil_mcp_collection_id = -1;
  electron_mcp_collection_id = -1;
  // BTdedxID= -1;
  // BTscintillatorID= -1;

  theElectron = G4Electron::ElectronDefinition();
  emass = theElectron -> GetPDGMass();
  energyDedx = 0;
  energyDedx_Primaries = 0;
  energyDedx_Secondaries = 0;

  energyDedx2 = 0;
  energyDedx2_Primaries = 0;
  energyDedx2_Secondaries = 0;

  // energyMount2=0;
  // energyBTdedx=0;
  // energyBTscintillator=0;

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

  // Default values
  upper_scintillator_threshold_ = 100.0 * keV;
  lower_scintillator_threshold_ = 100.0 * keV;
  electron_mcp_threshold_ = 2.0 * keV;

  event_messenger_ = new K37EventMessenger(this);
  the_aggregator_ = 0;
  // vector< double > spot;
}

K37EventAction::~K37EventAction() {
  delete stripHandler;
  delete event_messenger_;
}

void K37EventAction::BeginOfEventAction(const G4Event* ev) {
  if (ev -> GetEventID() % 1000 == 0) {
    G4cout << "Event " << ev -> GetEventID() << G4endl;
  }

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
    recoil_mcp_collection_id = SDman -> GetCollectionID(colNam="rMCP_HC");
    electron_mcp_collection_id = SDman -> GetCollectionID(colNam="eMCP_HC");
  }

  energyDedx = 0;
  energyDedx_Primaries = 0;
  energyDedx_Secondaries = 0;

  energyDedx2 = 0;
  energyDedx2_Primaries = 0;
  energyDedx2_Secondaries = 0;


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
  G4double energy_upper_scintillator = 0;
  G4double energy_upper_scintillator_secondaries = 0;
  G4double energy_upper_scintillator_primaries = 0;
  G4double energy_lower_scintillator = 0;
  G4double energy_lower_scintillator_secondaries = 0;
  G4double energy_lower_scintillator_primaries = 0;

  G4double time_upper_scintillator = 0.0;
  G4double time_lower_scintillator = 0.0;

  G4int upper_scintillator_pdg = 0;
  G4int lower_scintillator_pdg = 0;

  // Total energy stored in all the strips (each detector, each direction)
  // Pretty redundant, but a good check for bugs
  G4double sd_energy_total_plusZ_X = 0.0;
  G4double sd_energy_total_plusZ_Y = 0.0;
  G4double sd_energy_total_minsZ_X = 0.0;
  G4double sd_energy_total_minsZ_Y = 0.0;

  // Strip-by-strip information for each detector
  vector<G4double> sd_energy_plusZ_X(40, 0.0), sd_energy_plusZ_Y(40, 0.0);
  vector<G4double> sd_energy_minsZ_X(40, 0.0), sd_energy_minsZ_Y(40, 0.0);

  // rMCP info
  G4double recoil_mcp_x_pos = 0.0;
  G4double recoil_mcp_z_pos = 0.0;
  G4double recoil_mcp_time = 0.0;
  G4int recoil_pdg = 0;

  // eMCP info
  G4double electron_mcp_time = 0.0;
  G4double electron_mcp_energy = 0.0;

  if (fullenergy1CollID < 0 || dedx1CollID < 0) return;

  G4HCofThisEvent * hit_collection = evt->GetHCofThisEvent();
  K37StripDetectorHitsCollection* upper_sd_hit_collection = 0;
  K37StripDetectorHitsCollection* lower_sd_hit_collection = 0;
  K37ScintillatorHitsCollection* upper_scintillator_hit_collection = 0;
  K37ScintillatorHitsCollection* lower_scintillator_hit_collection = 0;
  K37RecoilMCPHitsCollection* recoil_mcp_hit_collection = 0;
  K37ElectronMCPHitsCollection* electron_mcp_hit_collection = 0;
  // G4THitsCollection<K37RecoilMCPHit> *recoil_mcp_hit_collection = 0;

  if (hit_collection) {
    upper_scintillator_hit_collection =
      static_cast<K37ScintillatorHitsCollection*>(hit_collection ->
                                                  GetHC(fullenergy1CollID));
    lower_scintillator_hit_collection =
      static_cast<K37ScintillatorHitsCollection*>(hit_collection ->
                                                  GetHC(fullenergy2CollID));
    upper_sd_hit_collection =
      static_cast<K37StripDetectorHitsCollection*>(hit_collection ->
                                                   GetHC(dedx1CollID));
    lower_sd_hit_collection =
      static_cast<K37StripDetectorHitsCollection*>(hit_collection ->
                                                   GetHC(dedx2CollID));
    recoil_mcp_hit_collection =
        static_cast<K37RecoilMCPHitsCollection*>(hit_collection ->
                                              GetHC(recoil_mcp_collection_id));
    electron_mcp_hit_collection =
        static_cast<K37ElectronMCPHitsCollection*>(hit_collection ->
                                           GetHC(electron_mcp_collection_id));
  }

  // ***************************************************************************
  // Get the energy deposited in the upper scintillator
  // ***************************************************************************
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
      if (i == 0) {                     // First hit - get particle
        upper_scintillator_pdg = hit -> GetParticlePDG();
      }
    }
    if (energy_upper_scintillator>0) {
      isThereEnergySili = true;
      if (energy_upper_scintillator_primaries == 0) {
        EventInformation->setGammaFiredScintillatorPlusZToTrue();
      }
    }
  }

  // ***************************************************************************
  // Get all the energy deposited in the lower scintillator
  // ***************************************************************************
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
      if (i == 0) {                     // First hit - get particle
        lower_scintillator_pdg = hit -> GetParticlePDG();
      }
    }
    if (energy_lower_scintillator>0) {
      isThereEnergySili2 = true;
      if (energy_lower_scintillator_primaries == 0) {
        EventInformation->setGammaFiredScintillatorMinusZToTrue();
      }
    }
  }

  // ***************************************************************************
  // Get all the energy deposited in the upper strip detector
  // ***************************************************************************
  if (upper_sd_hit_collection) {                // Strip detector plus Z
    sd_energy_plusZ_X = GetEDepVectorX(upper_sd_hit_collection);
    sd_energy_plusZ_Y = GetEDepVectorY(upper_sd_hit_collection);
    for (int i = 0; i < 40; i++) {
      sd_energy_total_plusZ_X += sd_energy_plusZ_X[i];
      sd_energy_total_plusZ_Y += sd_energy_plusZ_Y[i];
    }
    if (fabs(sd_energy_total_plusZ_X - sd_energy_total_plusZ_Y) >
        pow(10.0, -3)*keV) {
      G4cout << "ERROR.  X-ENERGY != Y-ENERGY." << G4endl;
    }
  }  // End strip detector plus z

  // ***************************************************************************
  // Get all the energy deposited in the lower strip detector
  // ***************************************************************************
  if (lower_sd_hit_collection) {  // Strip Detector Minus Z
    sd_energy_minsZ_X = GetEDepVectorX(lower_sd_hit_collection);
    sd_energy_minsZ_Y = GetEDepVectorY(lower_sd_hit_collection);
    for (int i = 0; i < 40; i++) {
      sd_energy_total_minsZ_X += sd_energy_minsZ_X[i];
      sd_energy_total_minsZ_Y += sd_energy_minsZ_Y[i];
    }
    if (fabs(sd_energy_total_minsZ_X - sd_energy_total_minsZ_Y) >
        pow(10.0, -3)*keV) {
      G4cout << "ERROR.  X-ENERGY != Y-ENERGY." << G4endl;
    }
  }

  K37ScintillatorHit *first_hit = 0;
  if (energy_upper_scintillator > upper_scintillator_threshold_) {
    first_hit = (*upper_scintillator_hit_collection)[0];
    time_upper_scintillator = first_hit -> GetTime();
  } else {
    time_upper_scintillator = 0.0;
  }
  if (energy_lower_scintillator > lower_scintillator_threshold_) {
    first_hit = (*lower_scintillator_hit_collection)[0];
    time_lower_scintillator = first_hit -> GetTime();
  } else {
    time_lower_scintillator = 0.0;
  }

  // ***************************************************************************
  // Get all the info the the recoil MCP detector
  // Use only the first hit
  // ***************************************************************************
  if (recoil_mcp_hit_collection) {      // rMCP
    if (recoil_mcp_hit_collection -> entries() > 0) {
      K37RecoilMCPHit *hit = (*recoil_mcp_hit_collection)[0];
      recoil_mcp_x_pos = hit -> GetXPos();
      recoil_mcp_z_pos = hit -> GetZPos();
      recoil_mcp_time  = hit -> GetTime();
      recoil_pdg       = hit -> GetParticlePDG();
      // G4cout << "Recoil MCP hit at x = "
      //        << G4BestUnit(recoil_mcp_x_pos, "Length") << " and y = "
      //        << G4BestUnit(recoil_mcp_z_pos, "Length") << G4endl;
    } else {
      // G4cout << "Got no hits in rMCP!" << G4endl;
      recoil_mcp_x_pos = 0.0;
      recoil_mcp_z_pos = 0.0;
      recoil_mcp_time  = 0.0;
    }
  }

  // ***************************************************************************
  // Get all the info the the electron MCP detector
  // Use only the first hit
  // ***************************************************************************
  if (electron_mcp_hit_collection) {      // rMCP
    if (electron_mcp_hit_collection -> entries() > 0) {
      int n_hit = electron_mcp_hit_collection -> entries();
      K37ElectronMCPHit *hit = (*electron_mcp_hit_collection)[0];
      electron_mcp_time = hit -> GetTime();
      for (int i = 0; i < n_hit; i++) {
        hit = (*electron_mcp_hit_collection)[i];
        electron_mcp_energy += hit -> GetEnergy();
      }
    }
  }

  if (EventPassesTrigger(energy_upper_scintillator, energy_lower_scintillator,
                         electron_mcp_energy)) {
    // G4cout << "Event passes my trigger with energy "
    //        << G4BestUnit(energy_upper_scintillator, "Energy") << " / "
    //        << G4BestUnit(energyDedx, "Energy") << G4endl << "\t\t"
    //        << G4BestUnit(energy_lower_scintillator, "Energy") << " / "
    //        << G4BestUnit(energyDedx2, "Energy") << G4endl;

    // G4double trigTime = pow(10.0, 10);
    // if (time_upper_scintillator > 0.0) {  // Then its above threshold
      // trigTime = min(trigTime, time_upper_scintillator/ns);
    // }
    // if (time_lower_scintillator > 0.0) {  // Then its above threhsold
      // trigTime = min(trigTime, time_lower_scintillator/ns);
    // }

    accepted++;
    runAct->SetAccepted();


    EventInformation->setThisIsAnAccepterEvent();

    (*active_channels_)["QDC_UpperPMT"] ->
        InsertData(energy_upper_scintillator/keV);
    (*active_channels_)["QDC_LowerPMT"] ->
        InsertData(energy_lower_scintillator/keV);
    (*active_channels_)["DL_X_Pos"] -> InsertData(recoil_mcp_x_pos/mm);
    (*active_channels_)["DL_Z_Pos"] -> InsertData(recoil_mcp_z_pos/mm);
    (*active_channels_)["TDC_SCINT_TOP"] ->
        InsertData(time_upper_scintillator/ns/v1190_factor_ns);
    (*active_channels_)["TDC_SCINT_BOTTOM"] ->
        InsertData(time_lower_scintillator/ns/v1190_factor_ns);
    (*active_channels_)["TDC_ION_MCP"] ->
        InsertData(recoil_mcp_time/ns/v1190_factor_ns);
    (*active_channels_)["TDC_ELECTRON_MCP"] ->
        InsertData(electron_mcp_time/ns/v1190_factor_ns);
    (*active_channels_)["ION_MCP_PARTICLE_PDG"] ->
        InsertData((G4double)recoil_pdg);
    (*active_channels_)["UPPER_SCINTILLATOR_PDG"] ->
        InsertData((G4double)upper_scintillator_pdg);
    (*active_channels_)["LOWER_SCINTILLATOR_PDG"] ->
        InsertData((G4double)lower_scintillator_pdg);

    // runAct->SetAcceptedPrimaryScatteredOffHoops();
    // Note: Dedx means strip detector and SiLi means scintillator
    // Fill all the ntuples with data from the vectors
    if (fillAllSDData) {
      // New (Aggregator) way
      fillSDNtuples(sd_energy_minsZ_X, "STRIP_LX_");
      fillSDNtuples(sd_energy_minsZ_Y, "STRIP_LY_");
      fillSDNtuples(sd_energy_plusZ_X, "STRIP_UX_");
      fillSDNtuples(sd_energy_plusZ_Y, "STRIP_UY_");
    }

    if (isThereEnergyDedx == true) {
      // stripHandler->PrintMaps();
      stripHandler->PassThePlusZDetectors();

      // ofstream detectorADA;
      // detectorADA.open("detectorADA.txt", ofstream::out | ofstream::app);
      // detectorADA.setf(std::ios::fixed, std::ios::floatfield);
      // detectorADA << setw(15) << left << energyDedx/keV << setw(15) << left
      //             << energy_upper_scintillator/keV << G4endl;
      // detectorADA.close();

      // histograms->FillNtuple(1, 0, energy_upper_scintillator);
      // histograms->FillNtuple(0, 0, energyDedx);
      // histograms->AddRowNtuple(1);
      // histograms->AddRowNtuple(0);


      runAct -> incrementPlusZ_vc(GetRelativisticFactor(emass,
                                                    energy_upper_scintillator));

      EventInformation ->
          setTotalEnergyInScintillator(energy_upper_scintillator);
      EventInformation -> setTotalEnergyInStripDetector(energyDedx);
      EventInformation -> setPlusZDetectorsFiredToTrue();
      EventInformation ->
          setStartStripDetectorTime((*upper_sd_hit_collection)[0] -> GetTime());
      if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
        listOfEnteredVolumes -> setEnteredVolumeName("----------------------A",
                                                     0, 0);
      }
    }  // End energy in plusZ

    if (isThereEnergyDedx2 == true) {
      stripHandler->PassTheMinusZDetectors();

      // ofstream detectorODA;
      // detectorODA.open("detectorODA.txt", ofstream::out | ofstream::app);
      // detectorODA.setf(std::ios::fixed, std::ios::floatfield);
      // detectorODA << setw(15)<< left << energyDedx2/keV << setw(15) << left
      //             << energy_lower_scintillator/keV << G4endl;
      // detectorODA.close();

      // histograms->FillNtuple(3, 0, energy_lower_scintillator);
      // histograms->FillNtuple(2, 0, energyDedx2);
      // histograms->AddRowNtuple(3);
      // histograms->AddRowNtuple(2);

      runAct ->
          incrementMinusZ_vc(GetRelativisticFactor(emass,
                                                   energy_lower_scintillator));
      EventInformation ->
          setTotalEnergyInScintillator(energy_lower_scintillator);
      EventInformation -> setTotalEnergyInStripDetector(energyDedx2);
      EventInformation ->
          setStartStripDetectorTime((*lower_sd_hit_collection)[0]->GetTime());

      if (listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded()) {
        listOfEnteredVolumes -> setEnteredVolumeName("----------------------O",
                                                     0, 0);
      }
    }  // End energy in minusZ

    // Add a new row here to add a new row for only accpeted events where
    // either there was energy in the plus or minus z detector, but not both!
    // Fill Run_Number with negative version number to indicate simulated
    (*active_channels_)["Run_Number"] -> InsertData(-1.0*K37_VERSION);

    G4double polarization = primary_generator_ -> GetPolarization();
    // 1000000 ns = 1000 us is within the optical pumping on time
    // 10 ns is outside the optical pumping on time
    uint64_t  op_time;
    G4double op_bit_onoff;
    if (fabs(polarization) > 0) {  // Polarized
      op_time = 1000000;   // Will resolove to polarized as if it were real data
      op_bit_onoff = 1.0;
    } else {               // Not polarized
      op_time = 10;        // Will show up as unpolarized just like real data
      op_bit_onoff = 0.0;
    }
    (*active_channels_)["TNIM_OP_Beam"] -> InsertDataL(op_time);
    (*active_channels_)["TTLBit_OPBeam"] -> InsertData(op_bit_onoff);
    // To match the analyzer TTLBit_Sigmaplus should be one for s+ and 0 for s-
    // Unpolarized light should be vetoed by  TNIM_OP_Beam, but I will give it
    // an error code of -10.
    G4double op_bit;
    if (polarization > 0) {
      op_bit = 1.0;
    } else if (polarization < 0) {
      op_bit = 0.0;
    } else {
      op_bit = -10.0;
    }
    (*active_channels_)["TTLBit_SigmaPlus"] -> InsertData(op_bit);

    the_aggregator_ -> EndEvent();
  }
  // PrintEvent(evt);
  //  G4cout << "------------------------------" << G4endl;
  // Add a new row here to add a new row for EVERY EVENT, even events that were
  // not "accepted."
}  // End of event action
//----------------
void K37EventAction::setEnteringDedx(G4ThreeVector enteringPosition) {
  spot.push_back(enteringPosition);
}

void K37EventAction::setStartingDirection(G4ThreeVector startingPosition) {
  start.push_back(startingPosition);
}

void K37EventAction::fillSDNtuples(vector<G4double> energy_strip,
                                   G4String name) {
  G4bool debug = false;
  char full_name[200];
  for (G4int i = 0; i < 40; i++) {
    snprintf(full_name, sizeof(full_name), "%s%02d", name.c_str(), i+1);
    (*active_channels_)[full_name] -> InsertData(energy_strip[i]/keV);
    if (energy_strip[i] > 0 && debug) {
      G4cout << full_name << " with " << energy_strip[i]/keV << G4endl;
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
  if (!event) {
    G4cout << "Event invalid!" << G4endl;
    return;
  }
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
         << number_trajectories << G4endl;
  for (G4int i = 0; i < number_vertices; i++) {
    p_def = event -> GetPrimaryVertex(i) -> GetPrimary(0) -> GetG4code();
    name = p_def -> GetParticleName();
    charge = p_def -> GetPDGCharge();
    G4cout << "Vertex " << i << " has primary particle: " << name
           << " (" << charge << ") " << G4endl;
  }
  G4cout << G4endl;
  for (G4int i = 0; i < number_trajectories; i++) {
    trajectory = reinterpret_cast<G4Trajectory*>((*trajectory_container)[i]);
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

bool K37EventAction::EventPassesTrigger(double upper_scintillator_energy,
                                        double lower_scintillator_energy,
                                        double electron_mcp_energy) {
  return (upper_scintillator_energy > upper_scintillator_threshold_ ||
          lower_scintillator_energy > lower_scintillator_threshold_ ||
          electron_mcp_energy > electron_mcp_threshold_);
}
