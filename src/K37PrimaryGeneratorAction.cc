// Authors: Spencer Behling and Benjamin Fenker 2013

#include <iomanip>
#include <cmath>

#include "K37PrimaryGeneratorAction.hh"
#include "K37DetectorConstruction.hh"
#include "K37PrimaryGeneratorMessenger.hh"
#include "K37AllPossibleEventInformation.hh"
#include "K37CloudSize.hh"
#include "K37EventGenerator.hh"

#include "G4Event.hh"
#include "G4SingleParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"

#include "globals.hh"
#include "Randomize.hh"

K37PrimaryGeneratorAction::K37PrimaryGeneratorAction(
                           K37DetectorConstruction* det,
                           K37AllPossibleEventInformation* APEI,
                           K37EventGenerator* evGen) :
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
  cloud = new K37CloudSize(0.1, 1);
  particleGun = new G4SingleParticleSource();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  positron = particleTable->FindParticle(particleName="e+");
  electron = particleTable->FindParticle(particleName="e-");
  Ar37MinusParticle = particleTable->FindParticle(particleName="Ar37Minus");

  ion = particleTable->GetIon(18, 37, 0);

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
  bool testingEVGenerator = false;
  evGenerator->MakeEvent();
  if (!testingEVGenerator) {
    cloud->makeEvent();
    EventVertex.set((cloud->xFinal())*mm, (cloud->yFinal())*mm,
                    (cloud->zFinal())*mm);
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

      this->setDaughterVertex();
      anEvent->AddPrimaryVertex(vertex);
      EventInformation->
        setDaughterMomentum(G4ThreeVector(evGenerator->dMomentumX(),
                                          evGenerator->dMomentumY(),
                                          evGenerator->dMomentumZ()));
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
  // G4PrimaryParticle* particle = new G4PrimaryParticle(positron,0,0,0.01 );
  vertex->SetPrimary(particle);
}

void K37PrimaryGeneratorAction::setDaughterVertex() {
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *daughter = particleTable -> FindParticle("Ar37Minus");
  vertex = new G4PrimaryVertex(EventVertex, 0);
  G4PrimaryParticle* particle =
    new G4PrimaryParticle(daughter, evGenerator->dMomentumX(),
                          evGenerator->dMomentumY(), evGenerator->dMomentumZ());
  vertex->SetPrimary(particle);
}

