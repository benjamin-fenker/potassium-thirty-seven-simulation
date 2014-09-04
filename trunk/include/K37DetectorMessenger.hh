// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37DetectorMessenger_h
#define K37DetectorMessenger_h 1

#include "globals.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UImessenger.hh"

class K37DetectorConstruction;

class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIdirectory;
//-----------------------------

class K37DetectorMessenger: public G4UImessenger {
 public:
  explicit  K37DetectorMessenger(K37DetectorConstruction* dc);
  ~K37DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  K37DetectorConstruction* myDetector;

  G4UIdirectory*        geometryDir;
  G4UIcmdWithAString*   ChangeMirrorMaterial;
  G4UIcmdWithABool*   ShouldTheMirrorBeWFEDM;
  G4UIcmdWithoutParameter* UpdateGeometryCommand;
  G4UIcmdWithoutParameter* PrintAvailableMaterials;
  G4UIcmdWithABool *make_chamber_cmd_;
  G4UIcmdWithABool *make_simple_chamber_cmd_;
  G4UIcmdWithABool *make_mirrors_cmd_;
  G4UIcmdWithABool *make_hoops_cmd_;
  G4UIcmdWithABool *make_electron_mcp_cmd_;
  G4UIcmdWithABool *make_coils_cmd_;
  G4UIcmdWithABool *make_recoil_mcp_cmd_;
  G4UIcmdWithABool *make_sd_holders_cmd_;
  G4UIcmdWithADoubleAndUnit *set_electron_mcp_radius_cmd_;
  G4UIcmdWithoutParameter* get_electron_mcp_radius_cmd_;
  G4UIcmdWithADoubleAndUnit *set_teflon_thickness_cmd_;
  G4UIcmdWithADoubleAndUnit *set_screw_head_length_cmd_;
};

//-----------------------------

#endif

