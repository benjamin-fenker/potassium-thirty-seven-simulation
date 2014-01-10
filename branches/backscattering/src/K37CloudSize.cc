// Authors: Spencer Behling and Benjamin Fenker 2013

// #include <fstream>

#include "K37CloudSize.hh"
#include "globals.hh"
#include "Randomize.hh"

K37CloudSize::K37CloudSize(G4ThreeVector cloud_center,
                           G4ThreeVector cloud_temperature,
                           G4ThreeVector cloud_size_start,
                           G4ThreeVector sail_velocity)
: cloud_center_(cloud_center),
  initial_cloud_size_(cloud_size_start), temperature_(cloud_temperature),
  sail_velocity_(sail_velocity), cycleTime(2.9472*ms),
  expansion_before_polarized_(300*microsecond), decay_time_(-10),
  half_life_(1.226*s), initialize_complete_(false) {
}

K37CloudSize::~K37CloudSize() {
}

void K37CloudSize::makeEvent() {
  // Position of the decaying particle at the instant the cloud starts expanding
  if (!initialize_complete_) Initialize();

  G4ThreeVector initial_position =
      G4ThreeVector(G4RandGauss::shoot(cloud_center_.x(),
                                       initial_cloud_size_.x()),
                    G4RandGauss::shoot(cloud_center_.y(),
                                       initial_cloud_size_.y()),
                    G4RandGauss::shoot(cloud_center_.z(),
                                       initial_cloud_size_.z()));

  // The trapping laser is off 200 us before the OP laser comes on and it takes
  // ~ 100 us for the atoms to polarize.  Therefore the atoms will have been
  // expanidng for at least 300 us (expansion_before_polarized_) before they
  // will be classified as "polarized" and counted.
  decay_time_ = -10;
  //  decay_time_ = cycleTime;
  while (decay_time_ < expansion_before_polarized_ ||
         decay_time_ > cycleTime) {
    //    decay_time_ = cycleTime*G4UniformRand();
    decay_time_ = G4RandExponential::shoot(half_life_/log(2));
  }

  // Thermal velocity of individual atom (random)
  G4ThreeVector velocity =
      G4ThreeVector(G4RandGauss::shoot(0, velocity_sigma_.x()),
                    G4RandGauss::shoot(0, velocity_sigma_.y()),
                    G4RandGauss::shoot(0, velocity_sigma_.z()));
  // Overall (bulk) movement of the cloud

  velocity += sail_velocity_;

  SetFinalPosition(initial_position, velocity, decay_time_);
   // Outputs a text file with starting x, y, z, vx, vy, vz, time
  std::ofstream ofs;
   ofs.open("cloudData.dat", std::ofstream::out | std::ofstream::app);
   ofs << final_position_.x()/mm << "\t" << final_position_.y()/mm << "\t"
       << final_position_.z()/mm << "\t"
       << velocity.x()/(mm/ns) << "\t" << velocity.y()/(mm/ns) << "\t"
       << velocity.z()/(mm/ns) << "\t" << decay_time_/ns  << G4endl;
   G4cout << decay_time_/ms << G4endl;
}

void K37CloudSize::SetupSigma(G4ThreeVector temperature) {
  velocity_sigma_.setX(CalcSigma(temperature.x()));
  velocity_sigma_.setY(CalcSigma(temperature.y()));
  velocity_sigma_.setZ(CalcSigma(temperature.z()));
}

G4double K37CloudSize::CalcSigma(G4double temperature) {
  // MB velocity distribution for each dimension is gaussian (normal) centered
  // at zero with standard deviation sigma = sqrt(kT/m)
  // G4 keeps masses in terms of energy so the c*c gives it actual velocity
  // units

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  // Mass of Ar37...
  G4double mass = particleTable -> GetIon(18, 37, 0) -> GetPDGMass();
  G4double mean_velocity = sqrt(temperature * k_Boltzmann * c_squared / mass);
  return mean_velocity;
}

void K37CloudSize::SetFinalPosition(G4ThreeVector initial_position,
                                    G4ThreeVector velocity, G4double time) {
  final_position_ = initial_position;
  final_position_ += velocity*time;
//   G4cout << "Initial position: " << initial_position.x()/mm << " mm, "
//          << initial_position.y()/mm << " mm, " << initial_position.z()/mm
//          << " mm" << G4endl;
//   G4cout << "Velocity: " << velocity.x()/(mm/ns) << " mm/ns, "
//          << velocity.y()/(mm/ns) << " mm/ns, " << velocity.z()/(mm/ns)
//          << " mm/ns" << G4endl;
//   G4cout << "Time: " << time/ns << " ns " << G4endl;
//   G4cout << "Final position: " << final_position_.x()/mm << " mm, "
//          << final_position_.y()/mm << " mm, " << final_position_.z()/mm
//          << " mm" << G4endl;
}

void K37CloudSize::SetTemperature(G4ThreeVector temp) {
  temperature_ = temp;
  if (temp.x() < 0 || temp.y() < 0 || temp.z() < 0) {
    G4cout << "ERROR! Cannot set negative temperature" << G4endl;
  } else {
    SetupSigma(temp);
  }
}

void K37CloudSize::SetTemperature(G4double temp) {
  SetTemperature(G4ThreeVector(temp, temp, temp));
}

void K37CloudSize::SetInitialCloudSize(G4ThreeVector size) {
  if (size.x() < 0 || size.y() < 0 || size.z() < 0) {
    G4cout << "ERROR! Cannot set negative cloud size" << G4endl;
  } else {
    initial_cloud_size_ = size;
  }
}

void K37CloudSize::SetInitialCloudSize(G4double size) {
  SetInitialCloudSize(G4ThreeVector(size, size, size));
}

void K37CloudSize::Initialize() {
  SetupSigma(temperature_);
  initialize_complete_ = true;
}

void K37CloudSize::SetSailVelocity(G4ThreeVector vel) {
  sail_velocity_ = vel;
}

void K37CloudSize::SetSailVelocity(G4double vel) {
  SetSailVelocity(G4ThreeVector(vel, vel, vel));
}
