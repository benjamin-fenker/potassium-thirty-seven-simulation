// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ScintillatorMessenger.hh"

K37ScintillatorMessenger::K37ScintillatorMessenger(
    K37ScintillatorDigitizer *digitizer) :
    digitizer_(digitizer) {

  G4UIdirectory *my_dir = new G4UIdirectory("/K37/Detectors/");
  my_dir -> SetGuidance("K37 Detector controls");
  

  G4String dir_name = "/K37/Detectors/" + digitizer -> GetName() + "/";
  dir_ = new G4UIdirectory(dir_name);
  dir_ -> SetGuidance("Control threshold, resolution of scintillator");

  threshold_cmd_ =
      new G4UIcmdWithADoubleAndUnit((dir_name + "setThreshold").c_str(), this);
  threshold_cmd_ -> SetGuidance("Set threshold above which scintillator will\n"
                                "\tread timing information");
  threshold_cmd_ -> SetParameterName("Threshold", false); // Not omitable
  threshold_cmd_ -> SetUnitCandidates("ev keV MeV");
  threshold_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);

  resolution_sigma0_cmd_ =
      new G4UIcmdWithADoubleAndUnit((dir_name + "setSigma0").c_str(), this);
  resolution_sigma0_cmd_ -> SetGuidance("Set sigma0 used to calculate energy\n"
                                       "\tresolution (s^2 = s0^2 + lambda*E)");
  resolution_sigma0_cmd_ -> SetParameterName("sigma0", false); // Not omitable
  resolution_sigma0_cmd_ -> SetUnitCandidates("ev keV MeV");
  resolution_sigma0_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);

  resolution_lambda_cmd_ =
      new G4UIcmdWithADoubleAndUnit((dir_name + "setLambda").c_str(), this);
  resolution_lambda_cmd_ -> SetGuidance("Set lambda used to calculate energy\n"
                                    "\tresolution (s^2 = s0^2 + lambda*E)");
  resolution_lambda_cmd_ -> SetParameterName("lambda", false);
  resolution_lambda_cmd_ -> SetUnitCandidates("eV keV MeV");
  resolution_lambda_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);

  apply_resolution_cmd_ =
      new G4UIcmdWithABool((dir_name + "applyResolution").c_str(), this);
  apply_resolution_cmd_ -> SetGuidance("Apply finite resolution?");
  apply_resolution_cmd_ -> AvailableForStates(G4State_PreInit, G4State_Idle);
}

K37ScintillatorMessenger::~K37ScintillatorMessenger() {
  delete dir_;
  delete threshold_cmd_;
  delete resolution_sigma0_cmd_;
  delete resolution_lambda_cmd_;
  delete apply_resolution_cmd_;
}

void K37ScintillatorMessenger::SetNewValue(G4UIcommand *cmd, G4String value) {
  if (cmd == threshold_cmd_) {
    digitizer_ -> SetThreshold(threshold_cmd_ -> GetNewDoubleValue(value));
  } else if(cmd == resolution_sigma0_cmd_) {
    digitizer_ ->
        SetResolutionSigma0(resolution_sigma0_cmd_ -> GetNewDoubleValue(value));
  } else if(cmd == resolution_lambda_cmd_) {
    digitizer_ ->
        SetResolutionLambda(resolution_lambda_cmd_ -> GetNewDoubleValue(value));
  } else if (cmd == apply_resolution_cmd_) {
    digitizer_ ->
        SetApplyFiniteResolution(apply_resolution_cmd_ ->
                                 GetNewBoolValue(value));
  } else {
    G4cout << "Error in K37ScintillatorMessenger::SetNewValue" << G4endl;
  }
}
