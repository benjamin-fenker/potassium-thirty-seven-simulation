#include "G4UIcmdWithABool.hh"
#include "K37RunMessenger.hh"
#include "K37RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"
#include "globals.hh"

//---------------------------------

K37RunMessenger::K37RunMessenger(K37RunAction* RA)
    :runAction (RA),RunDirectory(0),SaveFilesCommand(0),printEnergyLossTable(0),printAnnihilation(0),printTheVolumeNames(0) {
    RunDirectory = new G4UIdirectory("/K37/RunControls/");
    RunDirectory->SetGuidance("K37 Run Controls ");

    SaveFilesCommand = new G4UIcmdWithAnInteger("/K37/RunControls/saveFiles",this);
    SaveFilesCommand->SetParameterName("saveFiles",true);
    SaveFilesCommand->SetRange("saveFiles>=0 && saveFiles<=3");
    SaveFilesCommand->SetGuidance("Tells K37 what files should be made");
    SaveFilesCommand->SetGuidance("saveFiles = 0 nothing saved");
    SaveFilesCommand->SetGuidance("saveFiles = 1 runstat saved");
    SaveFilesCommand->SetGuidance("saveFiles = 2 opt.1 + Detector Energies saved");
    SaveFilesCommand->SetGuidance("saveFiles = 3 opt.2 + Hit Volume Names  saved");
    SaveFilesCommand->SetDefaultValue(0);
    SaveFilesCommand->AvailableForStates(G4State_PreInit,G4State_Idle);

    printEnergyLossTable = new G4UIcmdWithAString("/K37/RunControls/printEnergyLossTable",this);
    printEnergyLossTable->SetGuidance("Print the energy Loss Table for e+ in Materials");
    printEnergyLossTable->SetGuidance("The Material choices are are all listed by /K37/geometry/printMaterialList");
    printEnergyLossTable->SetParameterName("material",true);
    printEnergyLossTable->SetDefaultValue("SiliconCarbide");
    //printEnergyLossTable->SetCandidates("GlassyCarbon SiliconCarbide");
    //printEnergyLossTable->AvailableForStates(G4State_PreInit,G4State_Idle);
    printEnergyLossTable->AvailableForStates(G4State_Idle);

    printAnnihilation = new G4UIcmdWithABool("/K37/RunControls/printAnnihilation",this);
    printAnnihilation ->SetGuidance("Will cause the coordinates from the annihilation.");
    printAnnihilation ->SetGuidance("to be written to the file annihilation.txt");
    printAnnihilation ->SetParameterName("annihil",true);
    printAnnihilation ->SetDefaultValue(false);
    printAnnihilation ->AvailableForStates(G4State_Idle);

    printTheVolumeNames= new G4UIcmdWithABool("/K37/RunControls/printTheVolumeNames",this);
    printTheVolumeNames->SetGuidance("Will cause the list of VolumeNames to be ");
    printTheVolumeNames->SetGuidance("printed to the file volumeNames.txt");
    printTheVolumeNames->SetParameterName("pTheNames",true);
    printTheVolumeNames->SetDefaultValue(false);
    printTheVolumeNames->AvailableForStates(G4State_Idle);

}

//---------------------------------

K37RunMessenger::~K37RunMessenger() {
    delete RunDirectory;
    delete SaveFilesCommand;
    delete printEnergyLossTable;
    delete printAnnihilation;
    delete printTheVolumeNames;
}

//---------------------------------

void K37RunMessenger::SetNewValue(G4UIcommand* command,G4String newValues) {

    if (command == SaveFilesCommand) {
        //	runAction->SetRndmFreq(SaveFilesCommand->GetNewIntValue(newValues));
    }
    if (command == printEnergyLossTable) {
        runAction->PrintEnergyLossTable(newValues);
    }
    if (command == printAnnihilation) {
        runAction->SetRecordAnnihilationPosition(printAnnihilation->GetNewBoolValue(newValues));
    }
    if (command == printTheVolumeNames) {
        runAction->SetRecordVolumeNames (printTheVolumeNames->GetNewBoolValue(newValues));
    }

}

//---------------------------------
