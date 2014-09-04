// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37DetectorConstruction_h
#define K37DetectorConstruction_h 1

#include <CLHEP/Vector/Rotation.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4Box.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

#include "K37ElectronMCPSD.hh"
#include "K37RecoilMCPSD.hh"
#include "K37ScintillatorSD.hh"
#include "K37StripDetectorSD.hh"

class G4VPhysicalVolume;
class G4VisAttributes;
class G4Material;
class G4Element;
class G4Trd;
class G4LogicalVolume;
class K37DetectorMessenger;
class K37ElectricFieldSetup;
class HepRotation;

struct GeometryElement {
  G4String nameBase;
  G4double inner_radius;
  G4double outer_radius;
  G4double inner_radius2;
  G4double outer_radius2;
  G4double sweep_radius;

  G4double start_phi;
  G4double delta_phi;
  G4double rotation_angle;
  G4ThreeVector center_position;

  // Three cartesian dimensions
  G4double length;
  G4double width;
  G4double depth;

  G4double cutout_side_length;
  G4double cutout_radius;
  G4double cutout_depth;
};

class K37DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  K37DetectorConstruction();
  ~K37DetectorConstruction();

  G4VPhysicalVolume* Construct();
  const G4VPhysicalVolume* GetPhysicalWorld() {return world_phys_;}
  G4Material*  GetMirrorMaterial() {return MirrorMaterial;}
  G4double GetDistanceToTrap() {return 10.0;}
  G4double GetDeDxRadius() {return 10.0;}
  G4double GetSubtraction() {return 10.0;}
  void SetWFEDM_MirrorCut(G4bool value) {shouldTheMirrorBeWFEDMCut = value;}

  void UpdateGeometry();
  void SetMirrorMaterial(G4String);
  void PrintMaterialList();

  void SetMakeChamber(G4bool f) {
     if((makeSimpleChamber == true) && (f == true) )
     {
        G4cerr<<"Cannot make chamber and simple chamber simultanously."<<G4endl;
        G4cerr<<"Turn off simple chamber and try again."<<G4endl;
     }
     else
     {
        makeChamber = f;
     }
  }
  void SetMakeSimpleChamber(G4bool f) {
     if(( makeChamber == true) && (f == true) )
     {
        G4cerr<<"Cannot make chamber and simple chamber simultanously."<<G4endl;
        G4cerr<<"Turn off chamber and try again."<<G4endl;
     }
     else
     {
        makeSimpleChamber = f;
     }
  }

  void SetMakeMirrors(G4bool f) {makeMirrors = f;}
  void SetMakeHoops(G4bool f) {makeHoops = f;}
  void SetMakeElectronMCP(G4bool f) {makeElectronMCP = f;}
  void SetMakeCoils(G4bool f) {makeCoils = f;}
  void SetMakeRecoilMCP(G4bool f) {make_r_mcp_ = f;}
  void SetMakeSDholders(G4bool f) {make_sd_holders_ = f;}

  G4bool GetMakeElectronMCP() {return makeElectronMCP;}
  G4bool GetMakeRecoilMCP() {return make_r_mcp_;}

  void SetElectronMCPradius(G4double d) {electron_mcp_radius_= d;}
  G4double GetElectronMCPradius() {return electron_mcp_radius_;}

  void SetTeflonTapeThickness(G4double t) {teflon_front_face.length = t;}
  void SetMountingScrewHeadLength(G4double l) {sd_mounting_screw_head.length = l;}

 private:
  G4VPhysicalVolume* ConstructK37Experiment();
  void CalculateDimensions();
  void ConstructScintillators(G4SDManager* SDman);
  void ConstructStripDetectors(G4SDManager* SDman);
  void ConstructChamber();      // Not a sensitive detector
  void ConstructSimpleChamber();// Not a sensitive detector
  void ConstructMirrors();      // Not a sensitive detector
  void ConstructHoops();        // Not a sensitive detector
  void ConstructElectronMCP(G4SDManager *sd_man);
  void ConstructRecoilMCP(G4SDManager *sd_man);
  void ConstructCoils();        // Not a sensitive detector
  void ConstructAir();          // Not a sensitive detector
void ConstructBerylliumFoils(G4SDManager* SDman);
  void DefineMaterials();

  G4Material* world_material_;          // default is vacuum
  G4double world_size_;                 // default is 2.0 x 2.0 x 2.0 m

  G4Box *world_box_;                  // pointer to the solid world
  G4LogicalVolume *world_log_;        // pointer to the logical world
  G4VPhysicalVolume *world_phys_;      // pointer to the physical World

  G4Tubs *scintillator_tubs_;        // pointer to the solid scintillator

  G4LogicalVolume *upper_scintillator_log_;     // pointer to the logical scinti
  G4VPhysicalVolume *upper_scintillator_phys_;  // pointer to the physical scint
  K37ScintillatorSD *upper_scintillator_sens_;  // pointer to the sensitive scin

  G4LogicalVolume *lower_scintillator_log_;     // pointer to the logical scinti
  G4VPhysicalVolume *lower_scintillator_phys_;  // pointer to the physical scint
  K37ScintillatorSD *lower_scintillator_sens_;  // pointer to the sensitive scin

  G4Box *strip_detector_box_;              // pointer to the solid DSSSD

  G4LogicalVolume *upper_strip_detector_log_;     // ptr to the logical DSSSD
  G4VPhysicalVolume *upper_strip_detector_phys_;  // ptr to the physical DSSSD
  K37StripDetectorSD *upper_strip_detector_sens_;  // ptr to the sensitive DSSSD

  G4LogicalVolume *lower_strip_detector_log_;      // ptr to the logical DSSSD
  G4VPhysicalVolume *lower_strip_detector_phys_;   // ptr to the physical DSSSD
  K37StripDetectorSD *lower_strip_detector_sens_;  // ptr to the sensitive DSSSD

//   G4SubtractionSolid *upper_sd_holder_sol_;  // ptr to solid DSSSD holder
//   G4LogicalVolume *upper_sd_holder_log_;     // ptr to logical DSSSD holder
//   G4VPhysicalVolume *upper_sd_holder_phys_;  // ptr to physical DSSSD holder

//   G4SubtractionSolid *lower_sd_holder_sol_;  // ptr to solid DSSSD holder
//   G4LogicalVolume *lower_sd_holder_log_;     // ptr to logical DSSSD holder
//   G4VPhysicalVolume *lower_sd_holder_phys_;  // ptr to physical DSSSD holder

  G4SubtractionSolid *chamber_box_;        // pointer to the solid chamber
  G4LogicalVolume *chamber_log_;           // pointer to the logical chamber
  G4VPhysicalVolume *chamber_phys_;        // pointer to the physical chamber

  G4Tubs *electron_mcp_tub_;                // pointer to the solid eMCP
  G4LogicalVolume *electron_mcp_log_;      // pointer to the logical eMCP
  G4VPhysicalVolume *electron_mcp_phys_;   // pointer to the physical eMCP
  K37ElectronMCPSD *electron_mcp_sens_;    // pointer to the sensitive eMCP

  G4Tubs *recoil_mcp_tub_;                 // pointer to the solid rMCP
  G4LogicalVolume *recoil_mcp_log_;        // pointer to the logical rMCP
  G4VPhysicalVolume *recoil_mcp_phys_;      // pointer to the physcial rMCP
  K37RecoilMCPSD *recoil_mcp_sens_;         // pointer to the sensitive rMCP

  G4MultiFunctionalDetector *be_scorer_;   // pointer to the sensitive Be
  
  K37DetectorMessenger* detectorMessenger;  // pointer to the Messenger
  G4LogicalVolume * mirror_log;
  G4Material* MirrorMaterial;

  G4LogicalVolume *air_log_plus_;
  G4LogicalVolume *air_log_mins_;

  G4Material* FullEnergyDetectorMaterial;
  G4Material* DeDxDetectorMaterial;
  G4Material* SiliconDetectorFrameMaterial;
  G4Material* ChamberMaterial;
  G4Material* FoilMaterial;
  G4Material* HoopMaterial;
  G4Material* MirrorMountMaterial;
  G4Material* CoilsMetal;
  G4Material* CoilsLiquid;
  G4Material* Hoop7Material;

  G4Material*   MCPMaterial;
  G4double electron_mcp_radius_;

  //Numbers for the coil construction see /Drawings/cros-coil.png
  G4double distanceFromCloseEdgeToPolarization;
  G4double distanceFromBottomToChamberCenter;
  G4double tubeOuterDiameter;
  G4double tubeInnerDiameter;
  G4double tubeSpacing;

  G4bool shouldTheMirrorBeWFEDMCut;     // should the mirror be Wire Fed EDM cut
  G4bool check_all_for_overlaps_;

  // = true or straight cut = false
  CLHEP::HepRotation* changeZtoX;
  CLHEP::HepRotation* changeZto45;
  CLHEP::HepRotation* changeZtoNeg45;
  CLHEP::HepRotation* changeZto35;
  CLHEP::HepRotation* changeZtoNeg35;
  CLHEP::HepRotation* changeZtoNeg62;
  CLHEP::HepRotation* changeZtoNeg118;
  CLHEP::HepRotation* changeYtoBeamAxis;
  CLHEP::HepRotation* changeYtoBeamAxisForLPP1;
  CLHEP::HepRotation* changeYtoBeamAxisForLPP2;
  CLHEP::HepRotation* changeYtoBeamAxisForLPP3;
  CLHEP::HepRotation* changeYtoBeamAxisForLPP4;
  CLHEP::HepRotation* rotationForOpticalPumpingBeams1;
  CLHEP::HepRotation* rotationForOpticalPumpingBeams2;
  CLHEP::HepRotation* RFDRotation;
  CLHEP::HepRotation* FFRFRotation;
  CLHEP::HepRotation* MirrorCutRotation;
  CLHEP::HepRotation* hoopRotation;
  CLHEP::HepRotation* zRot;
  CLHEP::HepRotation* y180;
  CLHEP::HepRotation* mirror_rotation;
  CLHEP::HepRotation* MMRotation;

  G4VisAttributes * dedxFrame_logVisAttributes;
  G4VisAttributes * chamber_logVisAttributes;
  G4VisAttributes * OPRF_logVisAttributes;
  G4VisAttributes * RFD_logVisAttributes;
  G4VisAttributes * beryllium_logVisAttributes;
  G4VisAttributes * FFRF_logVisAttributes;
  G4VisAttributes * mirror_logVisAttributes;
  G4VisAttributes * MM_logVisAttributes;
  G4VisAttributes * cut_ESP_logVisAttributes;
  G4VisAttributes * hoop7_logVisAttributes;
  G4VisAttributes * SOED_logVisAttributes;
  G4VisAttributes * coils_logVisAttributes;
  G4VisAttributes * coilLiquid_logVisAttributes;
  G4VisAttributes * world_logVisAttributes;
  G4VisAttributes * air_vis;
  G4VisAttributes * scintillator_vis;
  G4VisAttributes * teflon_vis;
  G4VisAttributes * strip_detector_vis;
  G4VisAttributes * sd_inactive_vis;
  G4VisAttributes * mounting_screw_head_vis;

  G4VisAttributes * rmcp_logVisAttributes_;
  // Bools to turn off or on various aspects of the apparatus to simulate their
  // individual effects
  G4bool makeScintillators, makeStripDetectors, makeChamber, makeMirrors;
  G4bool makeHoops, makeElectronMCP, makeCoils, make_r_mcp_, make_sd_holders_;
  G4bool make_beryllium_foils_, makeSimpleChamber;

  GeometryElement simpleChamberTub;
  GeometryElement simpleChamberRentrantFlangeCut;
  GeometryElement mirror;
  GeometryElement mirror_mount;
  GeometryElement sd_frame;
  GeometryElement sd_inactive;
  GeometryElement sd_mounting_screw_head;
  GeometryElement strip_detector;
  GeometryElement teflon_front_face;
  GeometryElement scintillator;
  GeometryElement beryllium_window;
  GeometryElement reentrant_flange_front_face;
  GeometryElement reentrant_flange_descender;
  GeometryElement reentrant_flange_pipe;
  GeometryElement coil_inner_torus_PZ[4][4];
  GeometryElement coil_outer_tubs_PZ[4][4];
  GeometryElement coil_inner_torus_MZ[4][4];
  GeometryElement coil_outer_tubs_MZ[4][4];

  std::map<G4String, G4Material*> trinat_materials_;

};

#endif
