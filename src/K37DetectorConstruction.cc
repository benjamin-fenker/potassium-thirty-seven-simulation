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
      Hoop7Material(0), MCPMaterial(0), electron_mcp_radius_(20.0*mm),
      shouldTheMirrorBeWFEDMCut(false), check_all_for_overlaps_(false),
      changeZtoX(0),
      changeZto45(0), changeZtoNeg45(0), changeZto35(0), changeZtoNeg35(0),
      changeZtoNeg62(0), changeZtoNeg118(0), changeYtoBeamAxis(0),
      changeYtoBeamAxisForLPP1(0), changeYtoBeamAxisForLPP2(0),
      changeYtoBeamAxisForLPP3(0), changeYtoBeamAxisForLPP4(0),
      rotationForOpticalPumpingBeams1(0), rotationForOpticalPumpingBeams2(0),
      RFDRotation(0), FFRFRotation(0), MirrorCutRotation(0),
      hoopRotation(0), dedxFrame_logVisAttributes(0),
      chamber_logVisAttributes(0), OPRF_logVisAttributes(0),
      RFD_logVisAttributes(0), beryllium_logVisAttributes(0),
      FFRF_logVisAttributes(0), mirror_logVisAttributes(0),
      MM_logVisAttributes(0), cut_ESP_logVisAttributes(0),
      hoop7_logVisAttributes(0), SOED_logVisAttributes(0),
      coils_logVisAttributes(0), rmcp_logVisAttributes_(0) {

  // Default values
  world_size_ = 2.0 * m;

  // Define the inch
  static const G4double inch = 2.54*cm;

  // *************** All default dimensions should be placed here **************
  // *** Any derived dimnesions will be calcualted in a function below *********

  // Mirror dimensions from meeting website on 30-Jan-2014
  // Placement from detectorDrawing.pdf
  mirror.inner_radius = 0.0*inch;       // Its solid - duh
  mirror.outer_radius = 1.2*inch;       // 30-Jan-2014 meeting
  mirror.length = 0.25 * mm;            // 30-Jan-2014 meeting
  mirror.rotation_angle = 9.5*deg;      // 30-Jan-2014 meeting
  // Mirror mount dimensions from collimator_back.pdf & collimator_front.pdf
  mirror_mount.inner_radius = 0.0*inch; // Solid to start
  mirror_mount.outer_radius = (2.913/2.0) * inch; // collimator_{front,back}.pdf
  // Take the fat side of one slice and add to it the thin side of the other
  // slice
  G4double lip =
      (0.519*inch) - (2.0*mirror_mount.outer_radius)*tan(mirror.rotation_angle);
  mirror_mount.length = (0.519*inch) + lip;
  mirror_mount.cutout_side_length = 1.220 * inch;
  mirror_mount.cutout_radius = 2.441/2.0 * inch;
  mirror_mount.cutout_depth = 0.040 * inch;

  // ***********************************************************
  // The reentrant flange manufactured by Brush-Wellman
  // Described in reentrant_flange.pdf
  // Data from the drawings
  reentrant_flange_front_face.inner_radius = (58.00/2.0)*mm;
  reentrant_flange_front_face.outer_radius = (94.59/2.0)*mm;
  beryllium_window.inner_radius = 0.0*mm;
  beryllium_window.outer_radius = (58.00/2.0) * mm;
  beryllium_window.length = 0.229 * mm;
  reentrant_flange_descender.inner_radius2 = (98.56*mm)/2.0;
  reentrant_flange_descender.outer_radius2 = (101.60*mm)/2.0;
  reentrant_flange_descender.length = 10.00*mm;
  reentrant_flange_pipe.inner_radius = (98.56*mm)/2.0;
  reentrant_flange_pipe.outer_radius = (101.60*mm)/2.0;
  reentrant_flange_pipe.length = 130.0*mm;
  reentrant_flange_front_face.length = 10.00 * mm;


  // The SD frame is defined in the file BB1.pdf (the 3.0 mm thickness
  // is measured by hand I think)
  sd_frame.length = 55.0*mm;            // ???
  sd_frame.width  = 55.0*mm;            // ???
  sd_frame.depth  = 3.0*mm;
  sd_frame.cutout_side_length = 44.4*mm; // Enough for the entire chip
  sd_frame.cutout_depth = 1.05*sd_frame.depth; // All the way through
  //  sd_frame.center_position = G4ThreeVector(0.0, 0.0, 98.5*mm);

  strip_detector.length = 40.0*mm;
  strip_detector.width  = 40.0*mm;
  strip_detector.depth  = 0.3*mm;

  // There is non-active area of silicon that should be included in
  // the simulation.  See BB1.pdf
  sd_inactive.length = 44.4*mm;
  sd_inactive.width  = 44.4*mm;
  sd_inactive.depth  = 0.3*mm;

  // The SD is mounted to the flange by 4x 2-56 screws.  The socket
  // cap heads stick up out ouf the SD frame and the scintillator
  // rests against these screws.  This creates a small gap between the
  // silicon and scintillator.   See BB1.pdf for palcement
  // 0.082" = 2-56 socket head cap screw height
  // 0.020" = #2 flat washer height
  sd_mounting_screw_head.inner_radius = 0.0*mm;
  sd_mounting_screw_head.outer_radius = (0.137/2.0) * inch;
  sd_mounting_screw_head.length = (0.082 + 0.020) * inch;

  // ***********************************************************

  // Teflon front face and scintillator
  // The teflon front face covers the entire front face of the
  // scintillator. The covering is assumed to be completely uniform
  // and flat mostly because modeling it as anything else would be
  // impossible.  The thickness (90 um) comes from a perusing of the
  // internet and could be off in reality by 10s of microns.  
  scintillator.inner_radius = 0.0;
  scintillator.outer_radius = (90.0/2.0) * mm;

  teflon_front_face.length = 90.0*um;
  // teflon_front_face.length = 5.0*mm;

  scintillator.length = 3.5 * cm;
  // ***********************************************************

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
    make_sd_holders_ = true;
}

K37DetectorConstruction::~K37DetectorConstruction() {
  if (detectorMessenger) delete detectorMessenger;
  if (changeZtoX) delete changeZtoX;
  if (changeZto45) delete changeZto45;
  if (changeZtoNeg45) delete changeZtoNeg45;
  if (changeZto35) delete changeZto35;
  if (changeZtoNeg35) delete changeZtoNeg35;
  if (changeZtoNeg62) delete changeZtoNeg62;
  if (changeZtoNeg118) delete changeZtoNeg118;
  if (changeYtoBeamAxis) delete changeYtoBeamAxis;
  if (changeYtoBeamAxisForLPP1) delete changeYtoBeamAxisForLPP1;
  if (changeYtoBeamAxisForLPP2) delete changeYtoBeamAxisForLPP2;
  if (changeYtoBeamAxisForLPP3) delete changeYtoBeamAxisForLPP3;
  if (changeYtoBeamAxisForLPP4) delete changeYtoBeamAxisForLPP4;
  if (rotationForOpticalPumpingBeams1) delete rotationForOpticalPumpingBeams1;
  if (rotationForOpticalPumpingBeams2) delete rotationForOpticalPumpingBeams2;
  if (RFDRotation) delete RFDRotation;
  if (FFRFRotation)delete FFRFRotation;
  if (MirrorCutRotation) delete MirrorCutRotation;
  if (hoopRotation) delete hoopRotation;
  if (world_log_) delete world_log_;
  if (world_phys_) delete world_phys_;
  if (scintillator_tubs_) delete scintillator_tubs_;
  if (upper_scintillator_log_) delete upper_scintillator_log_;
  if (lower_scintillator_log_) delete lower_scintillator_log_;
  if (strip_detector_box_) delete strip_detector_box_;
  if (upper_strip_detector_log_) delete upper_strip_detector_log_;
  if (lower_strip_detector_log_) delete lower_strip_detector_log_;
  if (chamber_box_) delete chamber_box_;
  if (chamber_log_) delete chamber_log_;
  if (electron_mcp_tub_) delete electron_mcp_tub_;
  if (electron_mcp_log_) delete electron_mcp_log_;
  if (recoil_mcp_tub_) delete recoil_mcp_tub_;
  if (recoil_mcp_log_) delete recoil_mcp_log_;
  if (dedxFrame_logVisAttributes) delete dedxFrame_logVisAttributes;
  if (chamber_logVisAttributes) delete chamber_logVisAttributes;
  if (OPRF_logVisAttributes) delete OPRF_logVisAttributes;
  if (RFD_logVisAttributes) delete RFD_logVisAttributes;
  if (beryllium_logVisAttributes) delete beryllium_logVisAttributes;
  if (FFRF_logVisAttributes) delete FFRF_logVisAttributes;
  if (mirror_logVisAttributes) delete mirror_logVisAttributes;
  if (MM_logVisAttributes) delete MM_logVisAttributes;
  if (cut_ESP_logVisAttributes) delete cut_ESP_logVisAttributes;
  if (hoop7_logVisAttributes) delete hoop7_logVisAttributes;
  if (SOED_logVisAttributes) delete SOED_logVisAttributes;
  if (coils_logVisAttributes) delete coils_logVisAttributes;
  if (rmcp_logVisAttributes_) delete rmcp_logVisAttributes_;
}

G4VPhysicalVolume* K37DetectorConstruction::Construct() {
    return this->ConstructK37Experiment();
}
G4VPhysicalVolume* K37DetectorConstruction:: ConstructK37Experiment() {
  G4GeometryManager::GetInstance()->OpenGeometry();
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


    CalculateDimensions();
    ConstructAir();
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

void K37DetectorConstruction::CalculateDimensions() {
  // The FRONT of the mirror is 85 mm away so the center is slightly beyond that
  G4double z = (85.0*mm) + ((mirror.length/2.0)/cos(mirror.rotation_angle));
  mirror.center_position = G4ThreeVector(0.0, 0.0, z);
  mirror_mount.center_position = mirror.center_position;
  // Derived data
  reentrant_flange_front_face.inner_radius2 =
      reentrant_flange_front_face.inner_radius;

  G4double outer_radius_change = reentrant_flange_pipe.outer_radius -
      reentrant_flange_front_face.outer_radius;
  reentrant_flange_front_face.outer_radius2 =
      reentrant_flange_front_face.outer_radius + (0.5*outer_radius_change);
  G4double zpos = mirror_mount.center_position.z() +
      (0.5*mirror_mount.length) + (0.5*reentrant_flange_front_face.length);
  reentrant_flange_front_face.center_position = G4ThreeVector(0.0, 0.0, zpos);

  // ***********************************************************
  // The beryllium window is defined in reentrant_flange.pdf
  //  beryllium_window.center_position = G4ThreeVector(0.0, 0.0, 96.89*mm);
  zpos = reentrant_flange_front_face.center_position.z() +
      (0.5*reentrant_flange_front_face.length) - (0.5*beryllium_window.length);
  beryllium_window.center_position = G4ThreeVector(0.0, 0.0, zpos);

  // Reentrant flange descender
  reentrant_flange_descender.inner_radius =
      reentrant_flange_front_face.outer_radius;
  reentrant_flange_descender.outer_radius =
      reentrant_flange_front_face.outer_radius2;
  zpos = reentrant_flange_front_face.center_position.z() +
      (0.5*reentrant_flange_front_face.length) +
      (0.5*reentrant_flange_descender.length);
  reentrant_flange_descender.center_position = G4ThreeVector(0.0, 0.0, zpos);

  // Reentrant flange pipe
  zpos = reentrant_flange_descender.center_position.z() +
      (0.5*reentrant_flange_descender.length) + (0.5*reentrant_flange_pipe.length);
  reentrant_flange_pipe.center_position = G4ThreeVector(0, 0, zpos);

  sd_frame.center_position =
      G4ThreeVector(0.0, 0.0, reentrant_flange_front_face.center_position.z() +
                    (0.5*reentrant_flange_front_face.length) +
                    (0.5*sd_frame.depth));

  strip_detector.center_position = sd_frame.center_position;
  sd_inactive.center_position = sd_frame.center_position;

  // There are 4 of them with different x,y coordinates but the same
  // z-coordinate.  Define just that z-coordinate now
  zpos = sd_frame.center_position.z();
  zpos += (sd_frame.depth/2.0);
  zpos += (sd_mounting_screw_head.length/2.0);
  G4double off_center = 25.0*mm;        // x,y dimensions away from
                                        // the axis
  sd_mounting_screw_head.center_position =
      G4ThreeVector(off_center, off_center, zpos);
  teflon_front_face.inner_radius = scintillator.inner_radius;
  teflon_front_face.outer_radius = scintillator.outer_radius;

  zpos = sd_frame.center_position.z();
  zpos += (sd_frame.depth/2.0);
  zpos += sd_mounting_screw_head.length;
  zpos += (teflon_front_face.length/2.0);
  teflon_front_face.center_position = G4ThreeVector(0.0, 0.0, zpos);


  zpos = teflon_front_face.center_position.z();
  zpos += (0.5*teflon_front_face.length);
  zpos += (0.5*scintillator.length);
  scintillator.center_position = G4ThreeVector(0.0, 0.0, zpos);

  G4cout << "*******************************************************" << G4endl;
  G4cout << "**************** Geometry Definitions: ****************" << G4endl;
  G4cout << "   Mirror center to chamber center: "
         << G4BestUnit(mirror.center_position.z(), "Length") << G4endl;
  G4cout << "   Strip detector front to chamber center: "
         << G4BestUnit(strip_detector.center_position.z() -
                       (0.5*strip_detector.depth), "Length")
         << G4endl;
  // G4cout << "   Teflon front to chamber center: "
  //        << G4BestUnit(teflon_front_face.center_position.z() -
  //                      (0.5*teflon_front_face.length), "Length")
  //        << G4endl;
  G4cout << "   Scintillator front to chamber center: "
         << G4BestUnit(scintillator.center_position.z() -
                       (0.5*scintillator.length), "Length") << G4endl;
  G4cout << "*******************************************************" << G4endl;
}

void K37DetectorConstruction::ConstructScintillators(G4SDManager* SDman) {
  // Use same solid for both detectors :-)
  scintillator_tubs_ = new G4Tubs("scint_sol", scintillator.inner_radius,
                                  scintillator.outer_radius,
                                  scintillator.length/2.0, 0.0, 2.0*M_PI);

  // Use same vis attributes for both detectors :-)
  G4VisAttributes *scintillator_vis =
      new G4VisAttributes(G4Colour(0.11, 0.11, 0.44));
  scintillator_vis -> SetForceSolid(true);
  G4ThreeVector pos = -1.0*(scintillator.center_position -
                            reentrant_flange_pipe.center_position);

  upper_scintillator_log_ =  new G4LogicalVolume(scintillator_tubs_,
                                                 FullEnergyDetectorMaterial,
                                                 "scint_plusZ_log", 0, 0, 0);

  upper_scintillator_phys_ =
      new G4PVPlacement(0, pos,
                        upper_scintillator_log_, "scint_plusZ_phys",
                        air_log_plus_, false, 0, check_all_for_overlaps_);
  upper_scintillator_log_ -> SetVisAttributes(scintillator_vis);

  lower_scintillator_log_ = new G4LogicalVolume(scintillator_tubs_,
                                                FullEnergyDetectorMaterial,
                                                "scint_minusZ_log", 0, 0, 0);
  lower_scintillator_phys_ =
      new G4PVPlacement(0, pos,
                        lower_scintillator_log_, "scint_minusZ_phys",
                        air_log_mins_, false, 0, check_all_for_overlaps_);
  lower_scintillator_log_ -> SetVisAttributes(scintillator_vis);

  // Set up sensitive detectors
  G4String fullenergy1SDname = "/mydet/scintillatorPlusZ";

  if (!upper_scintillator_sens_) {
    upper_scintillator_sens_ = new K37ScintillatorSD(fullenergy1SDname);
    SDman->AddNewDetector(upper_scintillator_sens_);
  }

  upper_scintillator_log_ -> SetSensitiveDetector(upper_scintillator_sens_);

  G4String fullenergy2SDname = "/mydet/scintillatorMinusZ";

  if (!lower_scintillator_sens_) {
    lower_scintillator_sens_ = new K37ScintillatorSD(fullenergy2SDname);
    SDman->AddNewDetector(lower_scintillator_sens_);
  }

  lower_scintillator_log_ -> SetSensitiveDetector(lower_scintillator_sens_);

  // Teflon front face
  G4Tubs *teflon_tubs = new G4Tubs("teflon_tubs",
                                   teflon_front_face.inner_radius,
                                   teflon_front_face.outer_radius,
                                   teflon_front_face.length/2.0,
                                   0.0, 2*M_PI*rad);

  G4LogicalVolume *teflon_log =
      new G4LogicalVolume(teflon_tubs,
                          trinat_materials_.find("G4_TEFLON") -> second,
                          "teflon_front_face", 0, 0, 0);
  // white
  G4VisAttributes *teflon_vis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  teflon_vis -> SetForceSolid(true);
  teflon_log -> SetVisAttributes(teflon_vis);

  pos = -1.0*(teflon_front_face.center_position -
         reentrant_flange_pipe.center_position);
  
  new G4PVPlacement(0, pos, teflon_log,
                    "teflon_plus_z", air_log_plus_, false, 0,
                    check_all_for_overlaps_);
  new G4PVPlacement(0, pos, teflon_log,
                    "teflon_mins_z", air_log_mins_, false, 0,
                    check_all_for_overlaps_);
}

void K37DetectorConstruction::ConstructStripDetectors(G4SDManager* SDman) {
  // Same solid for both detectors :-)
  strip_detector_box_ = new G4Box("dedx_sol", strip_detector.length/2.0,
                                  strip_detector.width/2.0,
                                  strip_detector.depth/2.0);
  // Same vis for both detectors :-)
  G4VisAttributes *strip_detector_vis =
      new G4VisAttributes(G4Colour(0.96, 0.96, 0.0, 0.5)); // yellow

  strip_detector_vis -> SetForceSolid(true);
  upper_strip_detector_log_ = new G4LogicalVolume(strip_detector_box_,
                                                  DeDxDetectorMaterial,
                                                  "dedx_plusZ_log", 0, 0, 0);
  upper_strip_detector_log_ -> SetVisAttributes(strip_detector_vis);
  G4ThreeVector pos = reentrant_flange_pipe.center_position -
      strip_detector.center_position;
  upper_strip_detector_phys_ = new G4PVPlacement(0,
                                                 pos,
                                                 upper_strip_detector_log_,
                                                 "dedx_plusZ_phys",
                                                 air_log_plus_, false, 0,
                                                 check_all_for_overlaps_);

  lower_strip_detector_log_ = new G4LogicalVolume(strip_detector_box_,
                                                  DeDxDetectorMaterial,
                                                  "dedx_MinusZ_log", 0,
                                                  0, 0);
  lower_strip_detector_log_ -> SetVisAttributes(strip_detector_vis);
  lower_strip_detector_phys_ = new G4PVPlacement(0,
                                                 pos,
                                                 lower_strip_detector_log_,
                                                 "dedx_minusZ_phys", air_log_mins_,
                                                 false, 0, check_all_for_overlaps_);


  G4String dedx1SDname = "/mydet/dsssdPlusZ";

  if (!upper_strip_detector_sens_) {
    upper_strip_detector_sens_ = new K37StripDetectorSD(dedx1SDname);
    //                        pos_of_center, numStrips, stripWidth
    upper_strip_detector_sens_ -> SetupParameters(strip_detector.center_position,
                                                  40, 1.0*mm);
    SDman->AddNewDetector(upper_strip_detector_sens_);
  }
  upper_strip_detector_log_->SetSensitiveDetector(upper_strip_detector_sens_);

  G4String dedx2SDname = "/mydet/dsssdMinusZ";

  if (!lower_strip_detector_sens_) {
    lower_strip_detector_sens_ = new K37StripDetectorSD(dedx2SDname);
    //                        pos_of_center, numStrips, stripWidth
    lower_strip_detector_sens_ -> SetupParameters(-1.0*strip_detector.center_position,
                                                  40, 1.0*mm);
    SDman->AddNewDetector(lower_strip_detector_sens_);
  }
  lower_strip_detector_log_->SetSensitiveDetector(lower_strip_detector_sens_);

  // ------------------------------ dedx mount
  if (make_sd_holders_) {
    G4VSolid * dedx_holder_sol = new G4Box("dedx_holder_sol",
                                           sd_frame.length/2.0,
                                           sd_frame.width/2.0,
                                           sd_frame.depth/2.0);
    G4VSolid * dedx_holder_cut_sol = new G4Box("dedx_holder_cut_sol",
                                               sd_frame.cutout_side_length/2.0,
                                               sd_frame.cutout_side_length/2.0,
                                               sd_frame.cutout_depth/2.0);
    G4SubtractionSolid* dedxFrame_sol =
        new G4SubtractionSolid("dedxFrame_sol", dedx_holder_sol,
                               dedx_holder_cut_sol);
    G4LogicalVolume* dedxFrame_log =
        new G4LogicalVolume(dedxFrame_sol, SiliconDetectorFrameMaterial,
                            "dedxFrame_log", 0, 0, 0);
    dedxFrame_logVisAttributes =
        new G4VisAttributes(G4Colour(0.85, 0.65, 0.13));
    dedxFrame_logVisAttributes-> SetForceSolid(true);
    dedxFrame_log -> SetVisAttributes(dedxFrame_logVisAttributes);
    pos = reentrant_flange_pipe.center_position - sd_frame.center_position;
    new G4PVPlacement(0, pos, dedxFrame_log,
                      "dedxFrame_plusZ_phys", air_log_plus_, false, 0,
                      check_all_for_overlaps_);
    new G4PVPlacement(0, pos, dedxFrame_log,
                      "dedxFrame_minusZ_phys", air_log_mins_, false, 0,
                      check_all_for_overlaps_);
    // Add the inactive Si inside the frame
    G4Box *sd_inactive_box = new G4Box("sd_inactive_box",
                                       sd_inactive.length/2.0,
                                       sd_inactive.width/2.0,
                                       sd_inactive.depth/2.0);
    G4SubtractionSolid *sd_inactive_sub =
        new G4SubtractionSolid("sd_inactive_sub",
                               sd_inactive_box,
                               strip_detector_box_);
    G4LogicalVolume *sd_inactive_log = new G4LogicalVolume(sd_inactive_sub,
                                                           DeDxDetectorMaterial,
                                                           "sd_inactive_log");
    G4VisAttributes *sd_inactive_vis 
        = new G4VisAttributes(G4Colour(0.5, 0.5, 0.0));
    sd_inactive_vis -> SetForceSolid(true);
    sd_inactive_log -> SetVisAttributes(sd_inactive_vis);
    pos = reentrant_flange_pipe.center_position - sd_inactive.center_position;
    new G4PVPlacement(0, pos, sd_inactive_log,
                      "sd_inactive_plusZphys", air_log_plus_, false, 0,
                      check_all_for_overlaps_);
    new G4PVPlacement(0, pos, sd_inactive_log,
                      "sd_inactive_minsZphys", air_log_mins_, false, 0,
                      check_all_for_overlaps_);
    // Add the heads of the mounting screws that the scintillator
    // rests against
    G4Tubs *mounting_screw_head = new G4Tubs("mounting_screw_head_tub",
                                             sd_mounting_screw_head.inner_radius,
                                             sd_mounting_screw_head.outer_radius,
                                             sd_mounting_screw_head.length/2.0, 0, 2*M_PI);
    G4LogicalVolume *mounting_screw_head_log =
        new G4LogicalVolume(mounting_screw_head, ChamberMaterial,
                            "mounting_screw_head_log");
    G4VisAttributes *mounting_screw_head_vis =
        new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    mounting_screw_head_vis -> SetForceSolid(true);
    mounting_screw_head_log -> SetVisAttributes(mounting_screw_head_vis);
    // Place them!
    G4double xpos, ypos, zpos;
    xpos = sd_mounting_screw_head.center_position.x();
    ypos = sd_mounting_screw_head.center_position.y();
    zpos = reentrant_flange_pipe.center_position.z() -
        sd_mounting_screw_head.center_position.z();
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadP1",
                      air_log_plus_, false, 0, check_all_for_overlaps_);
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadM1",
                      air_log_mins_, false, 0, check_all_for_overlaps_);
    xpos *= -1.0;
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadP2",
                      air_log_plus_, false, 0, check_all_for_overlaps_);
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadM2",
                      air_log_mins_, false, 0, check_all_for_overlaps_);
    ypos *= -1.0;
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadP3",
                      air_log_plus_, false, 0, check_all_for_overlaps_);
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadM3",
                      air_log_mins_, false, 0, check_all_for_overlaps_);

    xpos *= -1.0;
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadP4",
                      air_log_plus_, false, 0, check_all_for_overlaps_);
    new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos),
                      mounting_screw_head_log, "screwHeadM4",
                      air_log_mins_, false, 0, check_all_for_overlaps_);
  }

  
}  // End construct stip detectors

void K37DetectorConstruction::ConstructAir() {
  G4NistManager* nist_manager = G4NistManager::Instance();
  // Air is needed to fill the space between the strip detector and
  // scintillator.  I will be constructed first to fill the entire
  G4Tubs *reentrant_pipe = new G4Tubs("air_in_pipe",
                                      0.0,
                                      reentrant_flange_pipe.inner_radius,
                                      reentrant_flange_pipe.length/2.0,
                                      0.0, 2*M_PI*rad);
  G4Cons *reentrant_des = new G4Cons("air_in_descender",
                                     0.0,
                                     reentrant_flange_descender.inner_radius2,
                                     0.0,
                                     reentrant_flange_descender.inner_radius,
                                     reentrant_flange_descender.length/2.0,
                                     0.0, 2.0*M_PI*rad);
  G4UnionSolid *reentrant_air =
      new G4UnionSolid("air_in_reentrant", reentrant_pipe, reentrant_des, 0,
                       G4ThreeVector(0.0, 0.0,
                                     0.5*(reentrant_flange_pipe.length +
                                          reentrant_flange_descender.length)));
  air_log_plus_ =
      new G4LogicalVolume(reentrant_air,
                          nist_manager -> FindOrBuildMaterial("G4_AIR"),
                          "air_in_reentrant_plus", 0, 0, 0);
  air_log_mins_ =
      new G4LogicalVolume(reentrant_air,
                          nist_manager -> FindOrBuildMaterial("G4_AIR"),
                          "air_in_reentrant_mins", 0, 0, 0);

  G4VisAttributes *air_vis =// new G4VisAttributes(false); // invisible
      new G4VisAttributes(G4Colour(0.53, 0.80, 0.98, 0.1)); // sky blue
  air_vis -> SetForceSolid(true);
  air_log_plus_ -> SetVisAttributes(air_vis);
  air_log_mins_ -> SetVisAttributes(air_vis);
  G4ThreeVector pos = reentrant_flange_pipe.center_position;// -
  //      G4ThreeVector(0.0, 0.0, 0.5*reentrant_flange_descender.length);
  CLHEP::HepRotation *zRot = new CLHEP::HepRotation;
  zRot -> rotateY(180.0*deg);

  new G4PVPlacement(zRot, pos, air_log_plus_, "air_plus_z", world_log_,
                    false, 0, check_all_for_overlaps_);

  new G4PVPlacement(0, -1.0*pos, air_log_mins_, "air_mins_z", world_log_,
                    false, 0, check_all_for_overlaps_);
}

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
  G4RotationMatrix *y180 = new G4RotationMatrix(G4ThreeVector(0.0, 1.0, 0.0),
  180.*deg);
  chamber_phys_ = new G4PVPlacement(y180, G4ThreeVector(0.0, 0.0, 0.0),
                                    chamber_log_, "chamber_phys", world_log_,
                                    false, 0, check_all_for_overlaps_);

  // chamber_log_VisAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  // chamber_logVisAttributes-> SetForceWireframe(true);

  // G4VisAttributes(false) means invisible. It is better than using the
  // convience function G4VisAttributes::Invisble becuse it produces
  // a non const pointer that can later be delete avoiding a memory
  // leak.
  //chamber_logVisAttributes = new G4VisAttributes(false);
  chamber_logVisAttributes = new G4VisAttributes(false);
      //    G4Colour(255/255., 170/255., 0, 0.1));

  chamber_logVisAttributes-> SetForceWireframe(true);
  chamber_log_ -> SetVisAttributes(chamber_logVisAttributes);

  // ------------------------------ Reentrant Flanges for beta-telescope
  // (z+ and z-) (OPRF)


  G4VSolid * OPRF_sol = new G4Tubs("OPRF_sol",
                                   reentrant_flange_pipe.inner_radius,
                                   reentrant_flange_pipe.outer_radius,
                                   reentrant_flange_pipe.length/2.0,
                                   0.0*deg, 360.0*deg);

  G4LogicalVolume * OPRF_log = new G4LogicalVolume(OPRF_sol,
                                                   ChamberMaterial,
                                                   "OPRF_log", 0, 0, 0);

  new G4PVPlacement(0, reentrant_flange_pipe.center_position, OPRF_log,
                    "OPRF_plusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
  new G4PVPlacement(0, -1.0*reentrant_flange_pipe.center_position, OPRF_log,
                    "OPRF_minusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
  OPRF_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  OPRF_logVisAttributes-> SetForceSolid(true);
  //  chamber_logVisAttributes-> SetForceWireframe(true);
  OPRF_log -> SetVisAttributes(OPRF_logVisAttributes);

  // ------------------------------ Reentrant Flange Descender (RFD)

  G4VSolid * RFD_sol = new G4Cons("RFD_sol",
                                  reentrant_flange_descender.inner_radius,
                                  reentrant_flange_descender.outer_radius,
                                  reentrant_flange_descender.inner_radius2,
                                  reentrant_flange_descender.outer_radius2,
                                  reentrant_flange_descender.length/2.0,
                                  0.0*deg, 360.0*deg);
  G4LogicalVolume* RFD_log = new G4LogicalVolume(RFD_sol, ChamberMaterial,
                                                 "RFD_log", 0, 0, 0);
  RFD_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  RFD_logVisAttributes-> SetForceSolid(true);
  RFD_log -> SetVisAttributes(RFD_logVisAttributes);

  new G4PVPlacement(0, reentrant_flange_descender.center_position,
                    RFD_log, "RFD_plusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);


  // G4RotationMatrix* RFDRotation = new G4RotationMatrix();
  RFDRotation = new CLHEP::HepRotation();
  RFDRotation->rotateX(180.*deg);

  new G4PVPlacement(RFDRotation,
                    -1.0*reentrant_flange_descender.center_position,
                    RFD_log, "RFD_minusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);

  // ------------------------------ beryllium declared here so that a
  // socket could be cut out for it
  G4VSolid * beryllium_sol = new G4Tubs("beryllium_sol",
                                        beryllium_window.inner_radius,
                                        beryllium_window.outer_radius,
                                        beryllium_window.length/2.0,
                                        0.0*rad, 2*M_PI*rad);

  G4LogicalVolume* beryllium_log = new G4LogicalVolume(beryllium_sol,
                                                       FoilMaterial,
                                                       "beryllium_log",
                                                       0, 0, 0);

  beryllium_logVisAttributes =
      new G4VisAttributes(G4Colour(0.80, 0.36, 0.36, 0.8)); // pale red
  // G4VisAttributes* beryllium_logVisAttributes =
  //   new G4VisAttributes(G4Colour(0.1, 0.2, 0.7));

  beryllium_logVisAttributes-> SetForceSolid(true);

  beryllium_log -> SetVisAttributes(beryllium_logVisAttributes);

  new G4PVPlacement(0, beryllium_window.center_position,
                    beryllium_log, "beryllium_plusZ_phys", world_log_, false,
                    0, check_all_for_overlaps_);
  new G4PVPlacement(0, -1.0*beryllium_window.center_position,
                    beryllium_log, "beryllium_minusZ_phys",
                    world_log_, false, 0, check_all_for_overlaps_);

  // ------------------------------ Front Face Reentrant Flange (FFRF)

  G4VSolid * FFRF_beforeCut_sol =
      new G4Cons("FFRF_beforeCut_sol",
                 reentrant_flange_front_face.inner_radius,
                 reentrant_flange_front_face.outer_radius,
                 reentrant_flange_front_face.inner_radius2,
                 reentrant_flange_front_face.outer_radius2,
                 reentrant_flange_front_face.length/2.0,
                 0.0*deg, 360.0*deg);

  G4ThreeVector cutForBeryllium(0.0, 0.0,
        0.5*(reentrant_flange_front_face.length - beryllium_window.length));

  G4SubtractionSolid* FFRF_sol =
    new G4SubtractionSolid("FFRF_sol", FFRF_beforeCut_sol, beryllium_sol, 0,
                           cutForBeryllium);
  
  G4LogicalVolume* FFRF_log = new G4LogicalVolume(FFRF_sol,
                                                  ChamberMaterial,
                                                  "FFRF_log", 0, 0, 0);

  FFRF_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  FFRF_logVisAttributes-> SetForceSolid(true);
  FFRF_log -> SetVisAttributes(FFRF_logVisAttributes);

  new G4PVPlacement(0,
                    reentrant_flange_front_face.center_position,
                    FFRF_log, "FFRF_plusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);

  // G4RotationMatrix* FFRFRotation = new G4RotationMatrix();
  FFRFRotation = new CLHEP::HepRotation();
  FFRFRotation->rotateX(180.*deg);

  new G4PVPlacement(FFRFRotation,
                    -1.0*reentrant_flange_front_face.center_position,
                    FFRF_log, "FFRF_minusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
}  // End construct chamber

void K37DetectorConstruction::ConstructMirrors() {
  // SiC mirrors to reflect OP light

  CLHEP::HepRotation *mirror_rotation = new CLHEP::HepRotation();
  mirror_rotation -> rotateX(mirror.rotation_angle);

  G4VSolid * mirror_sol = new G4Tubs("mirror", mirror.inner_radius,
                                     mirror.outer_radius, mirror.length/2.0,
                                     0.0*deg, 360.0*deg);

  mirror_log = new G4LogicalVolume(mirror_sol, MirrorMaterial,
                                   "mirror_log", 0, 0, 0);
  new G4PVPlacement(mirror_rotation, mirror.center_position,
                    mirror_log, "mirror_plusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
  new G4PVPlacement(mirror_rotation, -1.0*mirror.center_position,
                    mirror_log, "mirror_minusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
  mirror_logVisAttributes = new G4VisAttributes(G4Colour(0.9, 0.1, 0.1, 0.9));
  mirror_logVisAttributes-> SetForceSolid(true);
  mirror_log -> SetVisAttributes(mirror_logVisAttributes);
  // ------------------------------ Mirror Mount (MM)
  G4VSolid *mirror_mount_tub =
      new G4Tubs("mirror_mount_tub", mirror_mount.inner_radius,
                 mirror_mount.outer_radius, mirror_mount.length/2.0, 0.0, 2.*M_PI);

  // make the box slightly bigger in the z-direction for safety
  G4Box *collimator_box = new G4Box("collimator_box",
                                    mirror_mount.cutout_side_length/2.0,
                                    mirror_mount.cutout_side_length/2.0,
                                    1.05*mirror_mount.length/2.0);

  G4Tubs *mount_tubs = new G4Tubs("collimator_tubs",
                                  0.0, mirror_mount.cutout_radius,
                                  mirror_mount.cutout_depth/2.0, 0.0, 2*M_PI);

  G4SubtractionSolid* mirror_mount_sub =
      new G4SubtractionSolid("MM_sol", mirror_mount_tub, collimator_box);

  G4SubtractionSolid* mirror_mount_final =
      new G4SubtractionSolid("MM_final", mirror_mount_sub, mount_tubs,
                             mirror_rotation, G4ThreeVector(0, 0, 0));

  G4LogicalVolume *mirror_mount_log =
      new G4LogicalVolume(mirror_mount_final, MirrorMountMaterial,
                          "MM_log", 0, 0, 0);

  MM_logVisAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  G4VisAttributes* MM_logVisAttributes =
      new G4VisAttributes(G4Colour(0.2, 0.9, 0.5));

  //MM_logVisAttributes-> SetForceSolid(true);
  MM_logVisAttributes-> SetForceWireframe(true);
  mirror_mount_log -> SetVisAttributes(MM_logVisAttributes);

  new G4PVPlacement(0, mirror_mount.center_position,
                    mirror_mount_log, "MM_plusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);

  CLHEP::HepRotation *MMRotation = new CLHEP::HepRotation();
  MMRotation->rotateX(180.*deg);
  new G4PVPlacement(MMRotation, -1.0*mirror_mount.center_position,
                    mirror_mount_log, "MM_minusZ_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
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
                    world_log_, false, 0, check_all_for_overlaps_);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 29.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_4",
                    world_log_, false, 0, check_all_for_overlaps_);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -29.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_5",
                    world_log_, false, 0, check_all_for_overlaps_);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -57.5*mm, 0.),
                    hoop_3through6_box_log, "hoop_3through6_phys_6",
                    world_log_, false, 0, check_all_for_overlaps_);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., -75.5*mm, 0.),
                    hoop_7_log, "hoop_7_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 75.5*mm, 0.),
                    hoop_2_log, "hoop_2_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
  new G4PVPlacement(hoopRotation, G4ThreeVector(0., 97.5*mm, 0.),
                    hoop_1_log, "hoop_1_phys", world_log_, false, 0,
                    check_all_for_overlaps_);
}  // End construct hoops

void K37DetectorConstruction::ConstructElectronMCP(G4SDManager *sd_man) {
  //  G4double SOED_rmax = 40./2.*mm;
  G4double SOED_rmin = 0     *mm;
  G4double SOED_dz   = 10./2.*mm;
  G4double SOED_Sphi = 0.    *deg;
  G4double SOED_Dphi = 360.  *deg;
  G4double SOED_z_pos = -82.0 *mm;     // 82 mm
  electron_mcp_tub_ = new G4Tubs("SOED_sol", SOED_rmin, electron_mcp_radius_,
                                 SOED_dz, SOED_Sphi, SOED_Dphi);
  electron_mcp_log_ = new G4LogicalVolume(electron_mcp_tub_, MCPMaterial,
                                          "SOED_log", 0, 0, 0, check_all_for_overlaps_);
  electron_mcp_phys_ = new G4PVPlacement(changeZtoX,
                                         G4ThreeVector(0., SOED_z_pos, 0),
                                         electron_mcp_log_, "SOED_phys",
                                         world_log_, false, 0, check_all_for_overlaps_);
  SOED_logVisAttributes = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));
  SOED_logVisAttributes-> SetForceSolid(true);
  electron_mcp_log_ -> SetVisAttributes(SOED_logVisAttributes);


  // Set up sensitive detector
  if (!electron_mcp_sens_) {
    electron_mcp_sens_ = new K37ElectronMCPSD("/mydet/electron_mcp");
    sd_man -> AddNewDetector(electron_mcp_sens_);
  }
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
                                       false, 0, check_all_for_overlaps_);
  rmcp_logVisAttributes_ = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));
  rmcp_logVisAttributes_ -> SetForceSolid(true);
  recoil_mcp_log_ -> SetVisAttributes(rmcp_logVisAttributes_);

  // Set up sensitive detector
  if (!recoil_mcp_sens_) {
    recoil_mcp_sens_ = new K37RecoilMCPSD("/mydet/recoil_mcp");
    sd_man -> AddNewDetector(recoil_mcp_sens_);
  }

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
                    "coils_plusZ_phys", world_log_, false, 0, check_all_for_overlaps_);
  new G4PVPlacement(0, G4ThreeVector(0., 0., -65.), coils_log,
                    "coils_minusZ_phys", world_log_, false, 0, check_all_for_overlaps_);

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
  G4Material *teflon = nist_manager -> FindOrBuildMaterial("G4_TEFLON");
  //  G4Material *air = nist_manager -> FindOrBuildMaterial("G4_AIR")
  trinat_materials_.insert(std::pair<G4String, G4Material*>(teflon -> GetName(),
                                                            teflon));
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




