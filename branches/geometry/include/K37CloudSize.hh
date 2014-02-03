// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37CloudSize_h
#define K37CloudSize_h 1

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class K37CloudSize {
  ///
  /// \class K37CloudSize  K37CloudSize.hh K37CloudSize
  /// \brief This is a class to change the cload size based on temperature
  /// \author Spencer Behling
  /// \date Jan. 5 2012
  /// \since version 1.0
  ///
 public:
  K37CloudSize(G4ThreeVector cloud_center, G4ThreeVector temperature,
               G4ThreeVector  initial_size, G4ThreeVector sail_velocity);
  ~K37CloudSize();
  void makeEvent();
  G4ThreeVector GetFinalPosition() {return final_position_;}
  G4ThreeVector GetCloudCenter() {return cloud_center_;}
  G4ThreeVector GetInitialCloudSize() {return initial_cloud_size_;}
  G4ThreeVector GetTemperature() {return temperature_;}
  G4ThreeVector GetSailVelocity() {return sail_velocity_;}
  void SetCloudCenter(G4ThreeVector center) {cloud_center_ = center;}
  void SetTemperature(G4ThreeVector temp);
  void SetTemperature(G4double temp);
  void SetInitialCloudSize(G4ThreeVector size);
  void SetInitialCloudSize(G4double size);
  void SetSailVelocity(G4ThreeVector vel);
  void SetSailVelocity(G4double vel);
  G4double GetDecayTime() {return decay_time_;}

 private:
  G4ThreeVector cloud_center_;
  G4ThreeVector initial_cloud_size_;
  G4ThreeVector velocity_sigma_;
  G4ThreeVector final_position_;
  G4ThreeVector temperature_;
  G4ThreeVector sail_velocity_;

  G4double cycleTime;
  G4double expansion_before_polarized_;
  G4double decay_time_;
  void SetupSigma(G4ThreeVector temperature);
  G4double CalcSigma(G4double temperature);
  void SetFinalPosition(G4ThreeVector initial_position, G4ThreeVector velocity,
                        G4double time);
  G4bool initialize_complete_;
  void Initialize();
};

#endif
