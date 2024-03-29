// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef GeantAggregator_h
#define GeantAggregator_h 1

#include "Aggregator.hh"
#include "K37CloudSize.hh"
#include "K37ElectricFieldSetup.hh"
#include "K37EventAction.hh"
#include "K37PhysicsList.hh"
#include "K37PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"

class GeantAggregator : public AGG::Aggregator {
 public:
  GeantAggregator();
  void SetPrimaryGeneratorAction(K37PrimaryGeneratorAction *pga) {
    primary_generator_action_ = pga;
  }
  void SetElectricFieldSetup(K37ElectricFieldSetup *efs) {
    electric_field_setup_ = efs;
  }
  void SetCloudSize(K37CloudSize *cs) {cloud_size_ = cs;}
  void SetEventAction(K37EventAction *ea) {event_action_ = ea;}
  void SetPhysicsList(PhysicsList *list) {physics_list_ = list;}
  void EndRun();

 private:
  K37PrimaryGeneratorAction *primary_generator_action_;
  K37ElectricFieldSetup *electric_field_setup_;
  K37CloudSize *cloud_size_;
  K37EventAction *event_action_;
  PhysicsList *physics_list_;
};

#endif
