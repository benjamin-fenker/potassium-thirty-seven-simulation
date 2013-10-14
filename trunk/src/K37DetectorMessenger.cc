// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37DetectorMessenger.hh"
#include "K37DetectorConstruction.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "globals.hh"

K37DetectorMessenger::K37DetectorMessenger(K37DetectorConstruction* myDet)
    :myDetector(myDet), geometryDir(0), ChangeMirrorMaterial(0),
     ShouldTheMirrorBeWFEDM(0), UpdateGeometryCommand(0),
     PrintAvailableMaterials(0) {
    geometryDir = new G4UIdirectory("/K37/geometry/");
    geometryDir -> SetGuidance("geometry commands for K37");
    ChangeMirrorMaterial =
      new G4UIcmdWithAString("/K37/geometry/mirrorMaterial", this);
    ChangeMirrorMaterial -> SetGuidance("Change the material of the mirror");
    ChangeMirrorMaterial ->
      SetGuidance("Choices listed by /K37/geometry/printMaterialList");
    ChangeMirrorMaterial -> SetParameterName("material", true);
    ChangeMirrorMaterial -> SetDefaultValue("SiliconCarbide");
    // ChangeMirrorMaterial->SetCandidates("GlassyCarbon SiliconCarbide");
    // ChangeMirrorMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
    ChangeMirrorMaterial -> AvailableForStates(G4State_Idle);

    ShouldTheMirrorBeWFEDM =
      new G4UIcmdWithABool("/K37/geometry/ShouldTheMirrorBeWFEDM", this);
    ShouldTheMirrorBeWFEDM ->
      SetGuidance("If set to true the mirror mount will be cut like");
    ShouldTheMirrorBeWFEDM ->
      SetGuidance("a Wire Feed EDM. If set to false the mirror mount");
    ShouldTheMirrorBeWFEDM ->
      SetGuidance("will be cut straight like on a milling machine.");
    ShouldTheMirrorBeWFEDM -> SetParameterName("ShouldTheMirrorBeWFEDM", true);
    ShouldTheMirrorBeWFEDM -> SetDefaultValue(true);
    ShouldTheMirrorBeWFEDM -> AvailableForStates(G4State_Idle);

    UpdateGeometryCommand =
      new G4UIcmdWithoutParameter("/K37/geometry/update", this);
    UpdateGeometryCommand -> SetGuidance("Update geometry.");
    UpdateGeometryCommand ->
      SetGuidance("This command MUST be applied before \"beamOn\" ");
    UpdateGeometryCommand ->
      SetGuidance("if you changed geometrical value(s).");
    UpdateGeometryCommand ->
      SetGuidance("It is invoked automatically after Changing materials.");
    UpdateGeometryCommand -> AvailableForStates(G4State_Idle);

    PrintAvailableMaterials =
      new G4UIcmdWithoutParameter("/K37/geometry/printMaterialList", this);
    PrintAvailableMaterials ->
      SetGuidance("prints a list of all defined materials");
    PrintAvailableMaterials->AvailableForStates(G4State_Idle);

    make_chamber_cmd_ =
        new G4UIcmdWithABool("/K37/geometry/setMakeChamber", this);
    make_chamber_cmd_ -> SetGuidance("Include chamber in simulation?");
    make_chamber_cmd_ -> SetParameterName("make_chamber", true);
    make_chamber_cmd_ -> SetDefaultValue(true);
    make_chamber_cmd_ -> AvailableForStates(G4State_Idle);

    make_mirrors_cmd_ =
        new G4UIcmdWithABool("/K37/geometry/setMakeMirrors", this);
    make_mirrors_cmd_ -> SetGuidance("Include mirrors in simulation?");
    make_mirrors_cmd_ -> SetParameterName("make_mirrors", true);
    make_mirrors_cmd_ -> SetDefaultValue(true);
    make_mirrors_cmd_ -> AvailableForStates(G4State_Idle);

    make_hoops_cmd_ =
        new G4UIcmdWithABool("/K37/geometry/setMakeHoops", this);
    make_hoops_cmd_ -> SetGuidance("Include hoops in simulation?");
    make_hoops_cmd_ -> SetParameterName("make_hoops", true);
    make_hoops_cmd_ -> SetDefaultValue(true);
    make_hoops_cmd_ -> AvailableForStates(G4State_Idle);

    make_electron_mcp_cmd_ =
        new G4UIcmdWithABool("/K37/geometry/setMakeElectronMCP", this);
    make_electron_mcp_cmd_ ->
        SetGuidance("Include electron_mcp in simulation?");
    make_electron_mcp_cmd_ -> SetParameterName("make_electron_mcp", true);
    make_electron_mcp_cmd_ -> SetDefaultValue(true);
    make_electron_mcp_cmd_ -> AvailableForStates(G4State_Idle);

    make_coils_cmd_ =
        new G4UIcmdWithABool("/K37/geometry/setMakeCoils", this);
    make_coils_cmd_ -> SetGuidance("Include coils in simulation?");
    make_coils_cmd_ -> SetParameterName("make_coils", true);
    make_coils_cmd_ -> SetDefaultValue(true);
    make_coils_cmd_ -> AvailableForStates(G4State_Idle);

    make_recoil_mcp_cmd_ =
        new G4UIcmdWithABool("/K37/geometry/setMakeRecoilMCP", this);
    make_recoil_mcp_cmd_ -> SetGuidance("Include recoil_mcp in simulation?");
    make_recoil_mcp_cmd_ -> SetParameterName("make_recoil_mcp", true);
    make_recoil_mcp_cmd_ -> SetDefaultValue(true);
    make_recoil_mcp_cmd_ -> AvailableForStates(G4State_Idle);
}

K37DetectorMessenger::~K37DetectorMessenger() {
    delete geometryDir;
    delete ChangeMirrorMaterial;
    delete ShouldTheMirrorBeWFEDM;
    delete UpdateGeometryCommand;
    delete PrintAvailableMaterials;
    delete make_chamber_cmd_;
    delete make_mirrors_cmd_;
    delete make_hoops_cmd_;
    delete make_electron_mcp_cmd_;
    delete make_coils_cmd_;
    delete make_recoil_mcp_cmd_;
}

void K37DetectorMessenger::SetNewValue(G4UIcommand* command,
                                       G4String newValue) {
    if (command == ChangeMirrorMaterial) {
        myDetector-> SetMirrorMaterial(newValue);
    }
    if (command == ShouldTheMirrorBeWFEDM) {
        myDetector ->
          SetWFEDM_MirrorCut(ShouldTheMirrorBeWFEDM ->
                             GetNewBoolValue(newValue));
    }
    if (command == UpdateGeometryCommand) {
        myDetector->UpdateGeometry();
    }
    if (command == PrintAvailableMaterials) {
        myDetector-> PrintMaterialList();
    }

    if (command == make_chamber_cmd_) {
      myDetector->SetMakeChamber(make_chamber_cmd_->GetNewBoolValue(newValue));
    }
    if (command == make_hoops_cmd_) {
      myDetector -> SetMakeHoops(make_hoops_cmd_->GetNewBoolValue(newValue));
    }
}

