

#include "K37DetectorMessenger.hh"
#include "K37DetectorConstruction.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "globals.hh"

K37DetectorMessenger::K37DetectorMessenger(K37DetectorConstruction* myDet)
:myDetector(myDet), geometryDir(0), ChangeMirrorMaterial(0), ShouldTheMirrorBeWFEDM(0), UpdateGeometryCommand(0), PrintAvailableMaterials(0)
{ 
  geometryDir = new G4UIdirectory("/K37/geometry/");
  geometryDir ->SetGuidance("geometry commands for K37");
        
  ChangeMirrorMaterial = new G4UIcmdWithAString("/K37/geometry/mirrorMaterial",this);  
  ChangeMirrorMaterial->SetGuidance("Change the material of the mirror");
  ChangeMirrorMaterial->SetGuidance("The choices are are all listed by /K37/geometry/printMaterialList");
  ChangeMirrorMaterial->SetParameterName("material",true);
  ChangeMirrorMaterial->SetDefaultValue("SiliconCarbide");
  //ChangeMirrorMaterial->SetCandidates("GlassyCarbon SiliconCarbide");
  //ChangeMirrorMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  ChangeMirrorMaterial->AvailableForStates(G4State_Idle);

  ShouldTheMirrorBeWFEDM = new G4UIcmdWithABool("/K37/geometry/ShouldTheMirrorBeWFEDM",this);
  ShouldTheMirrorBeWFEDM ->SetGuidance("If set to true the mirror mount will be cut like");
  ShouldTheMirrorBeWFEDM ->SetGuidance("a Wire Feed EDM. If set to false the mirror mount");
  ShouldTheMirrorBeWFEDM ->SetGuidance("will be cut straight like on a milling machine.");
  ShouldTheMirrorBeWFEDM ->SetParameterName("ShouldTheMirrorBeWFEDM",true);
  ShouldTheMirrorBeWFEDM ->SetDefaultValue(true);
  ShouldTheMirrorBeWFEDM ->AvailableForStates(G4State_Idle);

  UpdateGeometryCommand = new G4UIcmdWithoutParameter("/K37/geometry/update",this);
  UpdateGeometryCommand->SetGuidance("Update geometry.");
  UpdateGeometryCommand->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateGeometryCommand->SetGuidance("if you changed geometrical value(s).");
  UpdateGeometryCommand->SetGuidance("It is invoked automatically after Changing materials.");
  UpdateGeometryCommand->AvailableForStates(G4State_Idle);

  PrintAvailableMaterials = new G4UIcmdWithoutParameter("/K37/geometry/printMaterialList",this);
  PrintAvailableMaterials->SetGuidance("prints a list of all defined materials");
  PrintAvailableMaterials->AvailableForStates(G4State_Idle);

}

K37DetectorMessenger::~K37DetectorMessenger()
{
	delete geometryDir;
	delete ChangeMirrorMaterial;  
	delete ShouldTheMirrorBeWFEDM;  
	delete UpdateGeometryCommand;  
	delete PrintAvailableMaterials;  
}

void K37DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
      
	if( command == ChangeMirrorMaterial )
	{
	       	myDetector-> SetMirrorMaterial(newValue);
	}   
	if (command == ShouldTheMirrorBeWFEDM)
	{
		myDetector-> SetWFEDM_MirrorCut(ShouldTheMirrorBeWFEDM->GetNewBoolValue(newValue));
        }
	if( command == UpdateGeometryCommand )
	{
		myDetector->UpdateGeometry();
	}
	if( command == PrintAvailableMaterials )
	{
		myDetector-> PrintMaterialList();
	}

}

