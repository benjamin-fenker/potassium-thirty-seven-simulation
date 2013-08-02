// Authors: Spencer Behling and Benjamin Fenker 2013

#include <TArrayD.h>

#include "GeantAggregator.hh"

GeantAggregator::GeantAggregator() {}

void GeantAggregator::EndRun() {
  TArrayD *parameters = new TArrayD(14);
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

  TFile *file = Aggregator::GetRootFile();
  file -> WriteObject(parameters, "RunParameters");
  Aggregator::EndRun();
}

