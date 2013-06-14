// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37EventMessenger.hh"

K37EventMessenger::K37EventMessenger(K37EventAction *event_action)
    : event_action_(event_action) {
  event_directory_ = new G4UIdirectory("/K37/EventControls/");
  event_directory_ -> SetGuidance("K37 Event controls (thresholds)");

  set_upper_scintillator_threshold_cmd_ = new G4UIcmdWithADoubleAndUnit(
      "/K37/EventControls/ThresholdUpperScintillator", this);
  set_upper_scintillator_threshold_cmd_ -> SetParameterName("Threshold", false);
  set_upper_scintillator_threshold_cmd_ -> SetDefaultValue(100.0 * keV);
  set_upper_scintillator_threshold_cmd_ -> SetUnitCategory("Energy");

  set_lower_scintillator_threshold_cmd_ = new G4UIcmdWithADoubleAndUnit(
      "/K37/EventControls/ThresholdLowerScintillator", this);
  set_lower_scintillator_threshold_cmd_ -> SetParameterName("Threshold", false);
  set_lower_scintillator_threshold_cmd_ -> SetDefaultValue(100.0 * keV);
  set_lower_scintillator_threshold_cmd_ -> SetUnitCategory("Energy");

  set_electron_mcp_threshold_cmd_ = new G4UIcmdWithADoubleAndUnit(
      "/K37/EventControls/ThresholdElectronMCP", this);
  set_electron_mcp_threshold_cmd_ -> SetParameterName("Threshold", false);
  set_electron_mcp_threshold_cmd_ -> SetDefaultValue(2.0 * keV);
  set_electron_mcp_threshold_cmd_ -> SetUnitCategory("Energy");
}

K37EventMessenger::~K37EventMessenger() {
  delete event_directory_;
}

void K37EventMessenger::SetNewValue(G4UIcommand *command, G4String new_values) {
  if (command == set_upper_scintillator_threshold_cmd_) {
    event_action_ ->
        SetUpperScintillatorThreshold(set_upper_scintillator_threshold_cmd_ ->
                                      GetNewDoubleValue(new_values));
  } else if (command == set_lower_scintillator_threshold_cmd_) {
    event_action_ ->
        SetLowerScintillatorThreshold(set_lower_scintillator_threshold_cmd_ ->
                                      GetNewDoubleValue(new_values));
  } else if (command == set_electron_mcp_threshold_cmd_) {
    event_action_ ->
        SetElectronMCPthreshold(set_electron_mcp_threshold_cmd_ ->
                                GetNewDoubleValue(new_values));
  } else {
    G4cout << "Unknown command in event messenger" << G4endl;
  }
}
