// Authors: Spencer Behling and Benjamin Fenker 2014

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

#include "K37Analysis.hh"
#include "K37AnalysisNumbering.hh"
#include "K37Config.hh"
#include "K37ContainerForStripInformation.hh"
#include "K37ElectronMCPHit.hh"
#include "K37EventAction.hh"
#include "K37RecoilMCPHit.hh"
#include "K37RunAction.hh"
#include "K37ScintillatorHit.hh"

using std::min;
using std::pow;
using std::ofstream;
using std::setw;
using std::left;

K37EventAction::K37EventAction(K37RunAction* run)
    :v1190_factor_ns(0.09765625),
    runAct(run),
    stripHandler(new K37ContainerForStripInformation()),
     count_backscatters_(false),
    energyUpperScint_Total(0),
    energyUpperScint_AllElse(0),
    energyUpperScint_Positron(0),
    energyUpperScint_Electron(0),
    energyUpperScint_Gamma(0),
    energyLowerScint_Total(0),
    energyLowerScint_AllElse(0),
    energyLowerScint_Positron(0),
    energyLowerScint_Electron(0),
    energyLowerScint_Gamma(0),
    energyUpperSilicon_Total(0),
    energyUpperSilicon_AllElse(0),
    energyUpperSilicon_Positron(0),
    energyUpperSilicon_Electron(0),
    energyUpperSilicon_Gamma(0),
    energyLowerSilicon_Total(0),
    energyLowerSilicon_AllElse(0),
    energyLowerSilicon_Positron(0),
    energyLowerSilicon_Electron(0),
     energyLowerSilicon_Gamma(0)
{

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
  // upper_scintillator_threshold_ = 100.0 * keV;
  // lower_scintillator_threshold_ = 100.0 * keV;
  electron_mcp_threshold_ = 2.0 * keV;

  G4cout << G4String(CONFIGURATION_DIRECTORY) << G4endl;
  // Declare digitizers here (following example advanced/ChargeExchangeMC)
  G4DigiManager *digiManager(G4DigiManager::GetDMpointer());

  digiManager ->
      AddNewModule(new K37ScintillatorDigitizer("scintillatorPlusZ"));
  digiManager ->
      AddNewModule(new K37ScintillatorDigitizer("scintillatorMinusZ"));

  digiManager -> AddNewModule(new K37StripDetectorDigitizer("dsssdPlusZ",
                            (G4String(CONFIGURATION_DIRECTORY) +
                             G4String("/upper_strip_detector_x.res")).c_str(),
                            (G4String(CONFIGURATION_DIRECTORY) +
                             G4String("/upper_strip_detector_y.res")).c_str()));

  digiManager -> AddNewModule(new K37StripDetectorDigitizer("dsssdMinusZ",
                            (G4String(CONFIGURATION_DIRECTORY) +
                             G4String("/lower_strip_detector_x.res")).c_str(),
                            (G4String(CONFIGURATION_DIRECTORY) +
                             G4String("/lower_strip_detector_y.res")).c_str()));

  digiManager -> List();

  event_messenger_ = new K37EventMessenger(this);
  the_aggregator_ = 0;
  // vector< double > spot;
}

K37EventAction::~K37EventAction() {
  delete stripHandler;
  delete event_messenger_;
}

void K37EventAction::BeginOfEventAction(const G4Event* ev) {
  if (ev -> GetEventID() == 0) G4cout << endl;
  if (ev -> GetEventID() % 50000 == 0) {
    G4cout << "\r" << "Event " << ev -> GetEventID() << G4endl;
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

  energyUpperScint_Total = 0;
  energyUpperScint_AllElse  = 0;
  energyUpperScint_Positron = 0;
  energyUpperScint_Electron = 0;
  energyUpperScint_Gamma    = 0;

  energyLowerScint_Total = 0;
  energyLowerScint_AllElse = 0;
  energyLowerScint_Positron = 0;
  energyLowerScint_Electron = 0;
  energyLowerScint_Gamma = 0;

  energyUpperSilicon_Total = 0;
  energyUpperSilicon_AllElse  = 0;
  energyUpperSilicon_Positron = 0;
  energyUpperSilicon_Electron = 0;
  energyUpperSilicon_Gamma    = 0;

  energyLowerSilicon_Total = 0;
  energyLowerSilicon_AllElse = 0;
  energyLowerSilicon_Positron = 0;
  energyLowerSilicon_Electron = 0;
  energyLowerSilicon_Gamma = 0;

  G4double time_upper_scintillator = 0.0;
  G4double time_lower_scintillator = 0.0;

  G4int upper_scintillator_pdg = 0;
  G4int lower_scintillator_pdg = 0;

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
  G4int electron_pdg = 0;

  // Digitizers
  G4DigiManager *digitizer_manager(G4DigiManager::GetDMpointer());
  K37ScintillatorDigitizer *upper_scintillator_digitizer(
      static_cast<K37ScintillatorDigitizer *>(
          digitizer_manager -> FindDigitizerModule("scintillatorPlusZ")));

  K37ScintillatorDigitizer *lower_scintillator_digitizer(
      static_cast<K37ScintillatorDigitizer *>(
          digitizer_manager -> FindDigitizerModule("scintillatorMinusZ")));

  K37StripDetectorDigitizer *upper_strip_detector_digitizer(
      static_cast<K37StripDetectorDigitizer *>(
          digitizer_manager -> FindDigitizerModule("dsssdPlusZ")));

  K37StripDetectorDigitizer *lower_strip_detector_digitizer(
      static_cast<K37StripDetectorDigitizer *>(
          digitizer_manager -> FindDigitizerModule("dsssdMinusZ")));


  lower_scintillator_digitizer -> Digitize();
  upper_scintillator_digitizer -> Digitize();

  upper_strip_detector_digitizer -> Digitize();
  lower_strip_detector_digitizer -> Digitize();

  if (fullenergy1CollID < 0 || dedx1CollID < 0) return;

  G4HCofThisEvent * hit_collection = evt->GetHCofThisEvent();
  K37RecoilMCPHitsCollection* recoil_mcp_hit_collection = 0;
  K37ElectronMCPHitsCollection* electron_mcp_hit_collection = 0;

  if (hit_collection) {
    recoil_mcp_hit_collection =
        static_cast<K37RecoilMCPHitsCollection*>(hit_collection ->
                                              GetHC(recoil_mcp_collection_id));
    electron_mcp_hit_collection =
        static_cast<K37ElectronMCPHitsCollection*>(hit_collection ->
                                           GetHC(electron_mcp_collection_id));
  }
  G4bool electron_mcp_constructed =
      detector_construction_ -> GetMakeElectronMCP();
  G4bool recoil_mcp_constructed = detector_construction_ -> GetMakeRecoilMCP();


  // ************************************************************
  // -------------------- Query the upper scintillator
  // ************************************************************
  energyUpperScint_Total = upper_scintillator_digitizer -> GetEnergyTotal();
  energyUpperScint_Electron =
      upper_scintillator_digitizer -> GetEnergyElectron();

  energyUpperScint_Positron =
      upper_scintillator_digitizer -> GetEnergyPositron();

  energyUpperScint_Gamma = upper_scintillator_digitizer -> GetEnergyGamma();
  energyUpperScint_AllElse = upper_scintillator_digitizer -> GetEnergyOther();
  time_upper_scintillator = upper_scintillator_digitizer -> GetHitTime();
  upper_scintillator_pdg = upper_scintillator_digitizer -> GetParticleCode();
  // ------------------------------------------------------------

  // ************************************************************
  // -------------------- Query the lower scintillator
  // ************************************************************
  energyLowerScint_Total = lower_scintillator_digitizer -> GetEnergyTotal();
  energyLowerScint_Electron =
      lower_scintillator_digitizer -> GetEnergyElectron();

  energyLowerScint_Positron =
      lower_scintillator_digitizer -> GetEnergyPositron();

  energyLowerScint_Gamma = lower_scintillator_digitizer -> GetEnergyGamma();
  energyLowerScint_AllElse = lower_scintillator_digitizer -> GetEnergyOther();
  time_upper_scintillator = upper_scintillator_digitizer -> GetHitTime();
  upper_scintillator_pdg = upper_scintillator_digitizer -> GetParticleCode();
  // ------------------------------------------------------------

  // ************************************************************
  // -------------------- Query the upper strip detector
  // ************************************************************
  sd_energy_plusZ_X = upper_strip_detector_digitizer -> GetEnergyXstrip();
  sd_energy_plusZ_Y = upper_strip_detector_digitizer -> GetEnergyYstrip();

  energyUpperSilicon_Total = upper_strip_detector_digitizer -> GetEnergyTotal();
  energyUpperSilicon_Electron =
      upper_strip_detector_digitizer -> GetEnergyElectron();

  energyUpperSilicon_Positron =
      upper_strip_detector_digitizer -> GetEnergyPositron();

  energyUpperSilicon_Gamma = upper_strip_detector_digitizer -> GetEnergyGamma();
  energyUpperSilicon_AllElse =
      upper_strip_detector_digitizer -> GetEnergyOther();
  // ------------------------------------------------------------

  // ************************************************************
  // -------------------- Query the lower strip detector
  // ************************************************************
  sd_energy_minsZ_X = lower_strip_detector_digitizer -> GetEnergyXstrip();
  sd_energy_minsZ_Y = lower_strip_detector_digitizer -> GetEnergyYstrip();

  energyLowerSilicon_Total = lower_strip_detector_digitizer -> GetEnergyTotal();
  energyLowerSilicon_Electron =
      lower_strip_detector_digitizer -> GetEnergyElectron();

  energyLowerSilicon_Positron =
      lower_strip_detector_digitizer -> GetEnergyPositron();

  energyLowerSilicon_Gamma = lower_strip_detector_digitizer -> GetEnergyGamma();
  energyLowerSilicon_AllElse =
      lower_strip_detector_digitizer -> GetEnergyOther();
  // ------------------------------------------------------------

  // ************************************************************
  // -------------------- Check for backscattering events
  // ************************************************************
  if (energyUpperSilicon_Total > 5*eV && energyLowerSilicon_Total > 5*eV &&
      count_backscatters_) {
    //    G4cout << "Probable backscatter..." << G4endl;
    ofstream ofs;
    ofs.open("backscatter.txt", std::ofstream::app);
    ofs << energyUpperSilicon_Total/keV << "\t"
        << upper_strip_detector_digitizer -> GetParticleCode() << "\t"
        << energyLowerSilicon_Total/keV << "\t"
        << lower_strip_detector_digitizer -> GetParticleCode() << G4endl;
    G4EventManager::GetEventManager()->KeepTheCurrentEvent();
    ofs.close();
  }
  // ------------------------------------------------------------

  // ***************************************************************************
  // Get all the info the the recoil MCP detector
  // Use only the first hit
  // ***************************************************************************
  if (recoil_mcp_hit_collection && recoil_mcp_constructed) {      // rMCP
    if (recoil_mcp_hit_collection -> entries() > 0) {
      K37RecoilMCPHit *hit = (*recoil_mcp_hit_collection)[0];
      recoil_mcp_x_pos = hit -> GetXPos();
      recoil_mcp_z_pos = hit -> GetZPos();
      recoil_mcp_time  = hit -> GetTime();
      recoil_pdg       = hit -> GetParticlePDG();
      /* DELETE HERE DELETE HERE */
      // G4cout << "Recoil MCP hit at x = "
      //        << G4BestUnit(recoil_mcp_x_pos, "Length") << " and y = "
      //        << G4BestUnit(recoil_mcp_z_pos, "Length") << G4endl;
      // for (G4int i = 0; i < recoil_mcp_hit_collection -> entries(); i++) {
      //   G4cout << "Ion Hit " << i << " at time "
      //          << G4BestUnit(recoil_mcp_time, "Time") << G4endl;
      // }
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
  if (electron_mcp_hit_collection && electron_mcp_constructed) {      // eMCP
    if (electron_mcp_hit_collection -> entries() > 0) {
      int n_hit = electron_mcp_hit_collection -> entries();

      K37ElectronMCPHit *hit = (*electron_mcp_hit_collection)[0];
      electron_mcp_time = hit -> GetTime();
      electron_pdg = hit -> GetParticlePDG();

      for (int i = 0; i < n_hit; i++) {
        hit = (*electron_mcp_hit_collection)[i];
        electron_mcp_energy += hit -> GetEnergy();
        /* DELETE HERE DELETE HERE */
        // G4cout << "Electron Hit " << i << " at time "
        //        << G4BestUnit(electron_mcp_time, "Time") << G4endl;
      }
    }
  }

  // if (EventPassesTrigger(energyUpperScint_Total, energyLowerScint_Total,
  //                        electron_mcp_energy)) {
  if (upper_scintillator_digitizer -> IsTriggered() ||
      lower_scintillator_digitizer -> IsTriggered() ||
      electron_mcp_energy > electron_mcp_threshold_) {
    G4EventManager::GetEventManager()->KeepTheCurrentEvent();
    // G4cout << "Event passes my trigger with energy "
    //        << G4BestUnit(energyUpperScint_Total, "Energy") << " / "
    //        << G4BestUnit(energyDedx, "Energy") << G4endl << "\t\t"
    //        << G4BestUnit(energyLowerScint_Total, "Energy") << " / "
    //        << G4BestUnit(energyDedx2, "Energy") << G4endl << "\t\t"
    //        << G4BestUnit(electron_mcp_energy, "Energy") << G4endl;
    
    // G4double trigTime = pow(10.0, 10);
    // if (time_upper_scintillator > 0.0) {  // Then its above threshold
      // trigTime = min(trigTime, time_upper_scintillator/ns);
    // }
    // if (time_lower_scintillator > 0.0) {  // Then its above threhsold
      // trigTime = min(trigTime, time_lower_scintillator/ns);
    // }

    accepted++;
    runAct->SetAccepted();

    //(*active_channels_)["QDC_LowerPMT"] ->
    //InsertData(energyLowerScint_Total/keV);
    (*active_channels_)["QDC_UpperPMT"   ] -> InsertData(energyUpperScint_Total/keV   );
    (*active_channels_)["QDC_UpperPMT_AE"] -> InsertData(energyUpperScint_AllElse/keV );
    (*active_channels_)["QDC_UpperPMT_P" ] -> InsertData(energyUpperScint_Positron/keV);
    (*active_channels_)["QDC_UpperPMT_E" ] -> InsertData(energyUpperScint_Electron/keV);
    (*active_channels_)["QDC_UpperPMT_G" ] -> InsertData(energyUpperScint_Gamma/keV   );

    (*active_channels_)["QDC_LowerPMT"   ] -> InsertData(energyLowerScint_Total/keV   );
    (*active_channels_)["QDC_LowerPMT_AE"] -> InsertData(energyLowerScint_AllElse/keV );
    (*active_channels_)["QDC_LowerPMT_P" ] -> InsertData(energyLowerScint_Positron/keV);
    (*active_channels_)["QDC_LowerPMT_E" ] -> InsertData(energyLowerScint_Electron/keV);
    (*active_channels_)["QDC_LowerPMT_G" ] -> InsertData(energyLowerScint_Gamma/keV   );

    (*active_channels_)["QDC_UpperSilicon"   ] -> InsertData(energyUpperSilicon_Total/keV   );
    (*active_channels_)["QDC_UpperSilicon_AE"] -> InsertData(energyUpperSilicon_AllElse/keV );
    (*active_channels_)["QDC_UpperSilicon_P" ] -> InsertData(energyUpperSilicon_Positron/keV);
    (*active_channels_)["QDC_UpperSilicon_E" ] -> InsertData(energyUpperSilicon_Electron/keV);
    (*active_channels_)["QDC_UpperSilicon_G" ] -> InsertData(energyUpperSilicon_Gamma/keV   );

    (*active_channels_)["QDC_LowerSilicon"   ] -> InsertData(energyLowerSilicon_Total/keV   );
    (*active_channels_)["QDC_LowerSilicon_AE"] -> InsertData(energyLowerSilicon_AllElse/keV );
    (*active_channels_)["QDC_LowerSilicon_P" ] -> InsertData(energyLowerSilicon_Positron/keV);
    (*active_channels_)["QDC_LowerSilicon_E" ] -> InsertData(energyLowerSilicon_Electron/keV);
    (*active_channels_)["QDC_LowerSilicon_G" ] -> InsertData(energyLowerSilicon_Gamma/keV   );


    (*active_channels_)["TDC_SCINT_TOP"] ->
        InsertData(time_upper_scintillator/ns/v1190_factor_ns);
    (*active_channels_)["TDC_SCINT_BOTTOM"] ->
        InsertData(time_lower_scintillator/ns/v1190_factor_ns);
    // G4cout << "TDC_ION_MCP     : " << recoil_mcp_time/ns << G4endl;
    // G4cout << "TDC_ELECTRON_MCP: " << electron_mcp_time/ns << G4endl;
    if (electron_mcp_constructed) {
      (*active_channels_)["TDC_ELECTRON_MCP"] ->
          InsertData(electron_mcp_time/ns/v1190_factor_ns);
      (*active_channels_)["ELECTRON_MCP_N_HITS"] ->
          InsertData((G4double)electron_mcp_hit_collection -> entries());
      (*active_channels_)["ELECTRON_MCP_PARTICLE_PDG"] ->
          InsertData((G4double)electron_pdg);
    }
    if (recoil_mcp_constructed) {
      (*active_channels_)["DL_X_Pos"] -> InsertData(recoil_mcp_x_pos/mm);
      (*active_channels_)["DL_Z_Pos"] -> InsertData(recoil_mcp_z_pos/mm);
      (*active_channels_)["TDC_ION_MCP"] ->
          InsertData(recoil_mcp_time/ns/v1190_factor_ns);
      (*active_channels_)["ION_MCP_PARTICLE_PDG"] ->
          InsertData((G4double)recoil_pdg);
    }
    (*active_channels_)["UPPER_SCINTILLATOR_PDG"] ->
        InsertData((G4double)upper_scintillator_pdg);
    (*active_channels_)["LOWER_SCINTILLATOR_PDG"] ->
        InsertData((G4double)lower_scintillator_pdg);

    /* DELETE HERE DELETE HERE */
    //    if (recoil_mcp_time - electron_mcp_time > 1510.0*ns) LookAtEvent(evt);

    // runAct->SetAcceptedPrimaryScatteredOffHoops();
    // Note: Dedx means strip detector and SiLi means scintillator
    // Fill all the ntuples with data from the vectors
    if (runAct -> RecordStripDetectorData()) {
      // New (Aggregator) way
      fillSDNtuples(sd_energy_minsZ_X, "STRIP_LX_");
      fillSDNtuples(sd_energy_minsZ_Y, "STRIP_LY_");
      fillSDNtuples(sd_energy_plusZ_X, "STRIP_UX_");
      fillSDNtuples(sd_energy_plusZ_Y, "STRIP_UY_");
    }

    if (isThereEnergyDedx == true) {
      // stripHandler->PrintMaps();
      stripHandler->PassThePlusZDetectors();

      runAct -> incrementPlusZ_vc(GetRelativisticFactor(emass,
               energyUpperScint_Total));
    }  // End energy in plusZ

    if (isThereEnergyDedx2 == true) {
      stripHandler->PassTheMinusZDetectors();

      runAct -> incrementMinusZ_vc(GetRelativisticFactor(emass,
               energyLowerScint_Total));
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
      op_time = primary_generator_ -> GetCloudSize() -> GetDecayTime();
      op_bit_onoff = 1.0;
    } else {               // Not polarized
      op_time = 10;        // Will show up as unpolarized just like real data
      op_bit_onoff = 0.0;
    }
    (*active_channels_)["TNIM_OP_Beam"] -> InsertDataL(op_time/ns);
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


void K37EventAction::LookAtEvent(const G4Event *event) {
  G4TrajectoryContainer *trajectory = event -> GetTrajectoryContainer();
  G4VTrajectory *v_trajectory;
  for (unsigned int i = 0; i < trajectory -> size(); i++) {
    v_trajectory = (*trajectory)[i];
    v_trajectory -> ShowTrajectory();
    v_trajectory -> DrawTrajectory();
    G4cout << G4endl;
  }
  G4cout << G4endl << "Any number to continue..." << G4endl;
  G4int j;
  G4cin >> j;
  G4cout << j << G4endl;
}


void K37EventAction::SortUpperSilicon(const G4int &pdgCode_,
      const G4double &energyHit_)
{

   energyUpperSilicon_Total += energyHit_;

   switch(pdgCode_)
   {
      case 11: //Electron
         {
            energyUpperSilicon_Electron += energyHit_;
            //G4cout<<"U Electron: "<<energyUpperSilicon_Electron<<G4endl;
            break;
         }
      case -11: //Positrons negative for anti-matter
         {
            energyUpperSilicon_Positron += energyHit_;
            //G4cout<<"U Positron: "<<energyUpperSilicon_Positron<<G4endl;
            break;
         }
      case 22: //Gamma 
         {
            energyUpperSilicon_Gamma += energyHit_;
            //G4cout<<"U Gamma: "<<energyUpperSilicon_Gamma<<G4endl;
            break;
         }
      default: //Everything else
         {
            energyUpperSilicon_AllElse += energyHit_;
            //G4cout<<"U All Else: "<<energyUpperSilicon_AllElse<<G4endl;
            break;
         }
   }
}

void K37EventAction::SortLowerSilicon(const G4int &pdgCode_,
      const G4double &energyHit_)
{

   energyLowerSilicon_Total += energyHit_;

   switch(pdgCode_)
   {
      case 11: //Electron
         {
            energyLowerSilicon_Electron += energyHit_;
            //G4cout<<"L Electron: "<<energyLowerSilicon_Electron<<G4endl;
            break;
         }
      case -11: //Positrons negative for anti-matter
         {
            energyLowerSilicon_Positron += energyHit_;
            //G4cout<<"L Positron: "<<energyLowerSilicon_Positron<<G4endl;
            break;
         }
      case 22: //Gamma 
         {
            energyLowerSilicon_Gamma += energyHit_;
            //G4cout<<"L Gamma: "<<energyLowerSilicon_Gamma<<G4endl;
            break;
         }
      default: //Everything else
         {
            energyLowerSilicon_AllElse += energyHit_;
            //G4cout<<"L All Else: "<<energyLowerSilicon_AllElse<<G4endl;
            break;
         }
   }
}
