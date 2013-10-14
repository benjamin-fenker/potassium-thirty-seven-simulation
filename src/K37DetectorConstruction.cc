// Authors: Spencer Behling and Benjamin Fenker 2013

// CLHEP files
#include <CLHEP/Vector/Rotation.h>

// Geant Files
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4EllipticalTube.hh"
#include "G4FieldManager.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4NistManager.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDParticleFilter.hh"
#include "G4SolidStore.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4TransportationManager.hh"
#include "G4Trd.hh"
#include "G4UnionSolid.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4VSDFilter.hh"
#include "G4VisAttributes.hh"

// Our Files
#include "K37DetectorConstruction.hh"
#include "K37DetectorMessenger.hh"
#include "K37ElectricFieldSetup.hh"

K37DetectorConstruction::K37DetectorConstruction()
    : world_material_(0), world_box_(0), world_log_(0), world_phys_(0),
      scintillator_tubs_(0), upper_scintillator_log_(0),
      upper_scintillator_phys_(0), upper_scintillator_sens_(0),
      lower_scintillator_log_(0), lower_scintillator_phys_(0),
      lower_scintillator_sens_(0), strip_detector_box_(0),
      upper_strip_detector_log_(0), upper_strip_detector_phys_(0),
      upper_strip_detector_sens_(0), lower_strip_detector_log_(0),
      lower_strip_detector_phys_(0), lower_strip_detector_sens_(0),
      chamber_box_(0), chamber_log_(0), chamber_phys_(0), electron_mcp_tub_(0),
      electron_mcp_log_(0), electron_mcp_phys_(0), electron_mcp_sens_(0),
      recoil_mcp_tub_(0), recoil_mcp_log_(0), recoil_mcp_phys_(0),
      recoil_mcp_sens_(0), detectorMessenger(0), mirror_log(0),
      MirrorMaterial(0), FullEnergyDetectorMaterial(0), DeDxDetectorMaterial(0),
      SiliconDetectorFrameMaterial(0), ChamberMaterial(0), FoilMaterial(0),
      HoopMaterial(0), MirrorMountMaterial(0), CoilsMaterial(0),
      Hoop7Material(0), MCPMaterial(0), MM_CollimatorCut_sol(0),
      shouldTheMirrorBeWFEDMCut(false), changeZtoX(0), changeZto45(0),
      changeZtoNeg45(0), changeZto35(0), changeZtoNeg35(0), changeZtoNeg62(0),
      changeZtoNeg118(0), changeYtoBeamAxis(0), changeYtoBeamAxisForLPP1(0),
      changeYtoBeamAxisForLPP2(0), changeYtoBeamAxisForLPP3(0),
      changeYtoBeamAxisForLPP4(0), rotationForOpticalPumpingBeams1(0),
      rotationForOpticalPumpingBeams2(0), RFDRotation(0), FFRFRotation(0),
      MirrorRotation(0), MirrorCutRotation(0), MMRotation(0), hoopRotation(0),
      dedxFrame_logVisAttributes(0),
      chamber_logVisAttributes(0), OPRF_logVisAttributes(0),
      RFD_logVisAttributes(0), beryllium_logVisAttributes(0),
      FFRF_logVisAttributes(0), mirror_logVisAttributes(0),
      MM_logVisAttributes(0), cut_ESP_logVisAttributes(0),
      hoop7_logVisAttributes(0), SOED_logVisAttributes(0),
      coils_logVisAttributes(0), rmcp_logVisAttributes_(0) {

  // Default values
  world_size_ = 2.0 * m;


  this-> DefineMaterials();
  detectorMessenger = new K37DetectorMessenger(this);
  changeZtoX = new CLHEP::HepRotation();
  changeZtoX->rotateX(90.*deg);
  // ******SET CONSTRUCTION FLAGS HERE**************************************
    makeScintillators = true;     // Must be true!
    makeStripDetectors = true;    // Must be true!
    makeChamber = true;
    makeMirrors = true;
    makeHoops = true;
    makeElectronMCP = true;
    makeCoils = true;
    make_r_mcp_ = true;
}

K37DetectorConstruction::~K37DetectorConstruction() {
    delete detectorMessenger;
    delete MM_CollimatorCut_sol;
    delete changeZtoX;
    delete changeZto45;
    delete changeZtoNeg45;
    delete changeZto35;
    delete changeZtoNeg35;
    delete changeZtoNeg62;
    delete changeZtoNeg118;
    delete changeYtoBeamAxis;
    delete changeYtoBeamAxisForLPP1;
    delete changeYtoBeamAxisForLPP2;
    delete changeYtoBeamAxisForLPP3;
    delete changeYtoBeamAxisForLPP4;
    delete rotationForOpticalPumpingBeams1;
    delete rotationForOpticalPumpingBeams2;
    delete RFDRotation;
    delete FFRFRotation;
    delete MirrorRotation;
    delete MirrorCutRotation;
    delete MMRotation;
    delete hoopRotation;
    delete world_log_;
    delete world_phys_;
    delete scintillator_tubs_;
    delete upper_scintillator_log_;
    delete lower_scintillator_log_;
    delete strip_detector_box_;
    delete upper_strip_detector_log_;
    delete lower_strip_detector_log_;
    delete chamber_box_;
    delete chamber_log_;
    delete electron_mcp_tub_;
    delete electron_mcp_log_;
    delete recoil_mcp_tub_;
    delete recoil_mcp_log_;
    delete dedxFrame_logVisAttributes;
    delete chamber_logVisAttributes;
    delete OPRF_logVisAttributes;
    delete RFD_logVisAttributes;
    delete beryllium_logVisAttributes;
    delete FFRF_logVisAttributes;
    delete mirror_logVisAttributes;
    delete MM_logVisAttributes;
    delete cut_ESP_logVisAttributes;
    delete hoop7_logVisAttributes;
    delete SOED_logVisAttributes;
    delete coils_logVisAttributes;
    delete rmcp_logVisAttributes_;
}

G4VPhysicalVolume* K37DetectorConstruction::Construct() {
    return this->ConstructK37Experiment();
}
G4VPhysicalVolume* K37DetectorConstruction:: ConstructK37Experiment() {
  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
    // ------------------------------------------ Sensitive Detector / filters

    /*------------------------------------------------------------------------
      The Sensitivity of each volume will be set at the time the volume is
      created. This will lead to what I believe to be a better organization of
      the code because every aspect of a volume will no be localized so that
      volumes can be added and subtracted more easily.
      ------------------------------------------------------------------------*/
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    // G4double world_x = 1.0*m;
    // G4double world_y = 1.0*m;
    // G4double world_z = 1.0*m;

    world_box_ = new G4Box("world_box", world_size_/2.0,
                           world_size_/2.0, world_size_/2.0);
    world_log_  = new G4LogicalVolume(world_box_, world_material_, "world_log",
                                     0, 0, 0);
    world_phys_ = new G4PVPlacement(0, G4ThreeVector(), world_log_,
                                    "world_phys", 0, false, 0);

    // G4VisAttributes(false) means invisible. It is better than using the
    // convience function G4VisAttributes::Invisble becuse it produces
    // a non const pointer that can later be delete avoiding a memory
    // leak.
    G4VisAttributes *world_logVisAttributes = new G4VisAttributes(false);
    // world_logVisAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0));
    world_log_ -> SetVisAttributes(world_logVisAttributes);

    if (makeScintillators) ConstructScintillators(SDman);
    if (makeStripDetectors) ConstructStripDetectors(SDman);
    if (makeChamber) ConstructChamber();
    if (makeMirrors) ConstructMirrors();
    if (makeHoops) ConstructHoops();
    if (makeElectronMCP) ConstructElectronMCP(SDman);
    if (makeCoils) ConstructCoils();
    if (make_r_mcp_) ConstructRecoilMCP(SDman);
    return world_phys_;
}


void K37DetectorConstruction::ConstructScintillators(G4SDManager* SDman) {
  G4double Scint_rmax             = 45*mm;  // 90/2
  G4double Scint_rmin             = 0.*mm;
  G4double Scint_dz               = 35*mm;
  G4double Scint_startPhi = 0.*deg;
  G4double Scint_deltaPhi = 360.*deg;
  G4double Scint_zPosition= 117.5*mm;

  // if (scintillator_tubs_) delete scintillator_tubs_;
  // if (upper_scintillator_log_) delete upper_scintillator_log_;
  // if (upper_scintillator_phys_) delete upper_scintillator_phys_;
  // if (lower_scintillator_log_) delete lower_scintillator_log_;
  // if (lower_scintillator_phys_) delete lower_scintillator_phys_;

  // Use same solid for both detectors :-)
  scintillator_tubs_ = new G4Tubs("scint_sol", Scint_rmin, Scint_rmax,
                                  (Scint_dz/2.)*mm, Scint_startPhi,
                                  Scint_deltaPhi);
  // Use same vis attributes for both detectors :-)
  G4VisAttributes *scintillator_vis = new G4VisAttributes(G4Colour(0.0, 0.0,
                                                                   1.0, 1.0));
  scintillator_vis -> SetForceSolid(true);

  upper_scintillator_log_ =  new G4LogicalVolume(scintillator_tubs_,
                                                 FullEnergyDetectorMaterial,
                                                 "scint_plusZ_log", 0, 0, 0);
  upper_scintillator_phys_ =
      new G4PVPlacement(0, G4ThreeVector(0., 0., Scint_zPosition),
                        upper_scintillator_log_, "scint_plusZ_phys", world_log_,
                        false, 0);
  upper_scintillator_log_ -> SetVisAttributes(scintillator_vis);

  lower_scintillator_log_ = new G4LogicalVolume(scintillator_tubs_,
                                                FullEnergyDetectorMaterial,
                                                "scint_minusZ_log", 0, 0, 0);
  lower_scintillator_phys_ =
      new G4PVPlacement(0, G4ThreeVector(0., 0., -Scint_zPosition),
                       lower_scintillator_log_, "scint_minusZ_phys", world_log_,
                        false, 0);
  lower_scintillator_log_ -> SetVisAttributes(scintillator_vis);

  // Set up sensitive detectors
  G4String fullenergy1SDname = "/mydet/scintillatorPlusZ";
  if (upper_scintillator_sens_) {
    G4cout << "Deleting upper scintillator..." << G4endl;
    delete upper_scintillator_sens_;
  }
  upper_scintillator_sens_ = new K37ScintillatorSD(fullenergy1SDname);
  SDman->AddNewDetector(upper_scintillator_sens_);
  upper_scintillator_log_ -> SetSensitiveDetector(upper_scintillator_sens_);

  G4String fullenergy2SDname = "/mydet/scintillatorMinusZ";
  if (lower_scintillator_sens_) {
    G4cout << "Deleting lower scintillator..." << G4endl;
    delete lower_scintillator_sens_;
  }
  lower_scintillator_sens_ = new K37ScintillatorSD(fullenergy2SDname);
  SDman->AddNewDetector(lower_scintillator_sens_);
  lower_scintillator_log_ -> SetSensitiveDetector(lower_scintillator_sens_);
}

void K37DetectorConstruction::ConstructStripDetectors(G4SDManager* SDman) {
  G4double Dedx_x = 40.0*mm;
  G4double Dedx_y = 40.0*mm;
  G4double Dedx_z = 0.3*mm;

  G4double Dedx_zPosition = 98.5*mm;

  // Same solid for both detectors :-)
  strip_detector_box_ = new G4Box("dedx_sol", (Dedx_x/2.),
                                  (Dedx_y/2.), (Dedx_z/2.));
  // Same vis for both detectors :-)
  G4VisAttributes *strip_detector_vis = new G4VisAttributes(G4Colour(1.0,
                                                                     1.0, 1.0));
  strip_detector_vis -> SetForceSolid(true);
  G4ThreeVector dedx_PlusZ_pos = G4ThreeVector(0.0, 0.0, Dedx_zPosition);
  upper_strip_detector_log_ = new G4LogicalVolume(strip_detector_box_,
                                                  DeDxDetectorMaterial,
                                                  "dedx_plusZ_log", 0, 0, 0);
  upper_strip_detector_log_ -> SetVisAttributes(strip_detector_vis);
  upper_strip_detector_phys_ = new G4PVPlacement(0, dedx_PlusZ_pos,
                                                 upper_strip_detector_log_,
                                                 "dedx_plusZ_phys",
                                                 world_log_, false, 0);



  G4ThreeVector dedx_MinusZ_pos = G4ThreeVector(0.0, 0.0, -Dedx_zPosition);
  lower_strip_detector_log_ = new G4LogicalVolume(strip_detector_box_,
                                                  DeDxDetectorMaterial,
                                                  "dedx_MinusZ_log", 0,
                                                  0, 0);
  lower_strip_detector_log_ -> SetVisAttributes(strip_detector_vis);
  lower_strip_detector_phys_ = new G4PVPlacement(0, dedx_MinusZ_pos,
                                                 lower_strip_detector_log_,
                                                 "dedx_minusZ_phys", world_log_,
                                                 false, 0);


  G4String dedx1SDname = "/mydet/dsssdPlusZ";
  if (upper_strip_detector_sens_) {
    G4cout << "Deleting upper strip detector..." << G4endl;
    delete upper_strip_detector_sens_;
  }
  upper_strip_detector_sens_ = new K37StripDetectorSD(dedx1SDname);
  //                        pos_of_center, numStrips, stripWidth
  upper_strip_detector_sens_ -> SetupParameters(dedx_PlusZ_pos, 40, 1.0*mm);
  SDman->AddNewDetector(upper_strip_detector_sens_);
  upper_strip_detector_log_->SetSensitiveDetector(upper_strip_detector_sens_);

  G4String dedx2SDname = "/mydet/dsssdMinusZ";
  if (lower_strip_detector_sens_) {
    G4cout << "Deleting lower strip detector..." << G4endl;
    delete lower_strip_detector_sens_;
  }
  lower_strip_detector_sens_ = new K37StripDetectorSD(dedx2SDname);
  //                        pos_of_center, numStrips, stripWidth
  lower_strip_detector_sens_ -> SetupParameters(dedx_MinusZ_pos, 40, 1.0*mm);
  SDman->AddNewDetector(lower_strip_detector_sens_);
  upper_strip_detector_log_->SetSensitiveDetector(lower_strip_detector_sens_);

  // ------------------------------ dedx mount
  G4double strip_detector_frame_X  = 44.4*mm;

  G4VSolid * dedx_holder_sol = new G4Box("dedx_holder_sol",
                                         strip_detector_frame_X/2.0,
                                         strip_detector_frame_X/2.0,
                                         1.5*mm);
  G4VSolid * dedx_holder_cut_sol = new G4Box("dedx_holder_cut_sol",
                                             Dedx_x/2.0, Dedx_y/2.0, 1.7*mm);
  G4SubtractionSolid* dedxFrame_sol =
    new G4SubtractionSolid("dedxFrame_sol", dedx_holder_sol,
                           dedx_holder_cut_sol);
  G4LogicalVolume* dedxFrame_log =
    new G4LogicalVolume(dedxFrame_sol, SiliconDetectorFrameMaterial,
                        "dedxFrame_log", 0, 0, 0);
  dedxFrame_logVisAttributes = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
  dedxFrame_logVisAttributes-> SetForceSolid(true);
  dedxFrame_log -> SetVisAttributes(dedxFrame_logVisAttributes);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 98.5*mm), dedxFrame_log,
                    "dedxFrame_plusZ_phys", world_log_, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -98.5*mm), dedxFrame_log,
                    "dedxFrame_minusZ_phys", world_log_, false, 0);
}  // End construct stip detectors

void K37DetectorConstruction::ConstructChamber() {
  G4double chamberBlock_x = 558.8*mm;
  G4double chamberBlock_y = 457.2*mm;
  G4double chamberBlock_z = 508.0*mm;

  // Trap center will be at (0,0,0) positive Z is along the direction of
  // the beta detectors going up.
  // Y is going into the pumping port and X is coming out toward the
  // computers in the TRINAT lab.

  G4Box* chamberBlock_sol = new G4Box("chamberBlock_sol", (chamberBlock_x/2.),
                                      (chamberBlock_y/2.), (chamberBlock_z/2.));

  // ------------------------------ Cut for the vertical Detectror
  // Ports (CFTVDP)

  G4Tubs* CFTVDP_sol = new G4Tubs("CFTVDP_sol", 0.0, (106.68/2.0)*mm,
                                  (510.0/2.)*mm, 0.0*deg, 360.0*deg);

  G4SubtractionSolid* ChamberCut1_sol =
    new G4SubtractionSolid("ChamberCut1_sol", chamberBlock_sol, CFTVDP_sol);

  // ------------------------------ Cut for the MCPs (CFTMCPS)

  G4Tubs* CFTMCPS_sol = new G4Tubs("CFTMCPS_sol", 0.0, (298.45/2.0)*mm,
                                   (560.0/2.)*mm, 0.0*deg, 360.0*deg);
  G4ThreeVector noChange(0, 0, 0);

  G4SubtractionSolid* ChamberCut2_sol =
    new G4SubtractionSolid("ChamberCut2_sol", ChamberCut1_sol, CFTMCPS_sol,
                           changeZtoX, noChange);

  // ------------------------------ Cut for the 45 deg MOT pipes (CFTMOT)

  G4Tubs* CFTMOT1_sol = new G4Tubs("CFTMOT1_sol", 0.0, (38.10/2.0)*mm,
                                   (850.0/2.)*mm, 0.0*deg, 360.0*deg);

  //      changeZto45 = new G4RotationMatrix();
  changeZto45 = new CLHEP::HepRotation();
  changeZto45->rotateY(90.0*deg);
  changeZto45->rotateX(45.*deg);

  G4SubtractionSolid* ChamberCut3_sol =
    new G4SubtractionSolid("ChamberCut3_sol", ChamberCut2_sol, CFTMOT1_sol,
                           changeZto45, noChange);

  G4Tubs* CFTMOT2_sol = new G4Tubs("CFTMOT2_sol", 0.0, (38.10/2.0)*mm,
                                   (850.0/2.)*mm, 0.0*deg, 360.0*deg);

  //      G4RotationMatrix* changeZtoNeg45 = new G4RotationMatrix();
  changeZtoNeg45 = new CLHEP::HepRotation();
  changeZtoNeg45->rotateY(90.0*deg);
  changeZtoNeg45->rotateX(-45.0*deg);

  G4SubtractionSolid* ChamberCut4_sol =
    new G4SubtractionSolid("ChamberCut4_sol", ChamberCut3_sol, CFTMOT2_sol,
                           changeZtoNeg45, noChange);

  // ------------------------------ Cut for the small diagnostic
  // ports (CFTSDP)

  G4Tubs* CFTSDP1_sol = new G4Tubs("CFTSDP1_sol", 0.0, (38.10/2.0)*mm,
                                   (850.0/2.)*mm, 0.0*deg, 360.0*deg);

  // G4RotationMatrix* changeZto35 = new G4RotationMatrix();
  changeZto35 = new CLHEP::HepRotation();
  changeZto35->rotateY(35.0*deg);

  G4SubtractionSolid* ChamberCut5_sol =
    new G4SubtractionSolid("ChamberCut5_sol", ChamberCut4_sol, CFTSDP1_sol,
                           changeZto35, noChange);

  G4Tubs* CFTSDP2_sol = new G4Tubs("CFTSDP2_sol", 0.0, (38.10/2.0)*mm,
                                   (850.0/2.)*mm, 0.0*deg, 360.0*deg);

  // G4RotationMatrix* changeZtoNeg35 = new G4RotationMatrix();
  changeZtoNeg35 = new CLHEP::HepRotation();
  changeZtoNeg35->rotateY(-35.0*deg);

  G4SubtractionSolid* ChamberCut6_sol =
    new G4SubtractionSolid("ChamberCut6_sol", ChamberCut5_sol, CFTSDP2_sol,
                           changeZtoNeg35, noChange);

  // ------------------------------ Cut for the large diagnostic
  // ports (CFTLDP)

  G4Tubs* CFTLDP1_sol = new G4Tubs("CFTLDP1_sol", 0.0, (76.20/2.0)*mm,
                                   (450.0/2.)*mm, 0.0*deg, 360.0*deg);

  // G4RotationMatrix* changeZtoNeg62 = new G4RotationMatrix();
  changeZtoNeg62 = new CLHEP::HepRotation();
  changeZtoNeg62->rotateY(-62.0*deg);

  G4ThreeVector cutFromTrapTowardPositveY62(100*mm, 0, (.531709*100)*mm);

  G4SubtractionSolid* ChamberCut7_sol =
    new G4SubtractionSolid("ChamberCut7_sol", ChamberCut6_sol, CFTLDP1_sol,
                           changeZtoNeg62, cutFromTrapTowardPositveY62);
  // G4UnionSolid* ChamberCut7_sol =
  //   new G4UnionSolid("ChamberCut7_sol", ChamberCut6_sol, CFTLDP1_sol,
  //                    changeZtoNeg62, cutFromTrapTowardPositveY);

  G4Tubs* CFTLDP2_sol = new G4Tubs("CFTLDP2_sol", 0.0, (76.20/2.0)*mm,
                                   (450.0/2.)*mm, 0.0*deg, 360.0*deg);

  // G4RotationMatrix* changeZtoNeg118 = new G4RotationMatrix();
  changeZtoNeg118 = new CLHEP::HepRotation();
  changeZtoNeg118->rotateY(-118.0*deg);

  G4ThreeVector cutFromTrapTowardPositveY118(100*mm, 0, (-.531709*100)*mm);

  G4SubtractionSolid* ChamberCut8_sol =
    new G4SubtractionSolid("ChamberCut8_sol", ChamberCut7_sol, CFTLDP2_sol,
                           changeZtoNeg118, cutFromTrapTowardPositveY118);

  // ------------------------------ Cut for pipe from first trap (CFPFFT)

  G4Tubs* CFPFFT_sol = new G4Tubs("CFPFFT_sol", 0.0, 9.53*mm,
                                  (560/2.)*mm, 0.0*deg, 360.0*deg);

  // G4RotationMatrix* changeYtoBeamAxis = new G4RotationMatrix();
  changeYtoBeamAxis = new CLHEP::HepRotation();
  changeYtoBeamAxis->rotateY(90*deg);

  G4SubtractionSolid* ChamberCut9_sol =
    new G4SubtractionSolid("ChamberCut9_sol", ChamberCut8_sol, CFPFFT_sol,
                           changeYtoBeamAxis, noChange);

  // ------------------------------ Cut for large pumping port (CFLPP)

  G4Tubs* CFLPP_sol = new G4Tubs("CFLPP_sol", 0.0, 25.4*mm , (285/2.)*mm,
                                 0.0*deg, 360.0*deg);

  // G4RotationMatrix* changeYtoBeamAxisForLPP1 = new G4RotationMatrix();
  changeYtoBeamAxisForLPP1 = new CLHEP::HepRotation();
  changeYtoBeamAxisForLPP1->rotateY(90*deg);

  G4ThreeVector positionForPumpingPortChamfer1(-160.0*mm, (152.4/2.0)*mm,
                                               (152.4/2.0)*mm);

  G4SubtractionSolid* ChamberCut10_sol =
    new G4SubtractionSolid("ChamberCut10_sol", ChamberCut9_sol, CFLPP_sol,
                           changeYtoBeamAxisForLPP1,
                           positionForPumpingPortChamfer1);

  // G4RotationMatrix* changeYtoBeamAxisForLPP2 = new G4RotationMatrix();
  changeYtoBeamAxisForLPP2 = new CLHEP::HepRotation();
  changeYtoBeamAxisForLPP2->rotateY(90*deg);

  G4ThreeVector positionForPumpingPortChamfer2(-160.0*mm, (-152.4/2.0)*mm,
                                               (152.4/2.0)*mm);

  G4SubtractionSolid* ChamberCut11_sol =
    new G4SubtractionSolid("ChamberCut11_sol", ChamberCut10_sol, CFLPP_sol,
                           changeYtoBeamAxisForLPP2,
                           positionForPumpingPortChamfer2);

  // G4RotationMatrix* changeYtoBeamAxisForLPP3 = new G4RotationMatrix();
  changeYtoBeamAxisForLPP3 = new CLHEP::HepRotation();
  changeYtoBeamAxisForLPP3->rotateY(90*deg);

  G4ThreeVector positionForPumpingPortChamfer3(-160.0*mm, (-152.4/2.0)*mm,
                                               (-152.4/2.0)*mm);

  G4SubtractionSolid* ChamberCut12_sol =
    new G4SubtractionSolid("ChamberCut12_sol", ChamberCut11_sol,
                           CFLPP_sol, changeYtoBeamAxisForLPP3,
                           positionForPumpingPortChamfer3);

  // G4RotationMatrix* changeYtoBeamAxisForLPP4 = new G4RotationMatrix();
  changeYtoBeamAxisForLPP4 = new CLHEP::HepRotation();
  changeYtoBeamAxisForLPP4->rotateY(90*deg);

  G4ThreeVector positionForPumpingPortChamfer4(-160.0*mm, (152.4/2.0)*mm,
                                               (-152.4/2.0)*mm);

  G4SubtractionSolid* ChamberCut13_sol =
    new G4SubtractionSolid("ChamberCut13_sol", ChamberCut12_sol, CFLPP_sol,
                           changeYtoBeamAxisForLPP4,
                           positionForPumpingPortChamfer4);

  // ------------------------------ central box cuts for large
  // pumping port

  G4Box* CFLPPBoxCut1_sol = new G4Box("CFLPPBoxCut1_sol", (285/2.)*mm,
                                      (152.4/2.)*mm, (203.2/2.)*mm);

  G4ThreeVector positionForPumpingPortChamfer5(-160.0*mm, 0.0 , 0.0);

  G4SubtractionSolid* ChamberCut14_sol =
    new G4SubtractionSolid("ChamberCut14_sol", ChamberCut13_sol,
                           CFLPPBoxCut1_sol, 0,
                           positionForPumpingPortChamfer5);

  G4Box* CFLPPBoxCut2_sol = new G4Box("CFLPPBoxCut2_sol", (285/2.)*mm,
                                      (203.2/2.)*mm, (152.4/2.)*mm);

  G4SubtractionSolid* ChamberCut15_sol =
    new G4SubtractionSolid("ChamberCut15_sol", ChamberCut14_sol,
                           CFLPPBoxCut2_sol, 0, positionForPumpingPortChamfer5);

  // ------------------------------ cut for optical pumping beams (CFOPB)

  G4Tubs* CFOPB_sol = new G4Tubs("CFOPB_sol", 0.0, (38.10/2.0)*mm,
                                 (300/2.)*mm, 0.0*deg, 360.0*deg);
  rotationForOpticalPumpingBeams1 = new CLHEP::HepRotation();
  rotationForOpticalPumpingBeams1->rotateX(19*deg);
  G4ThreeVector positionOfOpticalPumpingBeams1(0.0*mm,
                                               ((0.3443*170.0)+29.2655)*mm,
                                               170.0*mm);
  // X=0 because we are in the YZ plane Z=170 is arbitrary
  // Y=(tan19)*Z + 29.2655 where 29.2655 is the constant to make it cross
  // Z at Z=-85mm the mirror position

  G4SubtractionSolid* ChamberCut16_sol =
    new G4SubtractionSolid("ChamberCut16_sol", ChamberCut15_sol, CFOPB_sol,
                           rotationForOpticalPumpingBeams1,
                           positionOfOpticalPumpingBeams1);

  rotationForOpticalPumpingBeams2 = new CLHEP::HepRotation();
  rotationForOpticalPumpingBeams2->rotateX(19*deg);

  G4ThreeVector positionOfOpticalPumpingBeams2(0.0*mm,
                                               ((0.3443*-170.0)-29.2655)*mm,
                                               -170.0*mm);
  // The formula is almost the same as above but the sign of the constant
  // is flipped so that it crosses the Zaxis at Z=85mm the other mirror.

  G4SubtractionSolid* ChamberCut17_sol =
    new G4SubtractionSolid("ChamberCut17_sol", ChamberCut16_sol, CFOPB_sol,
                           rotationForOpticalPumpingBeams2,
                           positionOfOpticalPumpingBeams2);

  chamber_box_ = ChamberCut17_sol;      // Store the final product
  // ------------------------------ Final Product after all of the cuts.

  chamber_log_ = new G4LogicalVolume(chamber_box_, ChamberMaterial,
                                     "chamber_log_", 0, 0, 0);
  chamber_phys_ = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0),
                                    chamber_log_, "chamber_phys", world_log_,
                                    false, 0);

  // chamber_log_VisAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  // chamber_logVisAttributes-> SetForceWireframe(true);

  // G4VisAttributes(false) means invisible. It is better than using the
  // convience function G4VisAttributes::Invisble becuse it produces
  // a non const pointer that can later be delete avoiding a memory
  // leak.
  chamber_logVisAttributes = new G4VisAttributes(false);
  // chamber_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5,
  //                                                         0.5, 0.8));
  // chamber_logVisAttributes-> SetForceSolid(true);
  chamber_log_ -> SetVisAttributes(chamber_logVisAttributes);

  // ------------------------------ Optical Pumping Rentrant Flanges
  // (z+ and z-) (OPRF)


  G4VSolid * OPRF_sol = new G4Tubs("OPRF_sol", (98.298/2.0)*mm, (101.6/2.0)*mm,
                                   (160.0/2.0)*mm, 0.0*deg, 360.0*deg);

  G4LogicalVolume * OPRF_log = new G4LogicalVolume(OPRF_sol,
                                                   ChamberMaterial,
                                                   "OPRF_log", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 182*mm), OPRF_log,
                    "OPRF_plusZ_phys", world_log_, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -182*mm), OPRF_log,
                    "OPRF_minusZ_phys", world_log_, false, 0);
  OPRF_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  OPRF_logVisAttributes-> SetForceSolid(true);
  // chamber_logVisAttributes-> SetForceWireframe(true);
  OPRF_log -> SetVisAttributes(OPRF_logVisAttributes);

  // ------------------------------ Rentrant Flange Descender (RFD)

  G4VSolid * RFD_sol = new G4Cons("RFD_sol", 47.431*mm, 48.951*mm, 49.153*mm,
                                  50.673*mm, 2.5*mm, 0.0*deg, 360.0*deg);
  G4LogicalVolume* RFD_log = new G4LogicalVolume(RFD_sol, ChamberMaterial,
                                                 "RFD_log", 0, 0, 0);
  RFD_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  RFD_logVisAttributes-> SetForceSolid(true);
  RFD_log -> SetVisAttributes(RFD_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 99.5*mm), RFD_log,
                    "RFD_plusZ_phys", world_log_, false, 0);


  // G4RotationMatrix* RFDRotation = new G4RotationMatrix();
  RFDRotation = new CLHEP::HepRotation();
  RFDRotation->rotateX(180.*deg);

  new G4PVPlacement(RFDRotation, G4ThreeVector(0.0, 0.0, -99.5*mm), RFD_log,
                    "RFD_minusZ_phys", world_log_, false, 0);

  // ------------------------------ beryllium declared here so that a
  // socket could be cut out for it
  G4VSolid * beryllium_sol = new G4Tubs("beryllium_sol", 0*mm, 30.0*mm,
                                        (0.229/2.0)*mm, 0.0*deg,
                                        360.0*deg);
  G4LogicalVolume* beryllium_log = new G4LogicalVolume(beryllium_sol,
                                                       FoilMaterial,
                                                       "beryllium_log",
                                                       0, 0, 0);

  beryllium_logVisAttributes = new G4VisAttributes(G4Colour(0.1, 0.2, 0.7));
  // G4VisAttributes* beryllium_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.1, 0.2, 0.7));

  beryllium_logVisAttributes-> SetForceSolid(true);

  beryllium_log -> SetVisAttributes(beryllium_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, (97.0-(0.229/2.0))*mm),
                    beryllium_log, "beryllium_plusZ_phys", world_log_, false,
                    0);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -(97.0-(0.229/2.0))*mm),
                    beryllium_log, "beryllium_minusZ_phys",
                    world_log_, false, 0);

  // ------------------------------ Front Face Rentrant Flange (FFRF)

  G4VSolid * FFRF_beforeCut_sol = new G4Cons("FFRF_beforeCut_sol", 29.0*mm,
                                             47.295*mm, 29.0*mm, 48.951*mm,
                                             2.5*mm, 0.0*deg, 360.0*deg);

  G4ThreeVector cutForBeryllium(0.0*mm, 0.0*mm, (2.5 - (0.229/2.0))*mm);

  G4SubtractionSolid* FFRF_sol =
    new G4SubtractionSolid("FFRF_sol", FFRF_beforeCut_sol, beryllium_sol, 0,
                           cutForBeryllium);

  G4LogicalVolume* FFRF_log = new G4LogicalVolume(FFRF_sol, ChamberMaterial,
                                                  "FFRF_log", 0, 0, 0);

  FFRF_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  // G4VisAttributes* FFRF_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.8, 0.4, 0.5));

  FFRF_logVisAttributes-> SetForceSolid(true);

  FFRF_log -> SetVisAttributes(FFRF_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 94.5*mm), FFRF_log,
                    "FFRF_plusZ_phys", world_log_, false, 0);

  // G4RotationMatrix* FFRFRotation = new G4RotationMatrix();
  FFRFRotation = new CLHEP::HepRotation();
  FFRFRotation->rotateX(180.*deg);

  new G4PVPlacement(FFRFRotation, G4ThreeVector(0.0, 0.0, -94.5*mm),
                    FFRF_log, "FFRF_minusZ_phys", world_log_, false, 0);
}  // End construct chamber

void K37DetectorConstruction::ConstructMirrors() {
  G4double MirrorPositionZ = 85.05069*mm;

  // G4RotationMatrix *MirrorRotation = new G4RotationMatrix();
  MirrorRotation = new CLHEP::HepRotation();
  MirrorRotation->rotateX(9.5*deg);

  G4VSolid * mirror_sol = new G4Tubs("mirror", 0.0*mm, 30.48*mm, 0.25*mm,
                                     0.0*deg , 360.0*deg);

  mirror_log = new G4LogicalVolume(mirror_sol, MirrorMaterial,
                                   "mirror_log", 0, 0, 0);
  new G4PVPlacement(MirrorRotation, G4ThreeVector(0, 0, MirrorPositionZ),
                    mirror_log, "mirror_plusZ_phys", world_log_, false, 0);
  new G4PVPlacement(MirrorRotation,
                    G4ThreeVector(0, 0, -MirrorPositionZ), mirror_log,
                    "mirror_minusZ_phys", world_log_, false, 0);
  mirror_logVisAttributes = new G4VisAttributes(G4Colour(0.9, 0.1, 0.1, 0.9));
  mirror_logVisAttributes-> SetForceSolid(true);
  mirror_log -> SetVisAttributes(mirror_logVisAttributes);
  // ------------------------------ Mirror Mount (MM)
  G4VSolid * MM_beforCollimatorCut_sol =
    new G4Tubs("MM_beforCollimatorCut_sol", 0.0*deg, 42.5*mm, 7.0*mm, 0.0*deg,
               360.0*deg);
  if (shouldTheMirrorBeWFEDMCut == true) {
    MM_CollimatorCut_sol = new G4Trd("MM_CollimatorCut_sol", 16.05*mm, 18.93*mm,
                                     16.05*mm, 18.93*mm, 7.1*mm);
  } else {
    MM_CollimatorCut_sol = new G4Trd("MM_CollimatorCut_sol", 18.93*mm, 18.93*mm,
                                     18.93*mm, 18.93*mm, 7.1*mm);
  }

  G4SubtractionSolid* MM_beforeMirrorCut_sol =
    new G4SubtractionSolid("MM_beforeMirrorCut_sol", MM_beforCollimatorCut_sol,
                           MM_CollimatorCut_sol);

  // G4RotationMatrix *MirrorCutRotation = new G4RotationMatrix();
  MirrorCutRotation = new CLHEP::HepRotation();
  MirrorCutRotation->rotateX(9.5*deg);

  G4SubtractionSolid* MM_sol =
    new G4SubtractionSolid("MM_sol", MM_beforeMirrorCut_sol, mirror_sol,
                           MirrorCutRotation, G4ThreeVector(0, 0, 0.05069));

  G4LogicalVolume *MM_log = new G4LogicalVolume(MM_sol,
                                                MirrorMountMaterial,
                                                "MM_log", 0, 0, 0);

  MM_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  // G4VisAttributes* MM_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.9, 0.5));

  MM_logVisAttributes-> SetForceSolid(true);
  // MM_logVisAttributes-> SetForceWireframe(true);
  MM_log -> SetVisAttributes(MM_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 85.0*mm), MM_log,
                    "MM_plusZ_phys", world_log_, false, 0);
  // G4RotationMatrix* MMRotation = new G4RotationMatrix();
  MMRotation = new CLHEP::HepRotation();
  MMRotation->rotateX(180.*deg);

  new G4PVPlacement(MMRotation, G4ThreeVector(0.0, 0.0, -85.0*mm), MM_log,
                    "MM_minusZ_phys", world_log_, false, 0);
}  // End ConstructMirrors

void K37DetectorConstruction::ConstructHoops() {
  hoopRotation = new G4RotationMatrix();
  hoopRotation->rotateX(90.*deg);

  cut_ESP_logVisAttributes =
    new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 1.0));
  cut_ESP_logVisAttributes-> SetForceSolid(true);

  // ------------------------------  3 - 6  The definitions for these came
  // from Alexandre and a drawing for them is in /Drawings/hoops.pdf

  G4double hoop_3through6_x = (180./2.)*mm;
  G4double hoop_3through6_y = (90./2.)*mm;
  G4double hoop_3through6_z = (1./2.)*mm;

  G4Box * hoop_3through6_box_sol = new G4Box("hoop_3through6_box_sol",
                                             hoop_3through6_x,
                                             hoop_3through6_y,
                                             hoop_3through6_z);

  G4double hoop_3through6innerCut_x = (165.8/2.)*mm;
  G4double hoop_3through6innerCut_y = (75.8/2.)*mm;
  G4double hoop_3through6innerCut_z = (1.5/2.)*mm;

  G4Box * hoop_3through6_boxInnerCut_sol =
    new G4Box("hoop_3through6_boxInnerCut_sol", hoop_3through6innerCut_x,
              hoop_3through6innerCut_y, hoop_3through6innerCut_z);
  G4SubtractionSolid* cut_hoop_3through6_box_sol =
    new G4SubtractionSolid("cut_hoop_3through6_box_sol",
                           hoop_3through6_box_sol,
                           hoop_3through6_boxInnerCut_sol);
  G4double mountingHole_rmax = (4.2/2.)*mm;
  G4double mountingHole_rmin = 0.          *mm;
  G4double mountingHole_dz   = (1.5/2.)*mm;
  G4double mountingHole_Sphi = 0.          *deg;
  G4double mountingHole_Dphi = 360.        *deg;

  G4Tubs * mountingHole_sol = new G4Tubs("mountingHole_sol",
                                         mountingHole_rmin,
                                         mountingHole_rmax,
                                         mountingHole_dz,
                                         mountingHole_Sphi,
                                         mountingHole_Dphi);

  G4SubtractionSolid* cut_hoop_3through6_withHole1_sol =
    new G4SubtractionSolid("cut_hoop_3through6_withHole1_sol",
                           cut_hoop_3through6_box_sol, mountingHole_sol,
                           0, G4ThreeVector(85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_3through6_withHole2_sol =
    new G4SubtractionSolid("cut_hoop_3through6_withHole2_sol",
                           cut_hoop_3through6_withHole1_sol,
                           mountingHole_sol, 0,
                           G4ThreeVector(85.0*mm, -40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_3through6_withHole3_sol =
    new G4SubtractionSolid("cut_hoop_3through6_withHole3_sol",
                           cut_hoop_3through6_withHole2_sol,
                           mountingHole_sol, 0,
                           G4ThreeVector(-85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_3through6_withHoles_sol =
    new G4SubtractionSolid("cut_hoop_3through6_withHoles_sol",
                           cut_hoop_3through6_withHole3_sol,
                           mountingHole_sol, 0,
                           G4ThreeVector(-85.0*mm, -40.0*mm, 0));
  G4LogicalVolume *hoop_3through6_box_log =
    new G4LogicalVolume(cut_hoop_3through6_withHoles_sol, HoopMaterial,
                        "hoop_3through6_box_log", 0, 0, 0);
  // ------------------------------  2
  // Before 5-12-11 hoop 2 and 7 where identical. That is why many of the
  // variable names here are for hoop 7.
  // Hoop 7 now has its own implementation that appears below. It only
  // differes by the middle openiing. See pictures in the drawings
  // directory.

  G4double hoop_7_x = (180./2.)*mm;
  G4double hoop_7_y = (90./2.)*mm;
  G4double hoop_7_z = (1./2.)*mm;

  G4Box * hoop_7_box_sol = new G4Box("hoop_7_box_sol",
                                     hoop_7_x , hoop_7_y, hoop_7_z);

  G4double hoop_7innerCut_x = (100.0/2.)*mm;
  G4double hoop_7innerCut_y = (75.8/2.)*mm;
  G4double hoop_7innerCut_z = (1.5/2.)*mm;

  G4Box * hoop_7_boxInnerCut_sol = new G4Box("hoop_7_boxInnerCut_sol",
                                             hoop_7innerCut_x,
                                             hoop_7innerCut_y,
                                             hoop_7innerCut_z);
  G4SubtractionSolid* cut_hoop_7_box_sol =
    new G4SubtractionSolid("cut_hoop_7_box_sol", hoop_7_box_sol,
                           hoop_7_boxInnerCut_sol);

  G4SubtractionSolid* cut_hoop_7_withHole1_sol =
    new G4SubtractionSolid("cut_hoop_7_withHole1_sol",
                           cut_hoop_7_box_sol, mountingHole_sol, 0,
                           G4ThreeVector(85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_7_withHole2_sol =
    new G4SubtractionSolid("cut_hoop_7_withHole2_sol",
                           cut_hoop_7_withHole1_sol, mountingHole_sol, 0,
                           G4ThreeVector(85.0*mm, -40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_7_withHole3_sol =
    new G4SubtractionSolid("cut_hoop_7_withHole3_sol",
                           cut_hoop_7_withHole2_sol, mountingHole_sol, 0,
                           G4ThreeVector(-85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_7_withHoles_sol =
    new G4SubtractionSolid("cut_hoop_7_withHoles_sol",
                           cut_hoop_7_withHole3_sol, mountingHole_sol, 0,
                           G4ThreeVector(-85.0*mm, -40.0*mm, 0));

  G4double hoop_7beamCut_x = (35.0/2.)*mm;
  G4double hoop_7beamCut_y = (25.0/2.)*mm;
  G4double hoop_7beamCut_z = (1.5/2.)*mm;

  G4EllipticalTube * hoop_7beamCut =
    new G4EllipticalTube("hoop_7beamCut", hoop_7beamCut_x ,
                         hoop_7beamCut_y, hoop_7beamCut_z);

  G4SubtractionSolid* cut_hoop_7_withHolesAndBeams1_sol =
    new G4SubtractionSolid("cut_hoop_7_withHolesAndBeams1_sol",
                           cut_hoop_7_withHoles_sol, hoop_7beamCut, 0,
                           G4ThreeVector(75.0*mm, 0, 0));
  G4SubtractionSolid* cut_hoop_7_withHolesAndBeams_sol =
    new G4SubtractionSolid("cut_hoop_7_withHolesAndBeams_sol",
                           cut_hoop_7_withHolesAndBeams1_sol,
                           hoop_7beamCut, 0,
                           G4ThreeVector(-75.0*mm, 0, 0));

  G4double recoilPathHole_rmax = (80.0/2.)*mm;
  G4double recoilPathHole_rmin = 0.*mm;
  G4double recoilPathHole_dz   = (1.5/2.)*mm;
  G4double recoilPathHole_Sphi = 0.*deg;
  G4double recoilPathHole_Dphi = 360.*deg;

  G4Tubs * recoilPathHole_sol = new G4Tubs("recoilPathHole_sol",
                                           recoilPathHole_rmin,
                                           recoilPathHole_rmax,
                                           recoilPathHole_dz,
                                           recoilPathHole_Sphi,
                                           recoilPathHole_Dphi);

  G4SubtractionSolid* cut_hoop_7_withHolesAndBeamsAndRecoil_sol =
    new G4SubtractionSolid("cut_hoop_7_withHolesAndBeamsAndRecoil_sol",
                           cut_hoop_7_withHolesAndBeams_sol,
                           recoilPathHole_sol);

  // G4LogicalVolume *hoop_7_log =
  //   new G4LogicalVolume(cut_hoop_7_withHolesAndBeams_sol,
  //                       StainlessSteel, "hoop_7_log", 0, 0, 0);
  G4LogicalVolume *hoop_2_log =
    new G4LogicalVolume(cut_hoop_7_withHolesAndBeamsAndRecoil_sol,
                        Hoop7Material, "hoop_2_log", 0, 0, 0);
  // ------------------------------  7
  // G4double hoop_7_x = (180./2.)*mm;
  // G4double hoop_7_y = (90./2.)*mm;
  // G4double hoop_7_z = (1./2.)*mm;
  // G4Box * hoop_7_box_sol = new G4Box("hoop_7_box_sol",
  //                                    hoop_7_x, hoop_7_y, hoop_7_z);
  // G4double hoop_7innerCut_x = (100.0/2.)*mm;
  // G4double hoop_7innerCut_y = (75.8/2.)*mm;
  // G4double hoop_7innerCut_z = (1.5/2.)*mm;

  G4double hoop_7InnerHole_rmax = (75.8/2.)*mm;
  G4double hoop_7InnerHole_rmin = 0.      *mm;
  G4double hoop_7InnerHole_dz   = (1.5/2.)*mm;
  G4double hoop_7InnerHole_Sphi = 0.  *deg;
  G4double hoop_7InnerHole_Dphi = 360.*deg;
  G4Tubs * hoop_7InnerHole_sol = new G4Tubs("hoop_7InnerHole_sol",
                                            hoop_7InnerHole_rmin,
                                            hoop_7InnerHole_rmax,
                                            hoop_7InnerHole_dz,
                                            hoop_7InnerHole_Sphi,
                                            hoop_7InnerHole_Dphi);

  // G4Box * newhoop_7_boxInnerCut_sol =
  //   new G4Box("newhoop_7_boxInnerCut_sol", hoop_7innerCut_x,
  //             hoop_7innerCut_y, hoop_7innerCut_z);
  G4SubtractionSolid* cutTube_hoop_7_box_sol =
    new G4SubtractionSolid("cutTube_hoop_7_box_sol", hoop_7_box_sol,
                           hoop_7InnerHole_sol);
  G4SubtractionSolid* cutTube_hoop_7_withHole1_sol =
    new G4SubtractionSolid("cutTube_hoop_7_withHole1_sol",
                           cutTube_hoop_7_box_sol, mountingHole_sol, 0,
                           G4ThreeVector(85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cutTube_hoop_7_withHole2_sol =
    new G4SubtractionSolid("cutTube_hoop_7_withHole2_sol",
                           cutTube_hoop_7_withHole1_sol, mountingHole_sol,
                           0, G4ThreeVector(85.0*mm, -40.0*mm, 0));
  G4SubtractionSolid* cutTube_hoop_7_withHole3_sol =
    new G4SubtractionSolid("cutTube_hoop_7_withHole3_sol",
                           cutTube_hoop_7_withHole2_sol, mountingHole_sol,
                           0, G4ThreeVector(-85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cutTube_hoop_7_withHoles_sol =
    new G4SubtractionSolid("cutTube_hoop_7_withHoles_sol",
                           cutTube_hoop_7_withHole3_sol, mountingHole_sol,
                           0, G4ThreeVector(-85.0*mm, -40.0*mm, 0));

  // G4double hoop_7beamCut_x = (35.0/2.)*mm;
  // G4double hoop_7beamCut_y = (25.0/2.)*mm;
  // G4double hoop_7beamCut_z = (1.5/2.)*mm;
  // G4EllipticalTube * hoop_7beamCut =
  //   new G4EllipticalTube("hoop_7beamCut", hoop_7beamCut_x,
  //                        hoop_7beamCut_y, hoop_7beamCut_z);
  G4SubtractionSolid* cutTube_hoop_7_withHolesAndBeams1_sol =
    new G4SubtractionSolid("cutTube_hoop_7_withHolesAndBeams1_sol",
                           cutTube_hoop_7_withHoles_sol,
                           hoop_7beamCut, 0,
                           G4ThreeVector(75.0*mm, 0, 0));
  G4SubtractionSolid* cutTube_hoop_7_withHolesAndBeams_sol =
    new G4SubtractionSolid("cutTube_hoop_7_withHolesAndBeams_sol",
                           cutTube_hoop_7_withHolesAndBeams1_sol,
                           hoop_7beamCut, 0,
                           G4ThreeVector(-75.0*mm, 0, 0));

  // G4double recoilPathHole_rmax = (80.0/2.)*mm;
  // G4double recoilPathHole_rmin = 0.     *mm;
  // G4double recoilPathHole_dz   = (1.5/2.) *mm;
  // G4double recoilPathHole_Sphi = 0.     *deg;
  // G4double recoilPathHole_Dphi = 360.   *deg;
  // G4Tubs * recoilPathHole_sol = new G4Tubs("recoilPathHole_sol",
  //                                          recoilPathHole_rmin,
  //                                          recoilPathHole_rmax,
  //                                          recoilPathHole_dz,
  //                                          recoilPathHole_Sphi,
  //                                          recoilPathHole_Dphi);
  // G4SubtractionSolid* cutTube_hoop_7_withHolesAndBeamsAndRecoil_sol =
  //   new G4SubtractionSolid(
  //           "cutTube_hoop_7_withHolesAndBeamsAndRecoil_sol",
  //           cutTube_hoop_7_withHolesAndBeams_sol, recoilPathHole_sol);
  G4LogicalVolume *hoop_7_log =
    new G4LogicalVolume(cutTube_hoop_7_withHolesAndBeams_sol,
                        Hoop7Material, "hoop_7_log", 0, 0, 0);
  // G4LogicalVolume *hoop_2_log =
  //   new G4LogicalVolume(cut_hoop_7_withHolesAndBeamsAndRecoil_sol,
  //                       GlassyCarbon, "hoop_2_log", 0, 0, 0);
  // ------------------------------  1
  G4double hoop_1_x = (180./2.)*mm;
  G4double hoop_1_y = (100./2.)*mm;
  G4double hoop_1_z = (1./2.)*mm;

  G4Box * hoop_1_box_sol = new G4Box("hoop_1_box_sol",
                                     hoop_1_x , hoop_1_y, hoop_1_z);

  G4double hoop_1_centralHole_rmax = (90.0/2.)*mm;
  G4double hoop_1_centralHole_rmin = 0.       *mm;
  G4double hoop_1_centralHole_dz   = (1.5/2.) *mm;
  G4double hoop_1_centralHole_Sphi = 0.   *deg;
  G4double hoop_1_centralHole_Dphi = 360. *deg;

  G4Tubs * hoop_1_centralHole_sol =
    new G4Tubs("hoop_1_centralHole_sol", hoop_1_centralHole_rmin,
               hoop_1_centralHole_rmax, hoop_1_centralHole_dz,
               hoop_1_centralHole_Sphi, hoop_1_centralHole_Dphi);

  G4SubtractionSolid* cut_hoop_1_box_sol =
    new G4SubtractionSolid("cut_hoop_1_box_sol", hoop_1_box_sol,
                           hoop_1_centralHole_sol);

  G4SubtractionSolid* cut_hoop_1_withHole1_sol =
    new G4SubtractionSolid("cut_hoop_1_withHole1_sol", cut_hoop_1_box_sol,
                           mountingHole_sol, 0,
                           G4ThreeVector(85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_1_withHole2_sol =
    new G4SubtractionSolid("cut_hoop_1_withHole2_sol",
                           cut_hoop_1_withHole1_sol, mountingHole_sol, 0,
                           G4ThreeVector(85.0*mm, -40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_1_withHole3_sol =
    new G4SubtractionSolid("cut_hoop_1_withHole3_sol",
                           cut_hoop_1_withHole2_sol, mountingHole_sol,
                           0, G4ThreeVector(-85.0*mm, 40.0*mm, 0));
  G4SubtractionSolid* cut_hoop_1_withHoles_sol =
    new G4SubtractionSolid("cut_hoop_1_withHoles_sol",
                           cut_hoop_1_withHole3_sol, mountingHole_sol, 0,
                           G4ThreeVector(-85.0*mm, -40.0*mm, 0));

  G4SubtractionSolid* cut_hoop_1_withHolesAndBeams1_sol =
    new G4SubtractionSolid("cut_hoop_1_withHolesAndBeams1_sol",
                           cut_hoop_1_withHoles_sol, hoop_7beamCut, 0,
                           G4ThreeVector(97.0*mm, 0, 0));
  G4SubtractionSolid* cut_hoop_1_withHolesAndBeams_sol =
    new G4SubtractionSolid("cut_hoop_1_withHolesAndBeams_sol",
                           cut_hoop_1_withHolesAndBeams1_sol,
                           hoop_7beamCut, 0,
                           G4ThreeVector(-97.0*mm, 0, 0));
  G4LogicalVolume *hoop_1_log =
    new G4LogicalVolume(cut_hoop_1_withHolesAndBeams_sol, HoopMaterial,
                        "hoop_1_log", 0, 0, 0);
  // ------------------------------  vis and placment of hoops
  // cut_ESP_logVisAttributes = new G4VisAttributes(G4Colour(0.2, 0.8, 0.1));
  // G4VisAttributes* cut_ESP_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.8, 0.1));
  cut_ESP_logVisAttributes-> SetForceSolid(true);
  hoop7_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  // G4VisAttributes* hoop7_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.1, 0.6));
  hoop7_logVisAttributes-> SetForceSolid(true);

  hoop_3through6_box_log ->
    SetVisAttributes(cut_ESP_logVisAttributes);  // 3-6
  hoop_7_log -> SetVisAttributes(hoop7_logVisAttributes);    // 7
  hoop_2_log -> SetVisAttributes(hoop7_logVisAttributes);    // 2
  hoop_1_log -> SetVisAttributes(cut_ESP_logVisAttributes);  // 1

  // G4RotationMatrix* hoopRotation = new G4RotationMatrix();
  // hoopRotation = new CLHEP::HepRotation();
  // hoopRotation->rotateX(90.*deg);

  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 57.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_3",
                    world_log_, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 29.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_4",
                    world_log_, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -29.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_5",
                    world_log_, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -57.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_6",
                    world_log_, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -75.5*mm, 0.),
                    hoop_7_log, "hoop_7_phys", world_log_, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 75.5*mm, 0.),
                    hoop_2_log, "hoop_2_phys", world_log_, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 97.5*mm, 0.),
                    hoop_1_log, "hoop_1_phys", world_log_, false, 0);
}  // End construct hoops

void K37DetectorConstruction::ConstructElectronMCP(G4SDManager *sd_man) {
  G4double SOED_rmax = 40./2.*mm;
  G4double SOED_rmin = 0     *mm;
  G4double SOED_dz   = 10./2.*mm;
  G4double SOED_Sphi = 0.    *deg;
  G4double SOED_Dphi = 360.  *deg;
  G4double SOED_z_pos = -82.0 *mm;     // 82 mm
  electron_mcp_tub_ = new G4Tubs("SOED_sol", SOED_rmin, SOED_rmax,  SOED_dz,
                                 SOED_Sphi, SOED_Dphi);
  electron_mcp_log_ = new G4LogicalVolume(electron_mcp_tub_, MCPMaterial,
                                          "SOED_log", 0, 0, 0);
  electron_mcp_phys_ = new G4PVPlacement(changeZtoX,
                                         G4ThreeVector(0., SOED_z_pos, 0),
                                         electron_mcp_log_, "SOED_phys",
                                         world_log_, false, 0);
  SOED_logVisAttributes = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));
  SOED_logVisAttributes-> SetForceSolid(true);
  electron_mcp_log_ -> SetVisAttributes(SOED_logVisAttributes);

  if (electron_mcp_sens_) {
    G4cout << "Deleting electron MCP..." << G4endl;
    delete electron_mcp_sens_;
    G4cout << "done" << G4endl;
  }
  // Set up sensitive detector
  electron_mcp_sens_ = new K37ElectronMCPSD("/mydet/electron_mcp");
  sd_man -> AddNewDetector(electron_mcp_sens_);
  electron_mcp_log_ -> SetSensitiveDetector(electron_mcp_sens_);
}  // End construct EMCP

void K37DetectorConstruction::ConstructRecoilMCP(G4SDManager *sd_man) {
  // G4double rmcp_rmax = 83./2.*mm;
  G4double rmcp_rmax = 80./2.*mm;
  G4double rmcp_rmin = 0     *mm;
  G4double rmcp_dz   = 10./2.*mm;
  G4double rmcp_start_phi = 0.    *deg;
  G4double rmcp_d_phi = 360.  *deg;
  G4double rmcp_z_pos = (102.0 * mm) + rmcp_dz;
  // Extrudes symetrically about this point

  recoil_mcp_tub_ = new G4Tubs("rmcp_sol", rmcp_rmin, rmcp_rmax,  rmcp_dz,
                               rmcp_start_phi, rmcp_d_phi);
  recoil_mcp_log_ = new G4LogicalVolume(recoil_mcp_tub_, MCPMaterial,
                                       "rmcp_log", 0, 0, 0);
  recoil_mcp_phys_ = new G4PVPlacement(changeZtoX,
                                       G4ThreeVector(0., rmcp_z_pos, 0),
                                       recoil_mcp_log_, "rmcp_phys", world_log_,
                                       false, 0);
  rmcp_logVisAttributes_ = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));
  rmcp_logVisAttributes_ -> SetForceSolid(true);
  recoil_mcp_log_ -> SetVisAttributes(rmcp_logVisAttributes_);

  // Set up sensitive detector
  if (recoil_mcp_sens_) {
    G4cout << "Deleting recoil MCP..." << G4endl;
    delete recoil_mcp_sens_;
    G4cout << "done" << G4endl;
  }
  recoil_mcp_sens_ = new K37RecoilMCPSD("/mydet/recoil_mcp");
  sd_man -> AddNewDetector(recoil_mcp_sens_);
  recoil_mcp_log_ -> SetSensitiveDetector(recoil_mcp_sens_);
}



void K37DetectorConstruction::ConstructCoils() {
  G4double coils_rmax = 198./2. *mm;
  G4double coils_rmin = 138./2. *mm;
  G4double coils_dz   = 20./2.  *mm;
  G4double coils_Sphi = 0.          *deg;
  G4double coils_Dphi = 360.        *deg;

  G4Tubs * coils_sol = new G4Tubs("coils_sol", coils_rmin, coils_rmax,
                                  coils_dz, coils_Sphi, coils_Dphi);
  G4LogicalVolume * coils_log = new G4LogicalVolume(coils_sol, CoilsMaterial,
                                                    "coils_log", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 65.), coils_log,
                    "coils_plusZ_phys", world_log_, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -65.), coils_log,
                    "coils_minusZ_phys", world_log_, false, 0);

  coils_logVisAttributes= new G4VisAttributes(G4Colour(0.6, 0.35, 0.0, 1.0));
  // G4VisAttributes* coils_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.3, 0.1));

  coils_logVisAttributes-> SetForceSolid(true);
  // MM_logVisAttributes-> SetForceWireframe(true);

  coils_log -> SetVisAttributes(coils_logVisAttributes);
}  // End construct coils

void K37DetectorConstruction::DefineMaterials() {
  G4NistManager* nist_manager = G4NistManager::Instance();
  // ------------------------------------------------------ materials
  // atomic mass, atomic number, density, fractional mass
  G4double a, z, density, fractionmass;
  G4int nel, natoms,  ncomponents;  // number of elements, number of atoms

  G4Element* N  = new G4Element("Nitrogen", "N", z = 7., a = 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen", "O", z = 8., a = 16.00*g/mole);
  G4Element* F  = new G4Element("Fluorine", "F", z = 9.,
                                a = 18.9984*g/mole);
  G4Element* Mg = new G4Element("Magnesium", "Mg", z = 12.,
                                a = 24.3050*g/mole);
  G4Element* K  = new G4Element("Potassium", "K", z = 19.,
                                a = 39.0983*g/mole);
  G4Element* B  = new G4Element("Boron", "B", z = 5., a = 10.811*g/mole);
  G4Element* C  = new G4Element("Carbon"    , "C", z = 6.,
                                a = 12.0107  *g/mole);
  G4Element* Si = new G4Element("Silicon"   , "Si", z = 14.,
                                a = 28.086   *g/mole);
  G4Element* Mn = new G4Element("Manganese" , "Mn", z = 25.,
                                a = 54.94    *g/mole);
  G4Element* Cr = new G4Element("Chromium"  , "Cr", z = 24.,
                                a = 52.00    *g/mole);
  G4Element* Ni = new G4Element("Nickel"    , "Ni", z = 28.,
                                a = 58.70    *g/mole);
  G4Element* Fe = new G4Element("Iron"      , "Fe", z = 26.,
                                a = 55.85    *g/mole);
  G4Element* Al = new G4Element("Aluminum"  , "Al", z = 13.,
                                a = 26.9815  *g/mole);
  // G4Element* Cu = new G4Element("Copper"    , "Cu", z = 29.,
  //                               a = 63.546   *g/mole);
  // G4Element* W  = new G4Element("Tungsten"  , "W", z = 74.,
  //                               a = 183.84*g/mole);
  G4Element* H  = new G4Element("Hydrogen"  , "H", z = 1.,
                                a= 1.01     *g/mole);
  G4Element* S  = new G4Element("Sulfur"    , "S", z = 16.,
                                a = 32.065   *g/mole);
  G4Element* P  = new G4Element("Phosphorus", "P", z = 15.,
                                a = 30.973762*g/mole);

  // G4Material* Pb = new G4Material("Lead", z = 82., a = 207.19*g/mole,
  //                                 density = 11.35*g/cm3);

  G4Material* vacuum = new G4Material("Vac ", z = 1., a = 1.01*g/mole,
                                   density = universe_mean_density, kStateGas,
                                   2.73*kelvin, 3.e-18*pascal);

  G4Material* SiliconCarbide = new G4Material("SiliconCarbide",
                                              density = 3.21*g/cm3, nel = 2);
  SiliconCarbide->AddElement(Si, natoms = 1);
  SiliconCarbide->AddElement(C, natoms = 1);

  G4Material* SiliconDiOxide = new G4Material("SiliconDiOxide",
                                              density = 2.648*g/cm3, nel = 2);
  SiliconDiOxide->AddElement(Si, natoms = 1);
  SiliconDiOxide->AddElement(O , natoms = 2);


  G4Material* MagnesiumOxide = new G4Material("MagnesiumOxide",
                                              density = 3.58*g/cm3, nel = 2);
  MagnesiumOxide->AddElement(Mg, natoms = 1);
  MagnesiumOxide->AddElement(O , natoms = 1);

  G4Material* AluminumOxide = new G4Material("AluminumOxide",
                                             density = 3.95*g/cm3, nel = 2);
  AluminumOxide->AddElement(Al, natoms = 2);
  AluminumOxide->AddElement(O , natoms = 3);

  G4Material* PotassiumOxide = new G4Material("PotassiumOxide",
                                              density = 2.35*g/cm3, nel = 2);
  PotassiumOxide->AddElement(K, natoms = 2);
  PotassiumOxide->AddElement(O , natoms = 1);


  G4Material* BoronOxide = new G4Material("BoronOxide", density = 2.55*g/cm3,
                                          nel = 2);
  BoronOxide->AddElement(B, natoms = 2);
  BoronOxide->AddElement(O , natoms = 3);


  G4Material* Scintillator = new G4Material("Scintillator",
                                            density = 1.032*g/cm3, nel = 2);
  Scintillator->AddElement(H, 30);
  Scintillator->AddElement(C, 27);

  G4Material* StainlessSteel = new G4Material("StainlessSteel",
                                              density = 8.03*g/cm3, nel = 9);
  StainlessSteel -> AddElement(Fe, fractionmass = 0.70995);
  StainlessSteel -> AddElement(Cr, fractionmass = 0.18);
  StainlessSteel -> AddElement(Ni, fractionmass = 0.08);
  StainlessSteel -> AddElement(Mn, fractionmass = 0.02);
  StainlessSteel -> AddElement(N , fractionmass = 0.001);
  StainlessSteel -> AddElement(S , fractionmass = 0.0003);
  StainlessSteel -> AddElement(C , fractionmass = 0.0008);
  StainlessSteel -> AddElement(Si, fractionmass = 0.0075);
  StainlessSteel -> AddElement(P , fractionmass = 0.00045);

  G4Material* GlassyCarbon = new G4Material("GlassyCarbon", z = 6.,
                                            a = 12.0107*g/mole,
                                            density = 1.5*g/cm3);
  G4Material* Beryllium = new G4Material("Beryllium", z = 4.,
                                         a = 9.012182*g/mole,
                                         density = 1.848*g/cm3);
  G4Material* SiliconMat = new G4Material("SiliconMat", z = 14.,
                                          a = 28.086*g/mole,
                                          density= 2.329*g/cm3);

  G4Material* Titanium = new G4Material("Titanium", z = 22.,
                                          a = 47.867*g/mole,
                                          density= 4.507*g/cm3);

  G4Material* Sapphire = new G4Material("Sapphire", density = 3.98*g/cm3,
                                        nel = 2);
  Sapphire -> AddElement(Al, natoms = 2);
  Sapphire -> AddElement(O, natoms = 1);

  G4Material* Kapton = new G4Material("Kapton", density = 1.42*mg/cm3,
                                      nel = 4);
  Kapton -> AddElement(H, fractionmass = 0.026362);
  Kapton -> AddElement(C, fractionmass = 0.691133);
  Kapton -> AddElement(N, fractionmass = 0.073270);
  Kapton -> AddElement(O, fractionmass = 0.209235);

  G4Material* Macor = new G4Material("Macor", density = 2.52*g/cm3,
                                     ncomponents = 6);
  Macor -> AddMaterial(SiliconDiOxide, fractionmass = 46*perCent);
  Macor -> AddMaterial(MagnesiumOxide, fractionmass = 17*perCent);
  Macor -> AddMaterial(AluminumOxide , fractionmass = 16*perCent);
  Macor -> AddMaterial(PotassiumOxide, fractionmass = 10*perCent);
  Macor -> AddMaterial(BoronOxide    , fractionmass = 7*perCent);
  Macor -> AddElement(F, fractionmass = 4*perCent);

  G4Material *lead_glass = nist_manager -> FindOrBuildMaterial("G4_GLASS_LEAD");
  /* Pre-defined material:
     density = 6.22 g/cm3
     mean excitation energy = 526.4 eV
     radiation length = 1.266 cm
     nuclear interaction length = 25.755 cm
     Composition by mass:
                   Lead     (Z = 82) - 0.751938
                   Oxygen   (Z = 8)  - 0.156453
                   Silicon  (Z = 14) - 0.080866
                   Titanium (Z = 22) - 0.008092
                   Arsenic  (Z = 33) - 0.002651
   */

  // Set default materials
  MirrorMaterial = SiliconCarbide;
  world_material_ = vacuum;
  FullEnergyDetectorMaterial= Scintillator;
  DeDxDetectorMaterial = SiliconMat;
  SiliconDetectorFrameMaterial = Kapton;
  ChamberMaterial = StainlessSteel;
  FoilMaterial = Beryllium;
  HoopMaterial = GlassyCarbon;
  MirrorMountMaterial= StainlessSteel;
  CoilsMaterial = StainlessSteel;
  Hoop7Material = Titanium;
  MCPMaterial = lead_glass;
}

void K37DetectorConstruction::SetMirrorMaterial(G4String materialChoice) {
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name
  G4Material*materialSelectedFromTable;
  for (size_t J = 0; J < theMaterialTable->size(); J++) {
    materialSelectedFromTable = (*theMaterialTable)[J];
    if (materialSelectedFromTable ->GetName() == materialChoice) {
      MirrorMaterial =materialSelectedFromTable;
      mirror_log ->SetMaterial(materialSelectedFromTable);
    }
  }
}

void K37DetectorConstruction::UpdateGeometry() {
  G4RunManager::GetRunManager() ->
    DefineWorldVolume(ConstructK37Experiment());
}

void K37DetectorConstruction::PrintMaterialList() {
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  G4cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
           << G4endl;
    G4cout << "----------------- Materials availabe ---------------------"
           << G4endl;

    for (size_t J = 0; J < theMaterialTable->size(); J++) {
        G4Material*  materialSelectedFromTable = (*theMaterialTable)[J];
        G4cout << materialSelectedFromTable ->GetName() << G4endl;
    }
    G4cout << "----------------- Materials availabe ---------------------"
           << G4endl;
    G4cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
           << G4endl;
}




