// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37EventMessenger.hh"

K37EventMessenger::K37EventMessenger(K37EventAction *event_action)
    : event_action_(event_action) {
  event_directory_ = new G4UIdirectory("/K37/EventControls/");
  event_directory_ -> SetGuidance("K37 Event controls (thresholds)");

  set_upper_scintillator_threshold_cmd_ = new G4UIcmdWithADoubleAndUnit(
      "/K37/EventControls/ThresholdUpperScintillator", this);
  set_upper_scintillator_threshold_cmd_ ->
      SetGuidance("\n\tThis command is obsolete and should not be used.\n\t"
                  "Try /K37/Detectors/scintillatorPlusZ/setThreshold");
  set_upper_scintillator_threshold_cmd_ -> SetParameterName("Threshold", false);
  set_upper_scintillator_threshold_cmd_ -> SetDefaultValue(100.0 * keV);
  set_upper_scintillator_threshold_cmd_ -> SetUnitCategory("Energy");

  set_lower_scintillator_threshold_cmd_ = new G4UIcmdWithADoubleAndUnit(
      "/K37/EventControls/ThresholdLowerScintillator", this);
  set_lower_scintillator_threshold_cmd_ ->
      SetGuidance("\n\tThis command is obsolete and should not be used.\n\t"
                  "Try /K37/Detectors/scintillatorPlusZ/setThreshold");
  set_lower_scintillator_threshold_cmd_ -> SetParameterName("Threshold", false);
  set_lower_scintillator_threshold_cmd_ -> SetDefaultValue(100.0 * keV);
  set_lower_scintillator_threshold_cmd_ -> SetUnitCategory("Energy");

  set_electron_mcp_threshold_cmd_ = new G4UIcmdWithADoubleAndUnit(
      "/K37/EventControls/ThresholdElectronMCP", this);
  set_electron_mcp_threshold_cmd_ -> SetParameterName("Threshold", false);
  set_electron_mcp_threshold_cmd_ -> SetDefaultValue(2.0 * keV);
  set_electron_mcp_threshold_cmd_ -> SetUnitCategory("Energy");

  set_count_backscatters_cmd_ =
      new G4UIcmdWithABool("/K37/EventControls/setCountBackscatters", this);
  set_count_backscatters_cmd_ -> SetParameterName("choice", true);
  set_count_backscatters_cmd_ -> SetDefaultValue(true);
}

K37EventMessenger::~K37EventMessenger() {
  delete event_directory_;
  delete set_upper_scintillator_threshold_cmd_;
  delete set_lower_scintillator_threshold_cmd_;
  delete set_electron_mcp_threshold_cmd_;
  delete set_count_backscatters_cmd_;
}

void K37EventMessenger::SetNewValue(G4UIcommand *command, G4String new_values) {
  if (command == set_upper_scintillator_threshold_cmd_) {
    G4cout << "This command is obsolete and does nothing." << G4endl;
    G4cout << "Try: /K37/Detectors/scintillatorPlusZ/setThreshold" << G4endl;
    G4cout << "Enter any number to continue" << G4endl;
    G4int j;
    G4cin >> j;
    // event_action_ ->
    //    SetUpperScintillatorThreshold(set_upper_scintillator_threshold_cmd_ ->
    //                                      GetNewDoubleValue(new_values));
  } else if (command == set_lower_scintillator_threshold_cmd_) {
    // event_action_ ->
    //   SetLowerScintillatorThreshold(set_lower_scintillator_threshold_cmd_ ->
    //                                   GetNewDoubleValue(new_values));
    G4cout << "This command is obsolete and does nothing." << G4endl;
    G4cout << "Try: /K37/Detectors/scintillatorMinusZ/setThreshold" << G4endl;
    G4cout << "Enter any number to continue" << G4endl;
    G4int j;
    G4cin >> j;
  } else if (command == set_electron_mcp_threshold_cmd_) {
    event_action_ ->
        SetElectronMCPthreshold(set_electron_mcp_threshold_cmd_ ->
                                GetNewDoubleValue(new_values));
  } else if (command == set_count_backscatters_cmd_) {
    event_action_ ->
        SetCountBackscatters(set_count_backscatters_cmd_ ->
                             GetNewBoolValue(new_values));
  } else {
   G4cout << "Unknown command in event messenger" << G4endl;
 }
}
