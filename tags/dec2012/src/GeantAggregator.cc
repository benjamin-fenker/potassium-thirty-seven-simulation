// Authors: Spencer Behling and Benjamin Fenker 2013
#include <TArrayD.h>

#include "globals.hh"

#include "GeantAggregator.hh"

GeantAggregator::GeantAggregator() {}

void GeantAggregator::EndRun() {
  TArrayD *parameters = new TArrayD(23);

  // Get digitizer modules to look up thresholds
  G4DigiManager *digitizer_manager(G4DigiManager::GetDMpointer());
  K37ScintillatorDigitizer *upper_scintillator_digitizer(
      static_cast<K37ScintillatorDigitizer *>(
          digitizer_manager -> FindDigitizerModule("scintillatorPlusZ")));

  K37ScintillatorDigitizer *lower_scintillator_digitizer(
      static_cast<K37ScintillatorDigitizer *>(
          digitizer_manager -> FindDigitizerModule("scintillatorMinusZ")));

  parameters -> AddAt(primary_generator_action_ -> GetPolarization()       , 0);
  parameters -> AddAt(primary_generator_action_ -> GetAlignment()          , 1);
  parameters -> AddAt(electric_field_setup_ ->
                      GetConstantFieldValue().x()/(volt/cm) , 2);
  parameters -> AddAt(electric_field_setup_ ->
                      GetConstantFieldValue().y()/(volt/cm) , 3);
  parameters -> AddAt(electric_field_setup_ ->
                      GetConstantFieldValue().z()/(volt/cm) , 4);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetCloudCenter().x()/mm, 5);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetCloudCenter().y()/mm, 6);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetCloudCenter().z()/mm, 7);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetInitialCloudSize().x()/mm, 8);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetInitialCloudSize().y()/mm, 9);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetInitialCloudSize().z()/mm, 10);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetTemperature().x()/kelvin, 11);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetTemperature().y()/kelvin, 12);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetTemperature().z()/kelvin, 13);
  parameters -> AddAt(primary_generator_action_ -> GetRecoilCharge(), 14);
  parameters -> AddAt(event_action_ -> GetElectronMCPthreshold(), 15);
  parameters -> AddAt(upper_scintillator_digitizer -> GetThreshold(), 16);
  parameters -> AddAt(lower_scintillator_digitizer -> GetThreshold(), 17);
  parameters -> AddAt(physics_list_ -> GetDefaultCutValue()/micrometer, 18);
  G4String list_str = physics_list_ -> GetEmName();
  double list_code = -1;
  if (list_str == "local") list_code = 4;
  if (list_str == "emstandard_opt0") list_code = 0;
  if (list_str == "emstandard_opt1") list_code = 1;
  if (list_str == "emstandard_opt2") list_code = 2;
  if (list_str == "emstandard_opt3") list_code = 3;
  if (list_str == "standardSS") list_code = 5;
  if (list_str == "standardNR") list_code = 6;
  if (list_str == "empenelope") list_code = 7;
  if (list_str == "emlivermore") list_code = 8;
  if (list_str == "K37") list_code = 9;
  parameters -> AddAt(list_code, 19);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetSailVelocity().x()/(mm/ns), 20);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetSailVelocity().y()/(mm/ns), 21);
  parameters -> AddAt(primary_generator_action_ -> GetCloudSize() ->
                      GetSailVelocity().z()/(mm/ns), 22);
  TFile *file = Aggregator::GetRootFile();
  file -> WriteObject(parameters, "RunParameters");
  //delete parameters;
  //  G4cout << "Wrote RunParameters" << G4endl;
  Aggregator::EndRun();
}

