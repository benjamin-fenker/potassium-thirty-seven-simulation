// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37PrimaryGeneratorMessenger.hh"


// ----------------------------------

K37PrimaryGeneratorMessenger::K37PrimaryGeneratorMessenger(
                                  K37PrimaryGeneratorAction* Gun)
  :action_(Gun) {
  new G4UnitDefinition("millimeter/nanosecond", "mm/ns", "Velocity", mm/ns);

  gunDir = new G4UIdirectory("/K37/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");

  set_pol_cmd_ = new G4UIcmdWithADouble("/K37/gun/setPolarization", this);
  set_pol_cmd_ -> SetGuidance("Enter a new polarization (-1 -> 1)");
  set_pol_cmd_ -> SetParameterName("Polarization", false);
  set_pol_cmd_ -> SetDefaultValue(1.0);

  set_ali_cmd_ = new G4UIcmdWithADouble("/K37/gun/setAlignment", this);
  set_ali_cmd_ -> SetGuidance("Enter a new alignment (-1 -> 1)");
  set_ali_cmd_ -> SetParameterName("Alignment", false);
  set_ali_cmd_ -> SetDefaultValue(1.0);

  set_recoil_charge_cmd_ = new G4UIcmdWithAnInteger("/K37/gun/setRecoilCharge",
                                                    this);
  set_recoil_charge_cmd_ -> SetGuidance("Enter charge state of recoil Ar.");
  set_recoil_charge_cmd_ -> SetGuidance("Available charge states: +1 -> +3");
  set_recoil_charge_cmd_ -> SetParameterName("Charge state", true);
  set_recoil_charge_cmd_ -> SetDefaultValue(1);

  set_cloud_temperature_v_ =
      new G4UIcmdWith3VectorAndUnit("/K37/gun/setTemperatureV", this);
  set_cloud_temperature_v_ -> SetGuidance("Enter temperature for the atom");
  set_cloud_temperature_v_ -> SetGuidance("as a vector");
  set_cloud_temperature_v_ -> SetParameterName("T_x", "T_y", "T_z", true);
  set_cloud_temperature_v_ -> SetDefaultValue(G4ThreeVector(0.0, 0.0, 0.0));


  set_cloud_temperature_v_ -> SetUnitCategory("Temperature");

  set_cloud_temperature_d_ =
      new G4UIcmdWithADoubleAndUnit("/K37/gun/setTemperatureD", this);
  set_cloud_temperature_d_ -> SetGuidance("Enter temperature for the atoms.");
  set_cloud_temperature_d_ -> SetGuidance("Note that this will be applied to");
  set_cloud_temperature_d_ -> SetGuidance(" every direction!");
  set_cloud_temperature_d_ -> SetParameterName("Temperature", true);
  set_cloud_temperature_d_ -> SetDefaultValue(0.0);
  set_cloud_temperature_d_ -> SetUnitCategory("Temperature");

  set_initial_cloud_size_v_ =
      new G4UIcmdWith3VectorAndUnit("/K37/gun/setCloudSizeV", this);
  set_initial_cloud_size_v_ -> SetGuidance("Enter cloud size (sigma) at the");
  set_initial_cloud_size_v_ -> SetGuidance(" start of the optical pumping");
  set_initial_cloud_size_v_ -> SetGuidance(" cycle.");
  set_initial_cloud_size_v_ -> SetParameterName("wx", "wy", "wz", true);
  set_initial_cloud_size_v_ -> SetDefaultValue(G4ThreeVector(0.0, 0.0, 0.0));
  set_initial_cloud_size_v_ -> SetUnitCategory("Length");

  set_initial_cloud_size_d_ =
      new G4UIcmdWithADoubleAndUnit("/K37/gun/setCloudSizeD", this);
  set_initial_cloud_size_d_ -> SetGuidance("Enter spherical cloud size");
  set_initial_cloud_size_d_ -> SetGuidance(" (sigma) at the start of the");
  set_initial_cloud_size_d_ -> SetGuidance(" optical pumping cycle.");
  set_initial_cloud_size_d_ -> SetParameterName("width (sigma)", true);
  set_initial_cloud_size_d_ -> SetDefaultValue(0.0);
  set_initial_cloud_size_d_ -> SetUnitCategory("Length");

  set_cloud_center_ =
      new G4UIcmdWith3VectorAndUnit("/K37/gun/setCloudCenter", this);
  set_cloud_center_ -> SetGuidance("Enter the coordinates of the center of ");
  set_cloud_center_ -> SetGuidance("the cloud at the start of the optical ");
  set_cloud_center_ -> SetGuidance("pumping cycle.");
  set_cloud_center_ -> SetParameterName("x", "y", "z", true);
  set_cloud_center_ -> SetDefaultValue(G4ThreeVector(0.0, 0.0, 0.0));
  set_cloud_center_ -> SetUnitCategory("Length");

  set_make_beta_ = new G4UIcmdWithABool("/K37/gun/setMakeBeta", this);
  set_make_beta_ -> SetGuidance("Enter bool to generate primary betas or not");
  set_make_beta_ -> SetParameterName("Make Recoils", true);
  set_make_beta_ -> SetDefaultValue(true);

  set_make_recoil_ = new G4UIcmdWithABool("/K37/gun/setMakeRecoil", this);
  set_make_recoil_ ->
      SetGuidance("Enter bool to generate primary recoils or not");
  set_make_recoil_ -> SetParameterName("Make Recoils", true);
  set_make_recoil_ -> SetDefaultValue(true);

  set_make_shakeoff_electrons_ =
      new G4UIcmdWithABool("/K37/gun/setMakeShakeoffElectrons", this);
  set_make_shakeoff_electrons_ ->
      SetGuidance("Enter bool to generate primary SOEs or not");
  set_make_shakeoff_electrons_ -> SetParameterName("Make SOEs", true);
  set_make_shakeoff_electrons_ -> SetDefaultValue(true);

  set_cloud_sail_velocity_v_ =
      new G4UIcmdWith3VectorAndUnit("/K37/gun/setSailVelocity", this);
  set_cloud_sail_velocity_v_ -> SetGuidance("Enter the cloud's sail velocity");
  set_cloud_sail_velocity_v_ -> SetParameterName("vx", "vy", "vz", false);
  set_cloud_sail_velocity_v_ -> SetUnitCategory("Velocity");

  set_cloud_sail_velocity_d_ =
      new G4UIcmdWithADoubleAndUnit("/K37/gun/setSailVelocityD", this);
  set_cloud_sail_velocity_d_ -> SetGuidance("Enter the cloud's sail velocity");
  set_cloud_sail_velocity_d_ -> SetParameterName("velocity", false);
  set_cloud_sail_velocity_d_ -> SetUnitCategory("Velocity");
}

// ----------------------------------

K37PrimaryGeneratorMessenger::~K37PrimaryGeneratorMessenger() {
  //  delete RndmCmd;
  delete gunDir;
  delete set_pol_cmd_;
  delete set_ali_cmd_;
  delete set_recoil_charge_cmd_;
  delete set_cloud_temperature_v_;
  delete set_cloud_temperature_d_;
  delete set_initial_cloud_size_v_;
  delete set_initial_cloud_size_d_;
  delete set_cloud_center_;
  delete set_make_beta_;
  delete set_make_recoil_;
  delete set_make_shakeoff_electrons_;
  delete set_cloud_sail_velocity_v_;
  delete set_cloud_sail_velocity_d_;
}

// ----------------------------------

void K37PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue) {
  // if (command == RndmCmd) {
  //   action_ -> setRandomFlag(newValue);
  // }
  if (command == set_pol_cmd_) {
    action_ -> SetPolarization(set_pol_cmd_->GetNewDoubleValue(newValue));
  }
  if (command == set_ali_cmd_) {
    action_ -> SetAlignment(set_ali_cmd_->GetNewDoubleValue(newValue));
  }
  if (command == set_recoil_charge_cmd_) {
    action_ ->
        SetRecoilCharge(set_recoil_charge_cmd_ -> GetNewIntValue(newValue));
  }
  if (command == set_cloud_temperature_v_) {
    action_ -> GetCloudSize() -> SetTemperature(set_cloud_temperature_v_ ->
                                                GetNew3VectorValue(newValue));
  }
  if (command == set_cloud_temperature_d_) {
    action_ -> GetCloudSize() -> SetTemperature(set_cloud_temperature_d_ ->
                                                GetNewDoubleValue(newValue));
  }
  if (command == set_initial_cloud_size_v_) {
    action_ ->
        GetCloudSize() ->  SetInitialCloudSize(set_initial_cloud_size_v_ ->
                                               GetNew3VectorValue(newValue));
  }
  if (command == set_initial_cloud_size_d_) {
    action_ -> GetCloudSize() ->
        SetInitialCloudSize(set_initial_cloud_size_d_ ->
                            GetNewDoubleValue(newValue));
  }
  if (command == set_cloud_center_) {
    action_ -> GetCloudSize() ->
        SetCloudCenter(set_cloud_center_ -> GetNew3VectorValue(newValue));
  }
  if (command == set_make_beta_) {
    action_ -> SetMakeBeta(set_make_beta_ -> GetNewBoolValue(newValue));
  }
  if (command == set_make_recoil_) {
    action_ -> SetMakeRecoil(set_make_recoil_ -> GetNewBoolValue(newValue));
  }
  if (command == set_make_shakeoff_electrons_) {
    action_ -> SetMakeShakeoffElectrons(set_make_shakeoff_electrons_ ->
                                        GetNewBoolValue(newValue));
  }
  if (command == set_cloud_sail_velocity_v_) {
    action_ -> GetCloudSize() -> SetSailVelocity(set_cloud_sail_velocity_v_ ->
                                                 GetNew3VectorValue(newValue));
  }
  if (command == set_cloud_sail_velocity_d_) {
    action_ -> GetCloudSize() -> SetSailVelocity(set_cloud_sail_velocity_d_ ->
                                                 GetNewDoubleValue(newValue));
  }
}

// ----------------------------------

