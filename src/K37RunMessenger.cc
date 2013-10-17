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

  set_configuration_file_ =
      new G4UIcmdWithAString("/K37/RunControls/setConfigurationFile", this);
  set_configuration_file_ -> SetGuidance("Enter complete configuration file");
  set_configuration_file_ -> SetParameterName("filename", true);
  set_configuration_file_ -> SetDefaultValue("IOconfiguration.mac");
  set_configuration_file_ -> AvailableForStates(G4State_Idle);

  set_output_directory_ =
      new G4UIcmdWithAString("/K37/RunControls/setOutputDirectory", this);
  set_output_directory_ -> SetGuidance("Enter new output directory");
  set_output_directory_ -> SetParameterName("path", true);
  set_output_directory_ ->
      SetDefaultValue("/home/bfenker/geant4_workdir/K37Build");
  set_output_directory_ -> AvailableForStates(G4State_Idle);

  set_default_cut_cmd_ =
      new G4UIcmdWithADoubleAndUnit("/K37/RunControls/setDefaultCut", this);
  set_default_cut_cmd_ -> SetGuidance("Set secondary production threshold");
  set_default_cut_cmd_ -> SetParameterName("Threshold", false);
  set_default_cut_cmd_ -> SetUnitCategory("Length");
  set_default_cut_cmd_ -> AvailableForStates(G4State_PreInit);
}

//---------------------------------

K37RunMessenger::~K37RunMessenger() {
  delete RunDirectory;
  delete SaveFilesCommand;
  delete printEnergyLossTable;
  delete setFileName;
  delete set_configuration_file_;
  delete set_output_directory_;
  delete printAnnihilation;
  delete printTheVolumeNames;
  delete setFillEvGenData;
  delete setFillAllSDData;
  delete set_default_cut_cmd_;
}



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
    runAction -> SetRecordStripDetectorData(setFillAllSDData ->
                                            GetNewBoolValue(newValues));
  }

  if (command == setFileName) {
    // runAction -> setFileName(newValues);
    FILE *io_file;
    io_file = fopen(runAction -> GetConfigurationFileName(), "w+");
    // printf("root %s/%s.root", runAction -> GetOutputDirectory(),
    //        newValues.c_str());
    fprintf(io_file, "root %s/%s.root", runAction -> GetOutputDirectory(),
            newValues.c_str());
    fclose(io_file);
  }

  if (command == set_configuration_file_) {
    //    char *value_char = newValues.c_str();
    runAction -> SetConfigurationFileName(newValues.c_str());
  }

  if (command == set_output_directory_) {
    runAction -> SetOutputDirectory(newValues.c_str());
  }

  if (command == set_default_cut_cmd_) {
    runAction -> GetPhysicsList() ->
        SetDefaultCutValue(set_default_cut_cmd_ ->
                           GetNewDoubleValue(newValues));
    runAction -> GetPhysicsList() -> SetCuts();
  }
}

//---------------------------------
