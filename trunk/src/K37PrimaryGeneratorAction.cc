// Authors: Spencer Behling and Benjamin Fenker 2013

#include <iomanip>
#include <cmath>

#include "K37PrimaryGeneratorAction.hh"
#include "K37DetectorConstruction.hh"
#include "K37PrimaryGeneratorMessenger.hh"
#include "K37AllPossibleEventInformation.hh"
#include "K37CloudSize.hh"
#include "K37EventGenerator.hh"


#include "globals.hh"
#include "G4DecayTable.hh"
#include "G4DynamicParticle.hh"
#include "G4Event.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4SingleParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"


K37PrimaryGeneratorAction::K37PrimaryGeneratorAction(
                           K37DetectorConstruction* det,
                           K37AllPossibleEventInformation* APEI,
                           K37EventGenerator* evGen) :
    polarization_(1.0), alignment_(1.0), charge_state_ratio_(8, 1.0/8.0),
    detector(det), randomFlag("on"), EventInformation(APEI), v(), vertex(NULL),
    EventVertex(), K37Neutral(NULL), K37Minus(NULL), decayTableAr37Minus(NULL),
    K37MinusDecayMode(NULL), cloud(NULL), evGenerator(evGen) {
  gunMessenger = new K37PrimaryGeneratorMessenger(this);
  insideCollimator = detector->GetSubtraction();
  distanceToTrap =detector->GetDistanceToTrap();
  // G4cout <<"Subtraction: " << insideCollimator << " Distance to Trap (cm): "
  //        << distanceToTrap << G4endl;

  // G4int n_particle = 1;
  // particleGun = new G4ParticleGun(n_particle);

  //                     temp , size

  charge_state_ratio_[0] = 1.0;
  charge_state_ratio_[1] = 0.69699;
  charge_state_ratio_[2] = 0.19483;
  charge_state_ratio_[3] = 0.08173;
  charge_state_ratio_[4] = 0.01953;
  charge_state_ratio_[5] = 0.00581;
  charge_state_ratio_[6] = 0.00110;
  charge_state_ratio_[7] = 0.0;
  NormalizeChargeStateRatio();

  cloud = new K37CloudSize(G4ThreeVector(1.07*mm, 1.07*mm, -2.05*mm),
                           G4ThreeVector(0.00029*kelvin, 0.00029*kelvin,
                                         0.0014*kelvin),
                           G4ThreeVector(0.51*mm, 0.51*mm, 0.64*mm));

  particleGun = new G4SingleParticleSource();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  // ion = particleTable->GetIon(18, 37, 0);
  positron = particleTable->FindParticle(particleName="e+");
  electron = particleTable->FindParticle(particleName="e-");
  Ar37MinusParticle = particleTable->FindParticle(particleName="Ar37Minus");
  recoil_charge_ = 1.0;

  // G4cout<<"The fermi function from evGenerator is: "<<G4endl;
  // evGenerator->MakeEvent();
  // G4IonTable* ionTable = particleTable-> GetIonTable();
  // ionTable->DumpTable();
  // particleTable->DumpTable();
}

K37PrimaryGeneratorAction::~K37PrimaryGeneratorAction() {
  delete particleGun;
  delete gunMessenger;
  delete K37Neutral;
  delete K37Minus;
  delete decayTableAr37Minus;
  delete K37MinusDecayMode;
  delete cloud;
}

void K37PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  G4double recoil_charge_this_event;
  if (recoil_charge_ == -2) {
    recoil_charge_this_event = GetChargeStateThisEvent();
  } else {
    recoil_charge_this_event = recoil_charge_ * eplus;
  }
  (*active_channels_)["ION_CHARGE"] ->  InsertData(recoil_charge_this_event);

  bool testingEVGenerator = false;

  evGenerator -> MakeEvent(polarization_, alignment_, recoil_charge_this_event);

  //  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //  ion = particleTable->FindIon(18, 37, 0);

  if (!testingEVGenerator) {
    cloud -> makeEvent();
    EventVertex = cloud -> GetFinalPosition();

    // EventVertex.set(0*mm, 0*mm, 0*mm);
    // G4cout << "Starting from x = " << cloud -> xFinal()/mm << G4endl
    //        << "\t y = "  << cloud -> yFinal()/mm << G4endl << "\t z = "
    //        << cloud -> zFinal()/mm << G4endl;
    EventInformation->
      setMetaStableTimeOfDeath(CLHEP::RandExponential::shoot(260));

    EventInformation->setVertexPosition(EventVertex);
    EventInformation->setOriginalTheta(EventVertex.theta());

    particleGun->SetParticlePosition(EventVertex);

    if (randomFlag == "on") {
      // evMaker->MakeEvent();
      rho = 1;
      vertex = new G4PrimaryVertex(EventVertex, 0);
      this->setBetaVertex();
      anEvent->AddPrimaryVertex(vertex);

      this->setDaughterVertex(recoil_charge_this_event);
      anEvent->AddPrimaryVertex(vertex);
      EventInformation->
        setDaughterMomentum(G4ThreeVector(evGenerator->dMomentumX(),
                                          evGenerator->dMomentumY(),
                                          evGenerator->dMomentumZ()));
      SetSOelectronVertices(anEvent, recoil_charge_this_event + 1);
    } else {
      vertex = new G4PrimaryVertex(EventVertex, 0);
      G4PrimaryParticle* particle =
        new G4PrimaryParticle(electron, 0, -1097.906*keV, 0);
      vertex->SetPrimary(particle);
      anEvent->AddPrimaryVertex(vertex);
    }
  }
}

G4double K37PrimaryGeneratorAction::getVelocity(G4double kineticEnergy,
                                                G4double massOfParticle) {
  G4double velocityOverC = sqrt(1.0 -
                                (1.0/((kineticEnergy/massOfParticle + 1.0)*
                                      (kineticEnergy/massOfParticle + 1.0))));
  return velocityOverC;
}

void K37PrimaryGeneratorAction::setBetaVertex() {
  vertex = new G4PrimaryVertex(EventVertex, 0);
  G4PrimaryParticle* particle =
    new G4PrimaryParticle(positron, evGenerator->eMomentumX(),
                          evGenerator->eMomentumY(), evGenerator->eMomentumZ());
  vertex->SetPrimary(particle);
}

void K37PrimaryGeneratorAction::setDaughterVertex(G4double recoil_charge) {
  // G4ThreeVector momentum(evGenerator -> dMomentumX(),
  //                        evGenerator -> dMomentumY(),
  //                        evGenerator -> dMomentumZ());
  G4bool debug = false;
  G4ThreeVector momentum(0, 0, 0);
  vertex = new G4PrimaryVertex(EventVertex, 0);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *ion = particleTable -> GetIon(18, 37, 0);  // 37Ar
  G4PrimaryParticle* particle =
      new G4PrimaryParticle(ion, evGenerator->dMomentumX(),
                            evGenerator->dMomentumY(),
                            evGenerator->dMomentumZ());
  // Simulate any charge state >= +1
  particle -> SetCharge(recoil_charge * eplus);
  if (debug) {
    G4cout << "Ion mass: " << ion -> GetPDGMass()/c_squared/kg << " kg"
           << G4endl;
    G4cout << "Ion velocity: " << G4endl << "v0_x = "
           << (particle -> GetPx()/(particle -> GetMass())*c_light)/(mm/ns)
           << " mm/ns" << G4endl << "v0_y = "
           << (particle -> GetPy()/(particle -> GetMass())*c_light)/(mm/ns)
           << " mm/ns" << G4endl << "v0_z = "
           << (particle -> GetPz()/(particle -> GetMass())*c_light)/(mm/ns)
           << " mm/ns" << G4endl;
    G4cout << "Ion momentum: "
           << particle -> GetTotalMomentum()/(c_light*kg*m/s) << " kg*m/s"
           << G4endl;
    G4cout << "Ion kinetic energy: " << particle -> GetKineticEnergy()/joule
           << " J " << G4endl;
  }
  vertex->SetPrimary(particle);
}

void K37PrimaryGeneratorAction::SetSOelectronVertices(G4Event *ev,
                                                      G4int num_so_electrons) {
  G4bool debug = false;
  for (G4int i = 0; i < num_so_electrons; i++) {
    G4double kinetic_energy = -1.0, total_energy, momentum, mu, phi, theta;
    G4double px, py, pz;
    // Ar binding energy is 15.7 eV and SOE have around this energy
    // The 5.0 eV width is a total guess
    while (kinetic_energy < 0.0) {
      kinetic_energy = CLHEP::RandGauss::shoot(15.7*eV, 5.0*eV);
    }
    total_energy = kinetic_energy + electron_mass_c2;

    momentum = sqrt((total_energy * total_energy) -
                    (electron_mass_c2 * electron_mass_c2));
    mu = 1.0 - 2.0*G4UniformRand();
    theta = acos(mu);
    phi = 2.0*M_PI*G4UniformRand();
    px = momentum * sin(theta) * cos(phi);
    py = momentum * sin(theta) * sin(phi);
    pz = momentum * cos(theta);
    if (debug) {
      G4cout << "SOE " << i << G4endl;
      G4cout << "\tT = " << G4BestUnit(kinetic_energy, "Energy") << "\tE = "
             << G4BestUnit(total_energy, "Energy") << "\tP = "
             << momentum << G4endl;
      G4cout << "\ttheta = " << theta << "\tphi = " << phi << G4endl;
      G4cout << "\tpx = " << px << "\tpy = " << py << "\tpz = " << pz << G4endl;
    }
    G4PrimaryParticle *particle = new G4PrimaryParticle(electron, px, py, pz);
    vertex = new G4PrimaryVertex(EventVertex, 0);  // 0 means occurs at t = 0
    vertex -> SetPrimary(particle);
    ev -> AddPrimaryVertex(vertex);
  }
}

void K37PrimaryGeneratorAction::SetPolarization(G4double pol) {
  if (fabs(pol) <= 1.0) {
    polarization_ = pol;
  } else {
    G4cout << "WARNING: Polarization " << pol
           << " not in allowed range.  No changes made." << G4endl;
  }
}

void K37PrimaryGeneratorAction::SetAlignment(G4double ali) {
  if (fabs(ali) <= 1.0) {
    alignment_ = ali;
  } else {
    G4cout << "WARNING: Alignment " << ali
           << " not in allowed range.  No changes made." << G4endl;
  }
}

void K37PrimaryGeneratorAction::SetRecoilCharge(G4int charge) {
  if (charge < 0 && charge != -2) {
    G4cout << "Negative ions not supported as primary particles. "
           << " No change...Recoil charge = " << recoil_charge_ << G4endl;
  } else if (charge == -2) {
    G4cout << "Randomizing charge state based on compiled distribution"
           << G4endl;
    recoil_charge_ = charge;
  } else {                            // Positive ions action
    recoil_charge_ = charge;
  }
}

void K37PrimaryGeneratorAction::NormalizeChargeStateRatio() {
  G4double sum = 0.0;
  for (unsigned int i = 0; i < charge_state_ratio_.size(); i++) {
    sum += charge_state_ratio_[i];
  }
  for (unsigned int i = 0; i < charge_state_ratio_.size(); i++) {
    charge_state_ratio_[i] = charge_state_ratio_[i] / sum;
    // G4cout << "CS " << i << " has ratio "
    //        << charge_state_ratio_[i] << G4endl;
  }
}

G4double K37PrimaryGeneratorAction::GetChargeStateThisEvent() {
  G4double charge_state = -2.0, sum = 0.0;
  unsigned c = 0;
  G4double guess = CLHEP::RandFlat::shoot(0.0, 1.0);

  while (charge_state < 0 && c < charge_state_ratio_.size()) {
    sum += charge_state_ratio_[c];
    // G4cout << "Comparing to " << sum << "...";
    if (guess < sum) {
      charge_state = c * eplus;
      // G4cout << " passed!";
    }
    c++;
    // G4cout << G4endl;
  }
  // G4cout << "Random is " << guess << " --> charge state is +" << charge_state
  //        << "\n" << G4endl;
  return charge_state;
}