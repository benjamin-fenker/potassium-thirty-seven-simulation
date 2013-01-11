// Authors: Spencer Behling and Benjamin Fenker 2013

#include <CLHEP/Vector/Rotation.h>

#include "K37DetectorConstruction.hh"
#include "K37DetectorMessenger.hh"
#include "K37ScintillatorSD.hh"
#include "K37MirrorSD.hh"
#include "K37StripDetectorSD.hh"
#include "K37ElectricFieldSetup.hh"

// materials
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4PVReplica.hh"

// So that the geometry can be cleared between runs
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

K37DetectorConstruction::K37DetectorConstruction()
    : world_phys(0), detectorMessenger(0), mirror_log(0), MirrorMaterial(0),
      WorldMaterial(0), FullEnergyDetectorMaterial(0), DeDxDetectorMaterial(0),
      SiliconDetectorFrameMaterial(0), ChamberMaterial(0), FoilMaterial(0),
      HoopMaterial(0), MirrorMountMaterial(0), CoilsMaterial(0),
      Hoop7Material(0), MCPMaterial(0), MM_CollimatorCut_sol(0),
      fEmFieldSetup(0), shouldTheMirrorBeWFEDMCut(false), changeZtoX(0),
      changeZto45(0), changeZtoNeg45(0), changeZto35(0), changeZtoNeg35(0),
      changeZtoNeg62(0), changeZtoNeg118(0), changeYtoBeamAxis(0),
      changeYtoBeamAxisForLPP1(0), changeYtoBeamAxisForLPP2(0),
      changeYtoBeamAxisForLPP3(0), changeYtoBeamAxisForLPP4(0),
      rotationForOpticalPumpingBeams1(0), rotationForOpticalPumpingBeams2(0),
      RFDRotation(0), FFRFRotation(0), MirrorRotation(0), MirrorCutRotation(0),
      MMRotation(0), hoopRotation(0), world_log(0),
      scint_logVisAttributes_plusZ(0), scint_logVisAttributes_minusZ(0),
      dedx_logVisAttributes(0), dedx_logVisAttributes_minusZ(0),
      dedxFrame_logVisAttributes(0), chamber_logVisAttributes(0),
      OPRF_logVisAttributes(0), RFD_logVisAttributes(0),
      beryllium_logVisAttributes(0), FFRF_logVisAttributes(0),
      mirror_logVisAttributes(0), MM_logVisAttributes(0),
      cut_ESP_logVisAttributes(0), hoop7_logVisAttributes(0),
      SOED_logVisAttributes(0), coils_logVisAttributes(0),
      InvisibilityCloak(0) {
    this-> DefineMaterials();
    detectorMessenger = new K37DetectorMessenger(this);
    fEmFieldSetup = new K37ElectricFieldSetup();
    InvisibilityCloak = new G4VisAttributes(false);
    // ******SET CONSTRUCTION FLAGS HERE**************************************
    makeScintillators = true;     // Must be true!
    makeStripDetectors = true;    // Must be true!
    makeChamber = true;
    makeMirrors = true;
    makeHoops = true;
    makeElectronMCP = true;
    makeCoils = true;
}

K37DetectorConstruction::~K37DetectorConstruction() {
    delete detectorMessenger;
    delete fEmFieldSetup;
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
    delete world_log;
    delete scint_logVisAttributes_plusZ;
    delete scint_logVisAttributes_minusZ;
    delete dedx_logVisAttributes;
    delete dedx_logVisAttributes_minusZ;
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
    delete InvisibilityCloak;
    // Why would you ever want to throw away an invisibility cloak!? :>
}

G4VPhysicalVolume* K37DetectorConstruction::Construct() {
    return this->ConstructK37Experiment();
}
G4VPhysicalVolume* K37DetectorConstruction:: ConstructK37Experiment() {
    if (world_phys) {
        G4GeometryManager::GetInstance()->OpenGeometry();
        G4PhysicalVolumeStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4SolidStore::GetInstance()->Clean();
    }
    // ------------------------------------------ Sensitive Detector / filters

    /*------------------------------------------------------------------------
      The Sensitivity of each volume will be set at the time the volume is
      created. This will lead to what I believe to be a better organization of
      the code because every aspect of a volume will no be localized so that
      volumes can be added and subtracted more easily.
      -----------------------------------------------------------------------------*/
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    G4double world_x = 1.0*m;
    G4double world_y = 1.0*m;
    G4double world_z = 1.0*m;
    G4Box* world_box = new G4Box("world_box", world_x, world_y, world_z);
    world_log  = new G4LogicalVolume(world_box, WorldMaterial, "world_log",
                                     0, 0, 0);
    world_phys = new G4PVPlacement(0, G4ThreeVector(), world_log, "world_phys",
                                   0, false, 0);
    // world_log -> SetVisAttributes (G4VisAttributes::Invisible);
    world_log -> SetVisAttributes(InvisibilityCloak);

    if (makeScintillators) ConstructScintillators(SDman);
    if (makeStripDetectors) ConstructStripDetectors(SDman);
    if (makeChamber) ConstructChamber();
    if (makeMirrors) ConstructMirrors();
    if (makeHoops) ConstructHoops();
    if (makeElectronMCP) ConstructElectronMCP();
    if (makeCoils) ConstructCoils();
    if (makeCoils) {
    }  // End if(makeCoils)
    return world_phys;
}


void K37DetectorConstruction::ConstructScintillators(G4SDManager* SDman) {
  G4double Scint_rmax             = 45*mm;  // 90/2
  G4double Scint_rmin             = 0.*mm;
  G4double Scint_dz               = 35*mm;
  G4double Scint_startPhi = 0.*deg;
  G4double Scint_deltaPhi = 360.*deg;
  G4double Scint_zPosition= 117.5*mm;

  G4Tubs* scint_sol = new G4Tubs("scint_sol", Scint_rmin, Scint_rmax,
                                 (Scint_dz/2.)*mm, Scint_startPhi,
                                 Scint_deltaPhi);
  G4LogicalVolume* scint_plusZ_log =
    new G4LogicalVolume(scint_sol, FullEnergyDetectorMaterial,
                        "scint_plusZ_log", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., Scint_zPosition), scint_plusZ_log,
                    "scint_plusZ_phys", world_log, false, 0);

  scint_logVisAttributes_plusZ = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
  scint_logVisAttributes_plusZ -> SetForceSolid(true);
  scint_plusZ_log -> SetVisAttributes(scint_logVisAttributes_plusZ);

  G4LogicalVolume* scint_minusZ_log =
    new G4LogicalVolume(scint_sol, FullEnergyDetectorMaterial,
                        "scint_minusZ_log", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -Scint_zPosition),
                    scint_minusZ_log, "scint_minusZ_phys", world_log, false, 0);

  scint_logVisAttributes_minusZ = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  scint_logVisAttributes_minusZ-> SetForceSolid(true);
  scint_minusZ_log -> SetVisAttributes(scint_logVisAttributes_minusZ);
  // G4MultiFunctionalDetector* scintillatorMinusZ =
  //   new G4MultiFunctionalDetector("scintillatorMinusZ");
  // G4VPrimitiveScorer* ScintillatorEnergyDepositMinusZ =
  //   new G4PSEnergyDeposit("ScintillatorEnergyDeposited", 0);
  // primitive->SetFilter(epFilter);
  // scintillatorMinusZ ->
  //   RegisterPrimitive(ScintillatorEnergyDepositMinusZ);
  // SDman->AddNewDetector(scintillatorMinusZ);
  // scint_minusZ_log->SetSensitiveDetector(scintillatorMinusZ);
  G4String fullenergy1SDname = "/mydet/scintillatorPlusZ";
  K37ScintillatorSD * fullenergy1SD = new K37ScintillatorSD(fullenergy1SDname);
  SDman->AddNewDetector(fullenergy1SD);
  scint_plusZ_log->SetSensitiveDetector(fullenergy1SD);

  G4String fullenergy2SDname = "/mydet/scintillatorMinusZ";
  K37ScintillatorSD * fullenergy2SD =
    new K37ScintillatorSD(fullenergy2SDname);
  SDman->AddNewDetector(fullenergy2SD);
  scint_minusZ_log->SetSensitiveDetector(fullenergy2SD);
}

void K37DetectorConstruction::ConstructStripDetectors(G4SDManager* SDman) {
  G4double Dedx_x = 40.0*mm;
  G4double Dedx_y = 40.0*mm;
  G4double Dedx_z = 0.3*mm;

  G4double Dedx_zPosition = 98.5*mm;
  G4Box* dedx_sol = new G4Box("dedx_sol", (Dedx_x/2.),
                              (Dedx_y/2.), (Dedx_z/2.));
  G4ThreeVector dedx_PlusZ_pos = G4ThreeVector(0.0, 0.0, Dedx_zPosition);
  G4LogicalVolume* dedx_plusZ_log = new G4LogicalVolume(dedx_sol,
                                                        DeDxDetectorMaterial,
                                                        "dedx_plusZ_log",
                                                        0, 0, 0);
  new G4PVPlacement(0, dedx_PlusZ_pos, dedx_plusZ_log, "dedx_plusZ_phys",
                    world_log, false, 0);
  dedx_logVisAttributes = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
  dedx_logVisAttributes-> SetForceSolid(true);
  dedx_plusZ_log -> SetVisAttributes(dedx_logVisAttributes);
  // G4MultiFunctionalDetector* dedxDetectorPlusZ =
  //   new G4MultiFunctionalDetector("DedxDetectorPlusZ");
  // G4VPrimitiveScorer* dedxEnergyDepositPlusZ =
  //   new G4PSEnergyDeposit("DedxEnergyDeposited", 0);
  // dedxDetectorPlusZ->RegisterPrimitive(dedxEnergyDepositPlusZ);
  // SDman->AddNewDetector(dedxDetectorPlusZ);
  // dedx_plusZ_log->SetSensitiveDetector(dedxDetectorPlusZ);

  G4ThreeVector dedx_MinusZ_pos = G4ThreeVector(0.0, 0.0, -Dedx_zPosition);
  G4LogicalVolume* dedx_MinusZ_log = new G4LogicalVolume(dedx_sol,
                                                         DeDxDetectorMaterial,
                                                         "dedx_MinusZ_log", 0,
                                                         0, 0);
  new G4PVPlacement(0, dedx_MinusZ_pos, dedx_MinusZ_log, "dedx_minusZ_phys",
                    world_log, false, 0);
  dedx_logVisAttributes_minusZ = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  // G4VisAttributes* dedx_logVisAttributes_minusZ =
  //   new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));

  dedx_logVisAttributes_minusZ-> SetForceSolid(true);

  dedx_MinusZ_log -> SetVisAttributes(dedx_logVisAttributes_minusZ);

  // G4MultiFunctionalDetector* dedxDetectorMinusZ =
  //   new G4MultiFunctionalDetector("DedxDetectorMinusZ");
  // G4VPrimitiveScorer* dedxEnergyDepositMinusZ =
  //   new G4PSEnergyDeposit("DedxEnergyDeposited", 0);
  // dedxDetectorMinusZ->RegisterPrimitive(dedxEnergyDepositMinusZ);
  // SDman->AddNewDetector(dedxDetectorMinusZ);
  // dedx_MinusZ_log->SetSensitiveDetector(dedxDetectorMinusZ);
  G4String dedx1SDname = "/mydet/dsssdPlusZ";
  K37StripDetectorSD * dedx1SD = new K37StripDetectorSD(dedx1SDname);
  //                        pos_of_center, numStrips, stripWidth
  dedx1SD -> SetupParameters(dedx_PlusZ_pos, 40, 1.0*mm);
  SDman->AddNewDetector(dedx1SD);
  dedx_plusZ_log->SetSensitiveDetector(dedx1SD);

  G4String dedx2SDname = "/mydet/dsssdMinusZ";
  K37StripDetectorSD * dedx2SD = new K37StripDetectorSD(dedx2SDname);
  //                        pos_of_center, numStrips, stripWidth
  dedx2SD -> SetupParameters(dedx_MinusZ_pos, 40, 1.0*mm);
  SDman->AddNewDetector(dedx2SD);
  dedx_MinusZ_log->SetSensitiveDetector(dedx2SD);

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
  dedxFrame_logVisAttributes = new G4VisAttributes(G4Colour(1.0, 0.7, 0.5));
  dedxFrame_logVisAttributes-> SetForceSolid(true);
  dedxFrame_log -> SetVisAttributes(dedxFrame_logVisAttributes);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 98.5*mm), dedxFrame_log,
                    "dedxFrame_plusZ_phys", world_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -98.5*mm), dedxFrame_log,
                    "dedxFrame_minusZ_phys", world_log, false, 0);
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

  //      changeZtoX = new G4RotationMatrix();
  changeZtoX = new CLHEP::HepRotation();
  changeZtoX->rotateX(90.*deg);

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

  // ------------------------------ Final Product after all of the cuts.

  G4LogicalVolume* chamber_log = new G4LogicalVolume(ChamberCut17_sol,
                                                     ChamberMaterial,
                                                     "chamber_log",
                                                     0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), chamber_log,
                    "chamber_phys", world_log, false, 0);

  chamber_logVisAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  // chamber_logVisAttributes-> SetForceSolid(true);
  chamber_logVisAttributes-> SetForceWireframe(true);

  chamber_log -> SetVisAttributes(chamber_logVisAttributes);
  chamber_log -> SetVisAttributes(InvisibilityCloak);

  // ------------------------------ Optical Pumping Rentrant Flanges
  // (z+ and z-) (OPRF)


  G4VSolid * OPRF_sol = new G4Tubs("OPRF_sol", (98.298/2.0)*mm, (101.6/2.0)*mm,
                                   (160.0/2.0)*mm, 0.0*deg, 360.0*deg);

  G4LogicalVolume * OPRF_log = new G4LogicalVolume(OPRF_sol,
                                                   ChamberMaterial,
                                                   "OPRF_log", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 182*mm), OPRF_log,
                    "OPRF_plusZ_phys", world_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -182*mm), OPRF_log,
                    "OPRF_minusZ_phys", world_log, false, 0);
  OPRF_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.0));
  OPRF_logVisAttributes-> SetForceSolid(true);
  // chamber_logVisAttributes-> SetForceWireframe(true);
  OPRF_log -> SetVisAttributes(OPRF_logVisAttributes);

  // ------------------------------ Rentrant Flange Descender (RFD)

  G4VSolid * RFD_sol = new G4Cons("RFD_sol", 47.431*mm, 48.951*mm, 49.153*mm,
                                  50.673*mm, 2.5*mm, 0.0*deg, 360.0*deg);
  G4LogicalVolume* RFD_log = new G4LogicalVolume(RFD_sol, ChamberMaterial,
                                                 "RFD_log", 0, 0, 0);
  RFD_logVisAttributes = new G4VisAttributes(G4Colour(0.8, 0.4, 0.5));
  RFD_logVisAttributes-> SetForceSolid(true);
  RFD_log -> SetVisAttributes(RFD_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 99.5*mm), RFD_log,
                    "RFD_plusZ_phys", world_log, false, 0);


  // G4RotationMatrix* RFDRotation = new G4RotationMatrix();
  RFDRotation = new CLHEP::HepRotation();
  RFDRotation->rotateX(180.*deg);

  new G4PVPlacement(RFDRotation, G4ThreeVector(0.0, 0.0, -99.5*mm), RFD_log,
                    "RFD_minusZ_phys", world_log, false, 0);

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
                    beryllium_log, "beryllium_plusZ_phys", world_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -(97.0-(0.229/2.0))*mm),
                    beryllium_log, "beryllium_minusZ_phys",
                    world_log, false, 0);

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

  FFRF_logVisAttributes = new G4VisAttributes(G4Colour(0.8, 0.4, 0.5));
  // G4VisAttributes* FFRF_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.8, 0.4, 0.5));

  FFRF_logVisAttributes-> SetForceSolid(true);

  FFRF_log -> SetVisAttributes(FFRF_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 94.5*mm), FFRF_log,
                    "FFRF_plusZ_phys", world_log, false, 0);

  // G4RotationMatrix* FFRFRotation = new G4RotationMatrix();
  FFRFRotation = new CLHEP::HepRotation();
  FFRFRotation->rotateX(180.*deg);

  new G4PVPlacement(FFRFRotation, G4ThreeVector(0.0, 0.0, -94.5*mm),
                    FFRF_log, "FFRF_minusZ_phys", world_log, false, 0);
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
                    mirror_log, "mirror_plusZ_phys", world_log, false, 0);
  new G4PVPlacement(MirrorRotation,
                    G4ThreeVector(0, 0, -MirrorPositionZ), mirror_log,
                    "mirror_minusZ_phys", world_log, false, 0);
  mirror_logVisAttributes = new G4VisAttributes(G4Colour(0.1, 0.8, 0.1));
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

  MM_logVisAttributes = new G4VisAttributes(G4Colour(0.2, 0.9, 0.5));
  // G4VisAttributes* MM_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.9, 0.5));

  // MM_logVisAttributes-> SetForceSolid(true);
  MM_logVisAttributes-> SetForceWireframe(true);
  MM_log -> SetVisAttributes(MM_logVisAttributes);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 85.0*mm), MM_log,
                    "MM_plusZ_phys", world_log, false, 0);
  // G4RotationMatrix* MMRotation = new G4RotationMatrix();
  MMRotation = new CLHEP::HepRotation();
  MMRotation->rotateX(180.*deg);

  new G4PVPlacement(MMRotation, G4ThreeVector(0.0, 0.0, -85.0*mm), MM_log,
                    "MM_minusZ_phys", world_log, false, 0);
}  // End ConstructMirrors

void K37DetectorConstruction::ConstructHoops() {
  G4double hoop_rmax = 91./2.     *mm;
  G4double hoop_rmin = 0          *mm;
  G4double hoop_dz   = 1.         *mm;
  G4double hoop_Sphi = 0.         *deg;
  G4double hoop_Dphi = 360.       *deg;

  G4Tubs * hoop_sol = new G4Tubs("hoop_sol", hoop_rmin, hoop_rmax,
                                 hoop_dz, hoop_Sphi, hoop_Dphi);

  G4RotationMatrix* hoopRotation = new G4RotationMatrix();
  hoopRotation->rotateX(90.*deg);

  G4double hoop_x = (140./2.)*mm;
  G4double hoop_y = (106./2.)*mm;
  G4double hoop_z = (1./2.)*mm;

  G4Box * electrostaticplate_sol = new G4Box("electrostaticplate_sol",
                                             hoop_x , hoop_y, hoop_z);

  G4SubtractionSolid* cut_ESP_sol =
    new G4SubtractionSolid("cut_ESP_sol", electrostaticplate_sol, hoop_sol);

  G4LogicalVolume *cut_ESP_log = new G4LogicalVolume(cut_ESP_sol, HoopMaterial,
                                                     "cut_ESP_log", 0, 0, 0);
  G4VisAttributes* cut_ESP_logVisAttributes =
    new G4VisAttributes(G4Colour(0.2, 0.8, 0.1));
  cut_ESP_logVisAttributes-> SetForceSolid(true);
  // MM_logVisAttributes-> SetForceWireframe(true);
  cut_ESP_log -> SetVisAttributes(cut_ESP_logVisAttributes);

  G4double cut_ESP_ypos_0 = 49*mm;
  G4double cut_ESP_ypos_1 = 27.5*mm;
  G4double cut_ESP_ypos_2 = -27.5*mm;
  G4double cut_ESP_ypos_3 = -49*mm;

  new G4PVPlacement(hoopRotation, G4ThreeVector(0., cut_ESP_ypos_0, 0.),
                    cut_ESP_log, "cut_ESP_phys_0", world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., cut_ESP_ypos_1, 0.),
                    cut_ESP_log, "cut_ESP_phys_1", world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., cut_ESP_ypos_2, 0.),
                    cut_ESP_log, "cut_ESP_phys_2", world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., cut_ESP_ypos_3, 0.),
                    cut_ESP_log, "cut_ESP_phys_3", world_log, false, 0);

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
                        HoopMaterial, "hoop_2_log", 0, 0, 0);
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
  cut_ESP_logVisAttributes = new G4VisAttributes(G4Colour(0.2, 0.8, 0.1));
  // G4VisAttributes* cut_ESP_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.8, 0.1));
  cut_ESP_logVisAttributes-> SetForceSolid(true);
  hoop7_logVisAttributes = new G4VisAttributes(G4Colour(0.2, 0.1, 0.6));
  // G4VisAttributes* hoop7_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.1, 0.6));
  hoop7_logVisAttributes-> SetForceSolid(true);

  hoop_3through6_box_log ->
    SetVisAttributes(cut_ESP_logVisAttributes);  // 3-6
  hoop_7_log -> SetVisAttributes(hoop7_logVisAttributes);    // 7
  hoop_2_log -> SetVisAttributes(cut_ESP_logVisAttributes);  // 2
  hoop_1_log -> SetVisAttributes(cut_ESP_logVisAttributes);  // 1

  // G4RotationMatrix* hoopRotation = new G4RotationMatrix();
  hoopRotation = new CLHEP::HepRotation();
  hoopRotation->rotateX(90.*deg);

  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 57.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_3",
                    world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 29.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_4",
                    world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -29.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_5",
                    world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -57.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_6",
                    world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -75.5*mm, 0.),
                    hoop_7_log, "hoop_7_phys", world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 75.5*mm, 0.),
                    hoop_2_log, "hoop_2_phys", world_log, false, 0);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 97.5*mm, 0.),
                    hoop_1_log, "hoop_1_phys", world_log, false, 0);
}  // End construct hoops

void K37DetectorConstruction::ConstructElectronMCP() {
  G4double SOED_rmax = 80./2.*mm;
  G4double SOED_rmin = 0     *mm;
  G4double SOED_dz   = 10./2.*mm;
  G4double SOED_Sphi = 0.    *deg;
  G4double SOED_Dphi = 360.  *deg;

  G4Tubs * SOED_sol = new G4Tubs("SOED_sol", SOED_rmin, SOED_rmax,  SOED_dz,
                                 SOED_Sphi, SOED_Dphi);
  G4LogicalVolume * SOED_log = new G4LogicalVolume(SOED_sol, MCPMaterial,
                                                   "SOED_log", 0, 0, 0);
  new G4PVPlacement(changeZtoX, G4ThreeVector(0., -82.0*mm, 0), SOED_log,
                    "SOED_phys", world_log, false, 0);
  SOED_logVisAttributes = new G4VisAttributes(G4Colour(0.1, 0.8, 0.8));
  SOED_logVisAttributes-> SetForceSolid(true);
  SOED_log -> SetVisAttributes(SOED_logVisAttributes);
}  // End construct EMCP

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
                    "coils_plusZ_phys", world_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -65.), coils_log,
                    "coils_minusZ_phys", world_log, false, 0);

  coils_logVisAttributes= new G4VisAttributes(G4Colour(0.2, 0.3, 0.1));
  // G4VisAttributes* coils_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.2, 0.3, 0.1));

  coils_logVisAttributes-> SetForceSolid(true);
  // MM_logVisAttributes-> SetForceWireframe(true);

  coils_log -> SetVisAttributes(coils_logVisAttributes);
}  // End construct coils

void K37DetectorConstruction::DefineMaterials() {
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

  G4Material* Vac = new G4Material("Vac ", z = 1., a = 1.01*g/mole,
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

  // Set default materials
  MirrorMaterial = SiliconCarbide;
  WorldMaterial = Vac;
  FullEnergyDetectorMaterial= Scintillator;
  DeDxDetectorMaterial = SiliconMat;
  SiliconDetectorFrameMaterial = Kapton;
  ChamberMaterial = StainlessSteel;
  FoilMaterial = Beryllium;
  HoopMaterial = GlassyCarbon;
  MirrorMountMaterial= StainlessSteel;
  CoilsMaterial = StainlessSteel;
  Hoop7Material = StainlessSteel;
  MCPMaterial = SiliconMat;
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
    DefineWorldVolume(this -> ConstructK37Experiment());
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




