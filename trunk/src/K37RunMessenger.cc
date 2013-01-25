// Authors: Spencer Behling and Benjamin Fenker 2013

#include "G4UIcmdWithABool.hh"
#include "K37RunMessenger.hh"
#include "K37RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"
#include "globals.hh"

extern G4bool fillEvGenData;
extern G4bool fillAllSDData;

//---------------------------------

K37RunMessenger::K37RunMessenger(K37RunAction* RA)
  : runAction(RA), RunDirectory(0), SaveFilesCommand(0),
    printEnergyLossTable(0), printAnnihilation(0), printTheVolumeNames(0) {
  RunDirectory = new G4UIdirectory("/K37/RunControls/");
  RunDirectory->SetGuidance("K37 Run Controls ");

  SaveFilesCommand = new G4UIcmdWithAnInteger("/K37/RunControls/saveFiles",
                                              this);
  SaveFilesCommand -> SetParameterName("saveFiles", true);
  SaveFilesCommand -> SetRange("saveFiles>=0 && saveFiles<=3");
  SaveFilesCommand -> SetGuidance("Tells K37 what files should be made");
  SaveFilesCommand -> SetGuidance("saveFiles= 0 nothing saved");
  SaveFilesCommand -> SetGuidance("saveFiles= 1 runstat saved");
  SaveFilesCommand ->
      SetGuidance("saveFiles= 2 opt.1 + Detector Energies saved");
  SaveFilesCommand ->
      SetGuidance("saveFiles= 3 opt.2 + Hit Volume Names  saved");
  SaveFilesCommand -> SetDefaultValue(0);
  SaveFilesCommand -> AvailableForStates(G4State_PreInit, G4State_Idle);

  printEnergyLossTable =
    new G4UIcmdWithAString("/K37/RunControls/printEnergyLossTable", this);
  printEnergyLossTable ->
    SetGuidance("Print the energy Loss Table for e+ in Materials");
  printEnergyLossTable ->
    SetGuidance("Material choices listed by /K37/geometry/printMaterialList");
  printEnergyLossTable -> SetParameterName("material", true);
  printEnergyLossTable -> SetDefaultValue("SiliconCarbide");
  // printEnergyLossTable -> SetCandidates("GlassyCarbon SiliconCarbide");
  // printEnergyLossTable -> AvailableForStates(G4State_PreInit,G4State_Idle);
  printEnergyLossTable -> AvailableForStates(G4State_Idle);

  printAnnihilation =
    new G4UIcmdWithABool("/K37/RunControls/printAnnihilation", this);
  printAnnihilation ->
    SetGuidance("Will cause the coordinates from the annihilation");
  printAnnihilation ->
      SetGuidance("to be written to the file annihilation.txt");
  printAnnihilation -> SetParameterName("annihil", true);
  printAnnihilation -> SetDefaultValue(false);
  printAnnihilation -> AvailableForStates(G4State_Idle);

  printTheVolumeNames =
    new G4UIcmdWithABool("/K37/RunControls/printTheVolumeNames", this);
  printTheVolumeNames -> SetGuidance("Will cause VolumeNames to be");
  printTheVolumeNames -> SetGuidance("printed to the file volumeNames.txt");
  printTheVolumeNames -> SetParameterName("pTheNames", true);
  printTheVolumeNames -> SetDefaultValue(false);
  printTheVolumeNames -> AvailableForStates(G4State_Idle);

  setFillEvGenData =
      new G4UIcmdWithABool("/K37/RunControls/fillEvGenData", this);
  setFillEvGenData -> SetGuidance("Will fill (or not) event generator ntuples");
  setFillEvGenData -> SetParameterName("psetFillEvGenData", true);
  setFillEvGenData -> SetDefaultValue(true);
  setFillEvGenData -> AvailableForStates(G4State_Idle);

  setFillAllSDData =
      new G4UIcmdWithABool("/K37/RunControls/fillAllSDData", this);
  setFillAllSDData -> SetGuidance("Will fill (or not) event generator ntuples");
  setFillAllSDData -> SetParameterName("psetFillAllSDData", true);
  setFillAllSDData -> SetDefaultValue(true);
  setFillAllSDData -> AvailableForStates(G4State_Idle);

  setFileName = new G4UIcmdWithAString("/K37/RunControls/setFilename", this);
  setFileName -> SetGuidance("Enter output filename without extension");
  setFileName -> SetParameterName("filename", true);
  setFileName -> SetDefaultValue("K37");
  setFileName -> AvailableForStates(G4State_Idle);
}

//---------------------------------

K37RunMessenger::~K37RunMessenger() {
  delete RunDirectory;
  delete SaveFilesCommand;
  delete printEnergyLossTable;
  delete printAnnihilation;
  delete printTheVolumeNames;
  delete setFileName;
  delete setFillEvGenData;
  delete setFillAllSDData;
}

//---------------------------------

void K37RunMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  if (command == SaveFilesCommand) {
    //      runAction->SetRndmFreq(SaveFilesCommand->GetNewIntValue(newValues));
  }
  if (command == printEnergyLossTable) {
    runAction->PrintEnergyLossTable(newValues);
  }
  if (command == printAnnihilation) {
    runAction -> SetRecordAnnihilationPosition(printAnnihilation ->
                                               GetNewBoolValue(newValues));
  }
  if (command == printTheVolumeNames) {
    runAction -> SetRecordVolumeNames(printTheVolumeNames ->
                                      GetNewBoolValue(newValues));
  }

  if (command == setFillEvGenData) {
    fillEvGenData = setFillEvGenData -> GetNewBoolValue(newValues);
  }

  if (command == setFillAllSDData) {
    fillAllSDData = setFillAllSDData -> GetNewBoolValue(newValues);
  }

  if (command == setFileName) {
    runAction -> setFileName(newValues);
  }
}

//---------------------------------
