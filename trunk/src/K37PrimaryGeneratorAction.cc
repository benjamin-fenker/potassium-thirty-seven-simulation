// Authors: Spencer Behling and Benjamin Fenker 2013

#include <iomanip>
#include <cmath>

#include "K37PrimaryGeneratorAction.hh"
#include "K37DetectorConstruction.hh"
#include "K37PrimaryGeneratorMessenger.hh"
#include "K37CloudSize.hh"
#include "K37EventGenerator.hh"
#include "K37EventGeneratorNoRecoilOrderEffects.hh"

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
#include "G4SingleParticleSource.hh"
#include "G4GeneralParticleSource.hh"


K37PrimaryGeneratorAction::K37PrimaryGeneratorAction(
      K37DetectorConstruction* det,
      K37EventGenerator* evGen,
      K37EventGeneratorNoRecoilOrderEffects* twoP)
   : //Initalizer List
    use_gps(false),
      polarization_(1.0),
      alignment_(1.0),
      charge_state_ratio_(8, 1.0/8.0),
      branching_ratio(2, 1.0/2.0),
      detector(det),
      v(),
      vertex(NULL),
      EventVertex(),
      K37Neutral(NULL),
      K37Minus(NULL),
      decayTableAr37Minus(NULL),
      K37MinusDecayMode(NULL),
      cloud(NULL),
      evGenerator(evGen),
      twoPercent(twoP),
      make_beta_(true),
      make_recoil_(true),
      make_shakeoff_electrons_(true),
      makeTwoPercent(true),
      thisEventIsATwoPercent(false)
{

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

   branching_ratio[0] = 0.9789; //Main branch we care about
   branching_ratio[1] = 0.0207; //Two percent branch.
   NormalizeBranchingRatio();

   cloud = new K37CloudSize(G4ThreeVector(1.07*mm, 1.07*mm, -2.05*mm),  // center
         G4ThreeVector(0.00029*kelvin, 0.00029*kelvin,
            0.0014*kelvin),          // temperature
         G4ThreeVector(0.51*mm, 0.51*mm, 0.64*mm),    // width
         G4ThreeVector(0.0 *mm/ns, 0.0*mm/ns, 0.0*mm/ns));
   if (use_gps) {
     particleGun = new G4GeneralParticleSource();
   } else {
     particleGun = new G4SingleParticleSource();
   }
   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   G4String particleName;
   // ion = particleTable->GetIon(18, 37, 0);
   positron = particleTable->FindParticle(particleName="e+");
   electron = particleTable->FindParticle(particleName="e-");
   gamma = particleTable->FindParticle(particleName="gamma");
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
  if (use_gps) {
    particleGun -> GeneratePrimaryVertex(anEvent);
    return;
  }

  G4double recoil_charge_this_event;

  if(makeTwoPercent)
  {
     thisEventIsATwoPercent = TwoPercentEvent();
  }

  if(thisEventIsATwoPercent)
  {
     (*active_channels_)["TWO_PERCENT_BRANCH"] ->  InsertData(1.0);
  }
  else
  {
     (*active_channels_)["TWO_PERCENT_BRANCH"] ->  InsertData(0.0);
  }

  if (recoil_charge_ == -2) {
    recoil_charge_this_event = GetChargeStateThisEvent();
  } else {
    recoil_charge_this_event = recoil_charge_ * eplus;
  }
  (*active_channels_)["ION_CHARGE"] ->  InsertData(recoil_charge_this_event);

  // Get cloud position
  cloud -> makeEvent();
  EventVertex = cloud -> GetFinalPosition();

  // Set initial position of all particles where the cloud tells you
  particleGun->SetParticlePosition(EventVertex);
  //  vertex = new G4PrimaryVertex(EventVertex, 0);

  if (recoil_charge_ == -3) {            // photoions
    SetPhotoionizationVertices(anEvent);
  } else {                              // beta-decay
    // Setup initial momenta of B-decay particles
    if(thisEventIsATwoPercent)
    {
       twoPercent -> MakeEvent(polarization_, alignment_,
             recoil_charge_this_event);
    }
    else
    {
       evGenerator -> MakeEvent(polarization_, alignment_,
             recoil_charge_this_event);
    }

    rho = 1;

    if (make_beta_) {
      this->setBetaVertex();
      anEvent->AddPrimaryVertex(vertex);
    }
    if (make_recoil_) {
      this->setDaughterVertex(recoil_charge_this_event);
      anEvent->AddPrimaryVertex(vertex);
    }
    if (make_shakeoff_electrons_) {
      SetSOelectronVertices(anEvent, recoil_charge_this_event + 1);
    }
    if (thisEventIsATwoPercent) {
      SetGammaVertex(anEvent);
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
  G4PrimaryParticle* particle = 0;
  if(thisEventIsATwoPercent)
  {
     particle = new G4PrimaryParticle(positron, twoPercent->eMomentumX(),
           twoPercent->eMomentumY(), twoPercent->eMomentumZ());
  }
  else
  {
     particle = new G4PrimaryParticle(positron, evGenerator->eMomentumX(),
           evGenerator->eMomentumY(), evGenerator->eMomentumZ());
  }
  vertex->SetPrimary(particle);
}

void K37PrimaryGeneratorAction::SetPhotoionizationVertices(G4Event *ev) {
  vertex = new G4PrimaryVertex(EventVertex, 0);
  G4double e_kinetic_energy = 0.8 * eV;
  //  G4double e_kinetic_energy = 0.0000001 * eV;
  // G4ThreeVector momentum = GetMomentumIsotropic(e_kinetic_energy,
  //                                               electron_mass_c2);
  G4ThreeVector e_momentum = G4RandomDirection();
  (*active_channels_)["MU_GEN_ELE"] -> InsertData(e_momentum.cosTheta());
  //  G4cout << "Px = " << e_momentum.getX() << G4endl;
  G4PrimaryParticle *photoelectron = new G4PrimaryParticle(electron);
  photoelectron -> SetMomentumDirection(e_momentum);
  photoelectron -> SetKineticEnergy(e_kinetic_energy);

  vertex -> SetPrimary(photoelectron);
  ev -> AddPrimaryVertex(vertex);

  vertex = new G4PrimaryVertex(EventVertex, 0);
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *ion = particleTable -> GetIon(19, 37, 0);  // 37K
  G4ThreeVector r_momentum = e_momentum;
  r_momentum *= -1.0;
  (*active_channels_)["MU_GEN_RECOIL"] -> InsertData(r_momentum.cosTheta());
  G4double mass_ratio = electron_mass_c2 / ion -> GetPDGMass();
  //  G4cout << "Ratio = " <<  mass_ratio << G4endl;
  G4double ion_energy = e_kinetic_energy * mass_ratio;
  //  G4cout << "Ion energy = " << ion_energy / keV << " keV " << G4endl;
  //  G4cout << "Px = " << r_momentum.getX() << G4endl;
  G4PrimaryParticle *photoion = new G4PrimaryParticle(ion);
  photoion -> SetMomentumDirection(r_momentum);
  photoion -> SetKineticEnergy(ion_energy);
  photoion -> SetCharge(1.0 * eplus);

  //  G4cout << "Px = " << r_momentum.getX() << G4endl;

  vertex -> SetPrimary(photoion);
  ev -> AddPrimaryVertex(vertex);
}

void K37PrimaryGeneratorAction::SetGammaVertex(G4Event *ev)
{
  vertex = new G4PrimaryVertex(EventVertex, 0);
  G4double gamma_energy = 2.796 * MeV;
  G4ThreeVector gamma_direction = G4RandomDirection(); //isotropic
  G4PrimaryParticle *twoPercentGamma = new G4PrimaryParticle(gamma);
  twoPercentGamma -> SetMomentumDirection(gamma_direction);
  twoPercentGamma -> SetKineticEnergy(gamma_energy);

  vertex -> SetPrimary(twoPercentGamma);
  ev -> AddPrimaryVertex(vertex);
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
  G4PrimaryParticle* particle = 0;
  if(thisEventIsATwoPercent)
  {
     particle = new G4PrimaryParticle(ion,twoPercent->dMomentumX(),
           twoPercent->dMomentumY(),
           twoPercent->dMomentumZ());
  }
  else
  {
     particle = new G4PrimaryParticle(ion, evGenerator->dMomentumX(),
           evGenerator->dMomentumY(),
           evGenerator->dMomentumZ());
  }
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

G4ThreeVector K37PrimaryGeneratorAction::GetMomentumIsotropic(
    G4double kinetic_energy, G4double mass) {
  G4bool debug = false;
  G4double total_energy = kinetic_energy + mass;
  G4double momentum = sqrt((total_energy*total_energy) - (mass * mass));
  G4double mu = 1.0 - 2.0*G4UniformRand();
  G4double theta = acos(mu);
  G4double phi = 2.0*M_PI*G4UniformRand();
  G4double px, py, pz;
  px = momentum * sin(theta) * cos(phi);
  py = momentum * sin(theta) * sin(phi);
  pz = momentum * cos(theta);
  if (debug) {
    G4cout << "\tT = " << G4BestUnit(kinetic_energy, "Energy") << "\tE = "
           << G4BestUnit(total_energy, "Energy") << "\tP = "
           << momentum << G4endl;
    G4cout << "\ttheta = " << theta << "\tphi = " << phi << G4endl;
    G4cout << "\tpx = " << px << "\tpy = " << py << "\tpz = " << pz << G4endl;
  }
  return G4ThreeVector(px, py, pz);
}

void K37PrimaryGeneratorAction::SetSOelectronVertices(G4Event *ev,
                                                      G4int num_so_electrons) {
  for (G4int i = 0; i < num_so_electrons; i++) {
    G4double kinetic_energy = -1.0;
    // Ar binding energy is 15.7 eV and SOE have around this energy
    // The 5.0 eV width is a total guess
    while (kinetic_energy < 0.0) {
      kinetic_energy = CLHEP::RandGauss::shoot(100.0*eV, 5.0*eV);
    }
    G4ThreeVector momentum =
        GetMomentumIsotropic(kinetic_energy, electron_mass_c2);

    G4PrimaryParticle *particle = new G4PrimaryParticle(electron,
                                                        momentum.getX(),
                                                        momentum.getY(),
                                                        momentum.getZ());
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
  if (ali <= 1.0 && ali >= 0.0) {
    alignment_ = ali;
  } else if (ali < 0.0 && ali >= -1.0) {
    ali = fabs(ali);
    G4cout << "WARNING: Alignment must be positive (for both polarization states)";
    G4cout << G4endl;
    G4cout << "Note that this sign convention is opposite that in JTW 1957 and that";
    G4cout << G4endl;
    G4cout << "this change is fully documented in the \"JTWEvent.cc\" source file";
    G4cout << G4endl;
    G4cout << "*********************************************************" << G4endl;
    G4cout << "   Setting alignment to T = +" << ali << G4endl;
    G4cout << "*********************************************************" << G4endl;
    alignment_ = ali;
  } else {
    G4cout << "WARNING: Alignment " << ali
           << " not in allowed range.  No changes made." << G4endl;
  }
}

void K37PrimaryGeneratorAction::SetRecoilCharge(G4int charge) {
  if (charge < 0 && charge != -2 && charge != -3) {
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

void K37PrimaryGeneratorAction::NormalizeBranchingRatio()
{
  G4double sum = 0.0;
  for (unsigned int i = 0; i < branching_ratio.size(); i++)
  {
    sum += branching_ratio[i];
  }
  for (unsigned int i = 0; i < branching_ratio.size(); i++)
  {
    branching_ratio[i] = branching_ratio[i] / sum;
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

G4bool K37PrimaryGeneratorAction::TwoPercentEvent()
{
   G4double guess = CLHEP::RandFlat::shoot(0.0, 1.0);

   if (guess < branching_ratio[1])
   {
      return true;
   }
   else
   {
      return false;
   }
}

void K37PrimaryGeneratorAction::ToggleUseGPS() {
  use_gps = !(use_gps);
  delete particleGun;
  if (use_gps) {
    particleGun = new G4GeneralParticleSource();
  } else {
    particleGun = new G4SingleParticleSource();
  }
}

void K37PrimaryGeneratorAction::SetUseGPS(bool g) {
  if (g == use_gps) {
  } else {
    ToggleUseGPS();
  }
  return;  
}
