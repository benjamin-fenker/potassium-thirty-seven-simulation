// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37StripDetectorMessenger.hh"

K37StripDetectorMessenger::K37StripDetectorMessenger(
    K37StripDetectorDigitizer *digitizer) :
    digitizer_(digitizer) {

  G4UIdirectory *my_dir = new G4UIdirectory("/K37/Detectors/");
  my_dir -> SetGuidance("K37 Detector controls");
  

  G4String dir_name = "/K37/Detectors/" + digitizer -> GetName() + "/";
  dir_ = new G4UIdirectory(dir_name);
  dir_ -> SetGuidance("Control resolution of strip detector");

  resolution_fnameX_cmd_ =
      new G4UIcmdWithAString((dir_name + "setResolutionFileX").c_str(), this);
  resolution_fnameX_cmd_ -> SetGuidance("Set filename (full path) of file\n"
                                        "\twith X-strip resolutions");

  resolution_fnameX_cmd_ -> SetParameterName("File name (X)", false);
  resolution_fnameX_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);

  resolution_fnameY_cmd_ =
      new G4UIcmdWithAString((dir_name + "setResolutionFileY").c_str(), this);
  resolution_fnameY_cmd_ -> SetGuidance("Set filename (full path) of file\n"
                                        "\twith Y-strip resolutions");
  resolution_fnameY_cmd_ -> SetParameterName("File name (Y)", false);
  resolution_fnameY_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);

  apply_resolution_cmd_ =
      new G4UIcmdWithABool((dir_name + "applyResolution").c_str(), this);
  apply_resolution_cmd_ -> SetGuidance("Apply finite resolution?");
  apply_resolution_cmd_ -> SetParameterName("Apply finite resolution?", false);
  apply_resolution_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);
}

K37StripDetectorMessenger::~K37StripDetectorMessenger() {
  delete dir_;
  delete resolution_fnameX_cmd_;
  delete resolution_fnameY_cmd_;
  delete apply_resolution_cmd_;
}

void K37StripDetectorMessenger::SetNewValue(G4UIcommand *cmd,
                                            G4String value) {
  if (cmd == resolution_fnameX_cmd_) {
    digitizer_ -> SetResolutionX(value.c_str());
  } else if (cmd == resolution_fnameY_cmd_) {
    digitizer_ -> SetResolutionY(value.c_str());
  } else if (cmd == apply_resolution_cmd_) {
    digitizer_ -> SetApplyFiniteResolution(apply_resolution_cmd_ ->
                                           GetNewBoolValue(value));
  }
}
