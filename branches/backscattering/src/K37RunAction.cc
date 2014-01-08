// Authors: Spencer Behling and Benjamin Fenker 2013

#include <iomanip>
#include "K37RunAction.hh"

#include "K37EventAction.hh"
#include "K37PrimaryGeneratorAction.hh"
#include "K37DetectorConstruction.hh"
// Just need for testing
#include "JTW_Event.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4EmCalculator.hh"

#include "K37Analysis.hh"

using AGG::Aggregator;
using std::string;

G4bool fillEvGenData = true;
//----------------------------------

K37RunAction::K37RunAction()
    :runMessenger(0), record_strip_detector_data_(true) {
  // recordAnnihilationPosition = false;
  // recordAnnihilationPosition = true;
  //  recordVolumeNames = false;
  // recordVolumeNames = true;
  recordEventInformation = true;
  // recordEventInformation = false;
  NbofEvents = 0;
  accepted = 0;
  accepted_bs = 0;
  bs = 0;
  falsePositive = 0;
  falsePositiveWithSili = 0;
  falsePositiveWithDedx = 0;
  falsePositiveWithBoth = 0;

  acceptedWithSili2 = 0;

  acceptedWithDedx2 = 0;

  plusZHits_vc = 0.0;
  minusZHits_vc = 0.0;

  acceptedWithNone = 0;
  acceptedWithNoneSecondaryFiredDedx = 0;
  acceptedWithNoneSecondaryFiredSili = 0;
  acceptedWithNonePrimaryFiredBoth = 0;
  acceptedWithNoneSecondaryFiredBoth = 0;

  falsePositiveWithNone = 0;
  falsePositiveWithNoneSecondaryFiredDedx = 0;
  falsePositiveWithNoneSecondaryFiredSili = 0;
  falsePositiveWithNonePrimaryFiredBoth = 0;
  falsePositiveWithNoneSecondaryFiredBoth = 0;

  vetoed = 0;
  nonVetoed = 0;

  acceptedPrimaryScatteredOffHoops = 0;

  snprintf(configuration_filename_, sizeof(configuration_filename_),
           "IOconfiguration.mac");
  snprintf(output_directory_, sizeof(output_directory_),
           "/home/bfenker/geant4_workdir/K37Build");

  outFileName = "old_analysis_manager";
  // manager automatically
  runMessenger = new K37RunMessenger(this);

  // Detectors
  qdc_upper_pmt    = new Generic_Channel("QDC_UpperPMT"   , -1, "/D");
  qdc_lower_pmt    = new Generic_Channel("QDC_LowerPMT"   , -1, "/D");
  qdc_upper_pmt_ae = new Generic_Channel("QDC_UpperPMT_AE", -1, "/D");
  qdc_lower_pmt_ae = new Generic_Channel("QDC_LowerPMT_AE", -1, "/D");
  qdc_upper_pmt_p  = new Generic_Channel("QDC_UpperPMT_P" , -1, "/D");
  qdc_lower_pmt_p  = new Generic_Channel("QDC_LowerPMT_P" , -1, "/D");
  qdc_upper_pmt_e  = new Generic_Channel("QDC_UpperPMT_E" , -1, "/D");
  qdc_lower_pmt_e  = new Generic_Channel("QDC_LowerPMT_E" , -1, "/D");
  qdc_upper_pmt_g  = new Generic_Channel("QDC_UpperPMT_G" , -1, "/D");
  qdc_lower_pmt_g  = new Generic_Channel("QDC_LowerPMT_G" , -1, "/D");
  upper_dssd_parentID_ = new TDC_Channel("UpperSiliconParentID", -1,
                                         "/v", false);
  lower_dssd_parentID_ = new TDC_Channel("LowerSiliconParentID", -1,
                                         "/v", false);
  upper_dssd_boundary_status_= new TDC_Channel("UpperSiliconBoundary",
                                               -1, "/v", false);
  lower_dssd_boundary_status_= new TDC_Channel("LowerSiliconBoundary",
                                               -1, "/v", false);
  upper_dssd_theta_ = new TDC_Channel("UpperSiliconTheta", -1, "/v", false);
  lower_dssd_theta_ = new TDC_Channel("LowerSiliconTheta", -1, "/v", false);

  qdc_upper_dssd    = new Generic_Channel("QDC_UpperSilicon"   , -1, "/D");
  qdc_lower_dssd    = new Generic_Channel("QDC_LowerSilicon"   , -1, "/D");
  qdc_upper_dssd_ae = new Generic_Channel("QDC_UpperSilicon_AE", -1, "/D");
  qdc_lower_dssd_ae = new Generic_Channel("QDC_LowerSilicon_AE", -1, "/D");
  qdc_upper_dssd_p  = new Generic_Channel("QDC_UpperSilicon_P" , -1, "/D");
  qdc_lower_dssd_p  = new Generic_Channel("QDC_LowerSilicon_P" , -1, "/D");
  qdc_upper_dssd_e  = new Generic_Channel("QDC_UpperSilicon_E" , -1, "/D");
  qdc_lower_dssd_e  = new Generic_Channel("QDC_LowerSilicon_E" , -1, "/D");
  qdc_upper_dssd_g  = new Generic_Channel("QDC_UpperSilicon_G" , -1, "/D");
  qdc_lower_dssd_g  = new Generic_Channel("QDC_LowerSilicon_G" , -1, "/D");


  dl_x_pos_      = new TDC_Channel("DL_X_Pos"    , 3, "/v", false);
  dl_z_pos_      = new TDC_Channel("DL_Z_Pos"    , 4, "/v", false);
  char name[200];
  for (G4int i = 0; i < 40; i++) {
    snprintf(name, sizeof(name), "STRIP_UX_%02d", i+1);
    strip_detector_upper_x_[i] = new Generic_Channel(name, i+5, "/D");
    snprintf(name, sizeof(name), "STRIP_UY_%02d", i+1);
    strip_detector_upper_y_[i] = new Generic_Channel(name, i+45, "/D");
    snprintf(name, sizeof(name), "STRIP_LX_%02d", i+1);
    strip_detector_lower_x_[i] = new Generic_Channel(name, i+85, "/D");
    snprintf(name, sizeof(name), "STRIP_LY_%02d", i+1);
    strip_detector_lower_y_[i] = new Generic_Channel(name, i+125, "/D");
  }

  // TDCs
  tdc_scint_top_     = new TDC_Channel("TDC_SCINT_TOP"    , 201, "/v", false);
  tdc_scint_bottom_  = new TDC_Channel("TDC_SCINT_BOTTOM" , 202, "/v", false);
  tdc_ion_mcp_       = new TDC_Channel("TDC_ION_MCP"      , 203, "/v", false);
  tdc_electron_mcp_  = new TDC_Channel("TDC_ELECTRON_MCP" , 203, "/v", false);
  tdc_photo_diode_   = new TDC_Channel("TDC_PHOTO_DIODE"  , 204, "/v", false);
  tdc_long_electron_ = new TDC_Channel("TDC_LONG_ELECTRON", 205, "/v", false);
  tdc_nimio32_trigger_ = new TDC_Channel("TDC_NIMIO32_TRIGGER", 205, "/v",
                                         false);
  tdc_tdc_stop_= new TDC_Channel("TDC_TDC_STOP", 205, "/v", false);

  // Event generator and other Geant-only data
  electron_kinetic_energy_generated_ = new Generic_Channel("T_GEN_ELE", 301,
                                                           "/D");
  electron_mu_generated_ = new Generic_Channel("MU_GEN_ELE", 302, "/D");
  recoil_mu_generated_   = new Generic_Channel("MU_GEN_RECOIL", 302, "/D");
  recoil_mcp_particle_   = new Generic_Channel("ION_MCP_PARTICLE_PDG",
                                               303, "/D");
  recoil_charge_state_   = new Generic_Channel("ION_CHARGE", 304, "/D");
  upper_pmt_particle_    = new Generic_Channel("UPPER_SCINTILLATOR_PDG",
                                               305, "/D");
  lower_pmt_particle_    = new Generic_Channel("LOWER_SCINTILLATOR_PDG",
                                               305, "/D");
  num_hits_r_mcp_        = new Generic_Channel("RECOIL_MCP_N_HITS", 306, "/D");
  num_hits_e_mcp_        = new Generic_Channel("ELECTRON_MCP_N_HITS", 307,
                                               "/D");
  electron_mcp_particle_ = new Generic_Channel("ELECTRON_MCP_PARTICLE_PDG", 303,
                                               "/D");
  // Information to match analyzer (not really simulated)
  run_action_       = new Generic_Channel("Run_Number"      , 401, "/D");
  tnim_op_beam_     = new Generic_Channel("TNIM_OP_Beam"    , 402, "/l");
  ttlbit_sigmaplus_ = new Generic_Channel("TTLBit_SigmaPlus", 403, "/D");
  ttlbit_op_beam_   = new Generic_Channel("TTLBit_OPBeam"   , 404, "/D");

  the_aggregator_ = 0;
}

//----------------------------------

K37RunAction::~K37RunAction() {
  delete runMessenger;

  delete qdc_upper_pmt;
  delete qdc_lower_pmt;
  delete qdc_upper_pmt_ae;
  delete qdc_lower_pmt_ae;
  delete qdc_upper_pmt_p;
  delete qdc_lower_pmt_p;
  delete qdc_upper_pmt_e;
  delete qdc_lower_pmt_e;
  delete qdc_upper_pmt_g;
  delete qdc_lower_pmt_g;

  delete qdc_upper_dssd;
  delete qdc_lower_dssd;
  delete qdc_upper_dssd_ae;
  delete qdc_lower_dssd_ae;
  delete qdc_upper_dssd_p;
  delete qdc_lower_dssd_p;
  delete qdc_upper_dssd_e;
  delete qdc_lower_dssd_e;
  delete qdc_upper_dssd_g;
  delete qdc_lower_dssd_g;

  delete dl_x_pos_;
  delete dl_z_pos_;
  for (G4int i = 0; i < 40; i++) {
    delete strip_detector_upper_x_[i];
    delete strip_detector_upper_y_[i];
    delete strip_detector_lower_x_[i];
    delete strip_detector_lower_y_[i];
  }
  delete tdc_scint_top_;
  delete tdc_scint_bottom_;
  delete tdc_ion_mcp_;
  delete tdc_electron_mcp_;
  delete tdc_photo_diode_;
  delete tdc_long_electron_;
  delete tdc_nimio32_trigger_;
  delete tdc_tdc_stop_;
  delete electron_kinetic_energy_generated_;
  delete electron_mu_generated_;
  delete recoil_mu_generated_;
  delete run_action_;
  delete recoil_mcp_particle_;
  delete tnim_op_beam_;
  delete ttlbit_sigmaplus_;
  delete recoil_charge_state_;
  delete ttlbit_op_beam_;
  delete upper_pmt_particle_;
  delete lower_pmt_particle_;
  delete num_hits_r_mcp_;
  delete num_hits_e_mcp_;
  delete electron_mcp_particle_;
  delete upper_dssd_parentID_;
  delete upper_dssd_boundary_status_;
  delete upper_dssd_theta_;
  delete lower_dssd_parentID_;
  delete lower_dssd_boundary_status_;
  delete lower_dssd_theta_;
}

//----------------------------------

void K37RunAction::BeginOfRunAction(const G4Run* aRun) {
  // Finish setup of electric field
  NbofEvents = 0;

  accepted = 0;
  accepted_bs = 0;
  bs = 0;
  falsePositive = 0;
  falsePositiveWithSili = 0;
  falsePositiveWithDedx = 0;
  falsePositiveWithBoth = 0;

  acceptedWithSili2 = 0;
  acceptedWithDedx2 = 0;
  acceptedWithNone = 0;
  acceptedWithNoneSecondaryFiredDedx = 0;
  acceptedWithNoneSecondaryFiredSili = 0;
  acceptedWithNonePrimaryFiredBoth = 0;
  acceptedWithNoneSecondaryFiredBoth = 0;

  falsePositiveWithNone = 0;
  falsePositiveWithNoneSecondaryFiredDedx = 0;
  falsePositiveWithNoneSecondaryFiredSili = 0;
  falsePositiveWithNonePrimaryFiredBoth = 0;
  falsePositiveWithNoneSecondaryFiredBoth = 0;

  acceptedPrimaryScatteredOffHoops  =  0;

  vetoed = 0;
  nonVetoed = 0;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  ///////////////////////// My new stuff //////////////////
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in K37Analysis.hh

  // Open an output file
  //
  // G4String fileName = "K37";
  FILE *io_file;
  char data_type[80], out_file[80];
  io_file = fopen(GetConfigurationFileName(), "r");
  int result = fscanf(io_file, "%s %s", data_type, out_file);
  if (result != 2) {
     G4cout << "WARNING PROBLEMS READING CONFIG FILE" << G4endl;
  }
  G4cout << "Opening file: " << out_file << G4endl;
  // Analysis Version 3! Spencer's Aggregator---should be IDENTICAL to what we
  // get from the analyzer.
  // the_aggregator_ = new Aggregator();

  RegisterChannel(qdc_upper_pmt);
  RegisterChannel(qdc_lower_pmt);
  RegisterChannel(qdc_upper_pmt_ae);
  RegisterChannel(qdc_lower_pmt_ae);
  RegisterChannel(qdc_upper_pmt_p);
  RegisterChannel(qdc_lower_pmt_p);
  RegisterChannel(qdc_upper_pmt_e);
  RegisterChannel(qdc_lower_pmt_e);
  RegisterChannel(qdc_upper_pmt_g);
  RegisterChannel(qdc_lower_pmt_g);
  RegisterChannel(qdc_upper_dssd);
  RegisterChannel(qdc_lower_dssd);
  RegisterChannel(qdc_upper_dssd_ae);
  RegisterChannel(qdc_lower_dssd_ae);
  RegisterChannel(qdc_upper_dssd_p);
  RegisterChannel(qdc_lower_dssd_p);
  RegisterChannel(qdc_upper_dssd_e);
  RegisterChannel(qdc_lower_dssd_e);
  RegisterChannel(qdc_upper_dssd_g);
  RegisterChannel(qdc_lower_dssd_g);
  if (detector_construction_ -> GetMakeRecoilMCP()) {
    RegisterChannel(dl_x_pos_);
    RegisterChannel(dl_z_pos_);
    RegisterChannel(tdc_ion_mcp_);
    RegisterChannel(recoil_mcp_particle_);
    RegisterChannel(num_hits_r_mcp_);
  }
  if (record_strip_detector_data_) {
    for (G4int i = 0; i < 40; i++) {
      RegisterChannel(strip_detector_upper_x_[i]);
      RegisterChannel(strip_detector_upper_y_[i]);
      RegisterChannel(strip_detector_lower_x_[i]);
      RegisterChannel(strip_detector_lower_y_[i]);
    }
  }
  RegisterChannel(tdc_scint_top_);
  RegisterChannel(tdc_scint_bottom_);
  if (detector_construction_ -> GetMakeElectronMCP()) {
    RegisterChannel(tdc_electron_mcp_);
    RegisterChannel(num_hits_e_mcp_);
    RegisterChannel(electron_mcp_particle_);
  }
  RegisterChannel(tdc_photo_diode_);
  RegisterChannel(electron_kinetic_energy_generated_);
  RegisterChannel(electron_mu_generated_);
  RegisterChannel(recoil_mu_generated_);
  RegisterChannel(run_action_);
  RegisterChannel(tdc_long_electron_);
  RegisterChannel(tdc_nimio32_trigger_);
  RegisterChannel(tdc_tdc_stop_);

  RegisterChannel(tnim_op_beam_);
  RegisterChannel(ttlbit_sigmaplus_);
  RegisterChannel(recoil_charge_state_);
  RegisterChannel(ttlbit_op_beam_);
  RegisterChannel(upper_pmt_particle_);
  RegisterChannel(lower_pmt_particle_);
  RegisterChannel(upper_dssd_parentID_);
  RegisterChannel(upper_dssd_boundary_status_);
  RegisterChannel(upper_dssd_theta_);
  RegisterChannel(lower_dssd_parentID_);
  RegisterChannel(lower_dssd_boundary_status_);
  RegisterChannel(lower_dssd_theta_);

  the_aggregator_ -> RegisterIOMethod(configuration_filename_);
  // the_aggregator_ -> RegisterIOMethod("ScreenIO.mac");
  the_aggregator_ -> BeginRun();

  // test_channel_ -> InsertData(10.0);
  // the_aggregator_ -> EndEvent();

  // test_channel_ -> InsertData(15.0);
  // the_aggregator_ -> EndEvent();

  // test_channel_ -> InsertData(13.0);
  // the_aggregator_ -> EndEvent();
}

void K37RunAction::EndOfRunAction(const G4Run* aRun) {
  NbofEvents = aRun->GetNumberOfEvent();

  this->PrintResultsToScreen();
  this->PrintResultsToRunstat();

  // JTW_Event *mytester = new JTW_Event();
  // FILE *f = fopen("RecheckA_beta.dat", "a");
  // if (!f) G4cout << "COULDN'T OPEN FILE" << G4endl;
  // G4int numIter = 25000;
  // for (G4int j = 0; j < numIter; j++) {
  //   G4int power = 2;
  //   for (G4int i = 0; i < 2*pow(10, power); i++) {
  //     mytester -> MakeEvent();
  //         if (i %  static_cast<int>(pow(10, power-1)) == 0) {
  //           // G4cout << "Gen " << i << G4endl;
  //         }
  //   }
  //   // G4cout << "Writing to file..." << G4endl;
  //   // G4cout << mytester -> GetNumPlus() << "\t" << mytester -> GetNumMins()
  //   //        << G4endl;
  //   fprintf(f, "%d\t%d\n", mytester -> GetNumPlus(),
  //           mytester -> GetNumMins());
  //   mytester -> ResetGeneratedCounters();
  //   if (j % 1000 == 0) {
  //     G4cout << j << G4endl;
  //   }
  // }

  // fclose(f);
  // G4EmCalculator emCalculator;
  // emCalculator.SetVerbose(1);
  // G4ParticleDefinition* particle = kinematic -> GetParticleGun() ->
  //   GetParticleDefinition();


  the_aggregator_ -> EndRun();
  G4cout << "Last line of EORA" << G4endl;
}

void K37RunAction::PrintEnergyLossTable(G4String materialChoice) {
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4bool materialIsOnTheList = false;
  // search the material by its name
  G4Material*materialSelectedFromTable;
  for (size_t J = 0 ; J < theMaterialTable -> size(); J++) {
    materialSelectedFromTable = (*theMaterialTable)[J];
    if (materialSelectedFromTable ->GetName() == materialChoice) {
      G4cout << materialSelectedFromTable -> GetName()
             << " is on the list of availble materials" << G4endl;
      G4cout << "The energy loss table of e+ in "
             << materialSelectedFromTable -> GetName()
             << " will be calculated" << G4endl;
      materialIsOnTheList = true;
      break;
    }
  }

  if (materialIsOnTheList == false) {
    G4cerr << "Material was not on list" << G4endl;
    return;
  }

  G4EmCalculator emCalculator;
  G4double test_energies[800];
  for (G4int i = 0; i < 800; i++) {
    test_energies[i]=G4double(i)*0.005;
  }

  std::ofstream energyLossTableFile;
  energyLossTableFile.open("energyLossTableFile.txt",
                           std::ofstream::out | std::ofstream::trunc);

  energyLossTableFile << "Energy(MeV)" << "    " << "DEDX(MeV/cm)" << G4endl;
  for (G4int i = 0; i < 800; i++) {
    energyLossTableFile << std::setw(15) << std::left << (test_energies[i])/MeV
                        << std::setw(15)
                        <<(emCalculator.GetDEDX((test_energies[i])*MeV,
                                                "e+",
                                                materialSelectedFromTable ->
                                                GetName(),
                                                "DefaultRegionForTheWorld"))/
      (MeV/cm) << G4endl;
  }
  energyLossTableFile.close();
  G4cout<< "energyLossTableFile.txt written." << G4endl;
}


void K37RunAction::PrintResultsToScreen() {
  G4cout << "------------------------------------------------------" << G4endl;
  G4cout << "  # Events ___________: " << NbofEvents << G4endl;
  G4cout << "  # Accepted _________: " << accepted<< G4endl;
  G4cout << "------------------------------------------------------" << G4endl;

  // G4cout << "  # ABSoffHoop _______: "
  //        << acceptedPrimaryScatteredOffHoops<< G4endl;
  // G4cout << "  # Acc/Eve __________: "
  //        << (G4double(acceptedWithNone)/G4double(NbofEvents)) << G4endl;
  // G4cout << "  # NV _______________: " << nonVetoed<< G4endl;
  // G4cout << "  # NV/Eve ___________: "
  //        << (G4double(nonVetoed)/G4double(NbofEvents)) << G4endl;
  // G4cout << "  # FalsePositive ____: " << falsePositiveWithNone << G4endl;
  // G4cout << "  # Acc With Sili2 ___: " << acceptedWithSili2 << G4endl;
  // G4cout << "  # Acc With Dedx2 ___: " << acceptedWithDedx2 << G4endl;
  // G4cout << "  # Sec Fired Dedx__: " << secondaryInDedx << G4endl;
  // G4cout << "  # Sec Fired Both__: " << secondaryInBoth << G4endl;
  // G4cout << "  # FP With Sili _____: " << falsePositiveWithSili << G4endl;
  // G4cout << "  # FP With Dedx _____: " << falsePositiveWithDedx << G4endl;
  // G4cout << "  # Fired All 1 & 2 __: " << falsePositiveWithBoth << G4endl;
  // G4cout << "  # BackScatter ______: " << bs << G4endl;
  // G4cout << "  # NV BS ____________: " << accepted_bs << G4endl;
  // G4cout << "  # FP With None ___: " << falsePositiveWithNone << G4endl;
  // G4cout << "  # BS/Eve _________: " << (G4double(bs)/G4double(NbofEvents))
  //        << G4endl;
  // G4cout << "  # ABS/Eve ________: "
  //        << (G4double(accepted_bs)/G4double(NbofEvents)) << G4endl;
  // G4cout << "  # ABS/BS _________: " << (G4double(accepted_bs)/G4double(bs))
  //        << G4endl;

  // G4cout << "----------------------------------------------------" << G4endl;
  // G4cout << "  # Accepted With No 2 _______________________: "
  //        << acceptedWithNone<< G4endl;
  // G4cout << "  # Accepted With No 2 Vetoed_________________: "
  //        << vetoed<< G4endl;
  // G4cout << "  # Accepted With No 2 Non-Vetoed_____________: "
  //        << nonVetoed<< G4endl;
  // G4cout << "  # Accepted With No 2 Secondary Fired Dedx __: "
  //        << acceptedWithNoneSecondaryFiredDedx<< G4endl;
  // G4cout << "  # Accepted With No 2 Secondary Fired Sili __: "
  //        << acceptedWithNoneSecondaryFiredSili<< G4endl;
  // G4cout << "  # Accepted With No 2 Secondary Fired Both __: "
  //        << acceptedWithNoneSecondaryFiredBoth<< G4endl;
  // G4cout << "  # Accepted With No 2 Primary Fired Both ____: "
  //        << acceptedWithNonePrimaryFiredBoth<< G4endl;
  // G4cout << "----------------------------------------------------" << G4endl;
  // G4cout << "  # FP With No 1 ________________________: "
  //        << falsePositiveWithNone<< G4endl;
  // G4cout << "  # FP With No 1 Secondary Fired Dedx2 __: "
  //        << falsePositiveWithNoneSecondaryFiredDedx<< G4endl;
  // G4cout << "  # FP With No 1 Secondary Fired Sili2 __: "
  //        << falsePositiveWithNoneSecondaryFiredSili<< G4endl;
  // G4cout << "  # FP With No 1 Secondary Fired Both2 __: "
  //        << falsePositiveWithNonePrimaryFiredBoth<< G4endl;
  // G4cout << "  # FP With No 1 Primary Fired Both2 ____: "
  //        << falsePositiveWithNoneSecondaryFiredBoth<< G4endl;
  // G4cout << "----------------------------------------------------" << G4endl;
  // G4cout << "  # Accepted With Dedx2 _________________________________: "
  //        << acceptedWithDedx2 << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired Dedx ____________: "
  //        << acceptedWithDedx2SecondaryFiredDedx << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired Sili ____________: "
  //        << acceptedWithDedx2SecondaryFiredSili << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired Dedx2 ___________: "
  //        << acceptedWithDedx2SecondaryFiredDedx2 << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired Sili And Dedx2 __: "
  //        << acceptedWithDedx2SecondaryFiredSiliAndDedx2 << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired Dedx And Sili ___: "
  //        << acceptedWithDedx2SecondaryFiredDedxAndSili << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired Dedx And Dedx2 __: "
  //        << acceptedWithDedx2SecondaryFiredDedxAndDedx2 << G4endl;
  // G4cout << "  # Accepted With Dedx2 Secondary Fired All _____________: "
  //        << acceptedWithDedx2SecondaryFiredAll << G4endl;
  // G4cout << "  # Accepted With Dedx2 Primary Fired All _______________: "
  //        << acceptedWithDedx2SPrimaryFiredAll << G4endl;
  // G4cout << "-------------------------------" << G4endl;
}

void K37RunAction::RegisterChannel(K37_Data* channel) {
  (*active_channels_)[channel->GetName()] = channel;
  the_aggregator_ -> RegisterData(channel);
}

void K37RunAction::PrintResultsToRunstat() {
  // std::ofstream runstat;
  // runstat.open("runstat.txt", std::ofstream::out | std::ofstream::app);
  // runstat << "--------------------------------------------------------"
  //         << G4endl;
  // runstat << "  # Events ___________: " << NbofEvents  << G4endl;
  // runstat << "  # Accepted _________: " << accepted << G4endl;
  // runstat << "--------------------------------------------------------"
  //         << G4endl;
  // runstat.close();


  // runstat << "  # ABSoffHoop _______: " << acceptedPrimaryScatteredOffHoops
  //         << G4endl;
  // runstat << "  # Events _______: " << NbofEvents << G4endl;
  // runstat << "  # Accepted _____: " << accepted << G4endl;
  // runstat << "  # Acc/Eve ______: "
  //         << (G4double(accepted)/G4double(NbofEvents)) << G4endl;
  // runstat << "  # Sec Fired SiLi: " << secondaryInSiLi << G4endl;
  // runstat << "  # Sec Fired Dedx: " << secondaryInDedx << G4endl;
  // runstat << "  # Sec Fired Both: " << secondaryInBoth << G4endl;
  // runstat << "  # BackScatter __: " << bs << G4endl;
  // runstat << "  # FalsePositive_: " << falsePositive  << G4endl;
  // runstat << "  # BS/Eve _______: " << (G4double(bs)/G4double(NbofEvents))
  //         << G4endl;
  // runstat << "  # Accepted BS __: " << accepted_bs << G4endl;
  // runstat << "  # ABS/Eve ______: "
  //         << (G4double(accepted_bs)/G4double(NbofEvents)) << G4endl;
  // runstat << "  # ABS/BS _______: " << (G4double(accepted_bs)/G4double(bs))
  //         << G4endl;
  // runstat << "  # Acc/Eve __________: "
  //         << (G4double(acceptedWithNone)/G4double(NbofEvents)) << G4endl;
  // runstat << "  # NV _______________: " << nonVetoed << G4endl;
  // runstat << "  # NV/Eve ___________: "
  //         << (G4double(nonVetoed)/G4double(NbofEvents))  << G4endl;
  // runstat << "  # FalsePositive ____: " << falsePositiveWithNone  << G4endl;
  // runstat << "  # Acc With Sili2 ___: " << acceptedWithSili2  << G4endl;
  // runstat << "  # Acc With Dedx2 ___: " << acceptedWithDedx2  << G4endl;
  // runstat << "  # FP With Sili _____: " << falsePositiveWithSili  << G4endl;
  // runstat << "  # FP With Dedx _____: " << falsePositiveWithDedx  << G4endl;
  // runstat << "  # Fired All 1 & 2 __: " << falsePositiveWithBoth  << G4endl;
  // runstat << "  # BackScatter ______: " << bs  << G4endl;
  // runstat << "  # NV BS ____________: " << accepted_bs  << G4endl;
  // runstat << "--------------------------------------------------------"
  //         << G4endl;
  // runstat << "  # Accepted With No 2 _______________________: "
  //         << acceptedWithNone << G4endl;
  // runstat << "  # Accepted With No 2 Vetoed_________________: " << vetoed
  //         << G4endl;
  // runstat << "  # Accepted With No 2 Non-Vetoed_____________: " << nonVetoed
  //         << G4endl;
  // runstat << "  # Accepted With No 2 Secondary Fired Dedx __: "
  //         << acceptedWithNoneSecondaryFiredDedx << G4endl;
  // runstat << "  # Accepted With No 2 Secondary Fired Sili __: "
  //         << acceptedWithNoneSecondaryFiredSili << G4endl;
  // runstat << "  # Accepted With No 2 Secondary Fired Both __: "
  //         << acceptedWithNoneSecondaryFiredBoth << G4endl;
  // runstat << "  # Accepted With No 2 Primary Fired Both ____: "
  //         << acceptedWithNonePrimaryFiredBoth << G4endl;
  // runstat << "--------------------------------------------------------"
  //         << G4endl;
  // runstat << "  # FP With No 1 ________________________: "
  //         << falsePositiveWithNone << G4endl;
  // runstat << "  # FP With No 1 Secondary Fired Dedx2 __: "
  //         << falsePositiveWithNoneSecondaryFiredDedx << G4endl;
  // runstat << "  # FP With No 1 Secondary Fired Sili2 __: "
  //         << falsePositiveWithNoneSecondaryFiredSili << G4endl;
  // runstat << "  # FP With No 1 Secondary Fired Both2 __: "
  //         << falsePositiveWithNonePrimaryFiredBoth << G4endl;
  // runstat << "  # FP With No 1 Primary Fired Both2 ____: "
  //         << falsePositiveWithNoneSecondaryFiredBoth << G4endl;
  // runstat << "--------------------------------------------------------"
  //         << G4endl;
}
