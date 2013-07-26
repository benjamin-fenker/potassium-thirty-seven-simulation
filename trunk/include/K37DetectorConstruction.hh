// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37DetectorConstruction_h
#define K37DetectorConstruction_h 1

#include <CLHEP/Vector/Rotation.h>

#include "G4Box.hh"
#include "G4SDManager.hh"
#include "G4Tubs.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4VisAttributes;
class G4Material;
class G4Element;
class G4Trd;
class G4LogicalVolume;
class K37DetectorMessenger;
class K37ElectricFieldSetup;
class HepRotation;

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

 private:
  G4VPhysicalVolume* ConstructK37Experiment();
  void ConstructScintillators(G4SDManager* SDman);
  void ConstructStripDetectors(G4SDManager* SDman);
  void ConstructChamber();      // Not a sensitive detector
  void ConstructMirrors();      // Not a sensitive detector
  void ConstructHoops();        // Not a sensitive detector
  void ConstructElectronMCP(G4SDManager *sd_man);
  void ConstructRecoilMCP(G4SDManager *sd_man);
  void ConstructCoils();        // Not a sensitive detector
  void DefineMaterials();

  G4Material* world_material_;          // default is vacuum
  G4double world_size_;                 // default is 2.0 x 2.0 x 2.0 m

  G4Box *world_box_;                  // pointer to the solid world
  G4LogicalVolume *world_log_;        // pointer to the logical world
  G4VPhysicalVolume *world_phys_;      // pointer to the physical World

  G4Tubs *scintillator_tubs_;        // pointer to the solid scintillator

  G4LogicalVolume *upper_scintillator_log_;     // pointer to the logical scinti
  G4VPhysicalVolume *upper_scintillator_phys_;  // pointer to the physical scint

  G4LogicalVolume *lower_scintillator_log_;     // pointer to the logical scinti
  G4VPhysicalVolume *lower_scintillator_phys_;  // pointer to the physical scint

  K37DetectorMessenger* detectorMessenger;  // pointer to the Messenger
  G4LogicalVolume * mirror_log;
  G4Material* MirrorMaterial;

  G4Material* FullEnergyDetectorMaterial;
  G4Material* DeDxDetectorMaterial;
  G4Material* SiliconDetectorFrameMaterial;
  G4Material* ChamberMaterial;
  G4Material* FoilMaterial;
  G4Material* HoopMaterial;
  G4Material* MirrorMountMaterial;
  G4Material* CoilsMaterial;
  G4Material* Hoop7Material;
  G4Material*   MCPMaterial;
  G4Trd * MM_CollimatorCut_sol;
  G4bool shouldTheMirrorBeWFEDMCut;     // should the mirror be Wire Fed EDM cut
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
  CLHEP::HepRotation* MirrorRotation;
  CLHEP::HepRotation* MirrorCutRotation;
  CLHEP::HepRotation* MMRotation;
  CLHEP::HepRotation* hoopRotation;

  G4VisAttributes * world_logVisAttributes;
  G4VisAttributes * scint_logVisAttributes_plusZ;
  G4VisAttributes * scint_logVisAttributes_minusZ;
  G4VisAttributes * dedx_logVisAttributes;
  G4VisAttributes * dedx_logVisAttributes_minusZ;
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

  G4VisAttributes * rmcp_logVisAttributes_;
  // Bools to turn off or on various aspects of the apparatus to simulate their
  // individual effects
  G4bool makeScintillators, makeStripDetectors, makeChamber, makeMirrors;
  G4bool makeHoops, makeElectronMCP, makeCoils, make_r_mcp_;
};

#endif