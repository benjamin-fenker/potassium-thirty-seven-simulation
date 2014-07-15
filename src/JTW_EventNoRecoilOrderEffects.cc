// Authors: Spencer Behling
//This is a hack to add in a two percent branch

#include <cmath>
#include "K37Analysis.hh"
#include "JTW_EventNoRecoilOrderEffects.hh"
#include "K37FermiFunction.hh"
#include "K37AnalysisNumbering.hh"
#include "globals.hh"
#include "Randomize.hh"

//extern G4bool fillEvGenData;

void JTW_EventNoRecoilOrderEffects::MakeEvent() {
  MakeEvent(1.0, 1.0, 1.0);
}

void JTW_EventNoRecoilOrderEffects::MakeEvent(G4double polarization, G4double alignment,
                          G4double recoil_charge) {
  // Set cuts on production for testing the EV Generator
  G4double minElectronT = 0.0*electron.MaxT;  // MeV --> beta_min = 0.995
  // radius 20 mm 98.5 mm from trap center
  // electron.Theta = M_PI/2.0;
  electron.T = -10.0;
  while (electron.T < minElectronT) {
    while (true) {
      testOmega = 3.*G4UniformRand();
      // testOmega = 30.0*G4UniformRand(); // What's the right value here!?
      while (true) {
        while (true) {
          // For faster testing:
          // electron.T = 0.1*electron.MaxT;
          // electron.T -=  (electron.MaxT - minElectronT) * G4UniformRand();
          electron.T = electron.MaxT*G4UniformRand();  // MeV
          electron.E = electron.T + electron.Mass;     // MeV
          electron.PmagSquared = (electron.T+electron.Mass)*
            (electron.T+electron.Mass) -
            electron.Mass*electron.Mass;  // MeV/c with c = 1
          electron.Pmag = sqrt(electron.PmagSquared);

          G4double mu;    // cos(electron.Theta)
          // Picks mu from -1 to 1 uniformly
          //          mu = 1.0 - 2.0*G4UniformRand();
          // Picks mu uniformly from abs(mu) = minCosTheta to 1
          mu = 1.0 - (1.0-min_cos_theta_)*G4UniformRand();
          if (G4UniformRand() > 0.5) mu *= -1.0;
          // mu = 1.0;

          electron.Theta = acos(mu);
          electron.Phi = 2.0*M_PI*G4UniformRand();

          // x = r*sin(Theta)*cos(Phi)
          // y = r*sin(Theta)*sin(Phi)
          // z = r*cos(Theta)

          electron.unitX = sin(electron.Theta)*cos(electron.Phi);
          electron.unitY = sin(electron.Theta)*sin(electron.Phi);
          electron.unitZ = cos(electron.Theta);

          electron.X = electron.Pmag*electron.unitX;
          electron.Y = electron.Pmag*electron.unitY;
          electron.Z = electron.Pmag*electron.unitZ;

          neutrino.Theta = acos(1.0 - 2.0*G4UniformRand());
          neutrino.Phi = 2.0*M_PI*G4UniformRand();
          neutrino.unitX = sin(neutrino.Theta)*cos(neutrino.Phi);
          neutrino.unitY = sin(neutrino.Theta)*sin(neutrino.Phi);
          neutrino.unitZ = cos(neutrino.Theta);

          // V dot W = mag (V)*mag(W) cos(alpha)
          // where alpha is the angle between them for unit vectors this reduces
          // to alpha = acos(V dot W) and cos(alpha) = V dot W

          cosBetaNeutrinoAngle =  electron.unitX*neutrino.unitX +
            electron.unitY*neutrino.unitY + electron.unitZ*neutrino.unitZ;

          neutrino.E =  parent.Mass*(electron.MaxE - electron.E)/
            (parent.Mass - electron.E + electron.Pmag * cosBetaNeutrinoAngle);

          if (neutrino.E < neutrino.MaxE) {
            // G4cout << "Broke out neutrino" << G4endl;
            break;
          }

          continue;
        }  // Innermost while(true)

        neutrino.T = neutrino.E;        // Massless neutrino
        neutrino.Pmag = neutrino.E;

        neutrino.X = neutrino.Pmag*neutrino.unitX;
        neutrino.Y = neutrino.Pmag*neutrino.unitY;
        neutrino.Z = neutrino.Pmag*neutrino.unitZ;

        neutrino.PmagSquared = neutrino.Pmag*neutrino.Pmag;

        daughter.X = -1.0*(electron.X + neutrino.X);
        daughter.Y = -1.0*(electron.Y + neutrino.Y);
        daughter.Z = -1.0*(electron.Z + neutrino.Z);

        daughter.Pmag = sqrt(daughter.X*daughter.X + daughter.Y*daughter.Y +
                             daughter.Z*daughter.Z);

        daughter.unitX = daughter.X/daughter.Pmag;
        daughter.unitY = daughter.Y/daughter.Pmag;
        daughter.unitZ = daughter.Z/daughter.Pmag;

        daughter.E = sqrt(daughter.Pmag*daughter.Pmag +
                          daughter.Mass*daughter.Mass);
        // daughter.T =  daughter.E-daughter.Mass-electron.Mass;
        daughter.T =  daughter.E-daughter.Mass;

        daughter.Theta =  acos(daughter.Z/daughter.Pmag);
        daughter.Phi =  atan2(daughter.Y, daughter.X);
        if (daughter.Phi < 0.0) {
          daughter.Phi = 2.0*M_PI + daughter.Phi;
        }
        if (daughter.E < daughter.MaxE) {
          // G4cout << "broke out (daughter)" << G4endl;
          break;
        }
        continue;
      }  // Middle while(true)
      // since j is (0,0,1) eMomentum dot j is just eMomentumZ

      eDotJ = electron.Z;
      nDotJ = neutrino.Z;

      eDotn = electron.X*neutrino.X + electron.Y*neutrino.Y +
        electron.Z*neutrino.Z;

      Omega = (1.0/(4.0*pow(M_PI, 3.0)))
        *(FF->getVFF(electron.T))
        *electron.Pmag
        *electron.E
        *pow((electron.MaxE - electron.E), 2.0)
        *Zhi*(1.0
              + LittleA*(eDotn)/(electron.E*neutrino.E)
              - alignment*LittleC*(eDotn)/(3.0*electron.E*neutrino.E)
              + alignment*LittleC*eDotJ*nDotJ/(electron.E*neutrino.E)
              + polarization*BigA*eDotJ/electron.E
              + polarization*BigB*nDotJ/neutrino.E);
      if (Omega >= testOmega) {
        // G4cout << "Broke out (omega)! " << G4endl;
        break;
      }
      continue;
    }  // Outermost while(true)
  }
  //  G4cout << cos(electron.Theta) << "\t" << electron.T << G4endl;
  // Calculate the velocity of the electron in the lab from.
  // E = gamma*m*c^2; gamma = (1-v^2/c^2)^(-1/2) --> v = c*sqrt(1-(m^2c^4/E^2)
  double gamma = electron.E/electron.Mass;
  double v_over_c = sqrt(1.0 - pow(gamma, -2.0));

  // G4cout << "T = " << electron.T << "\t-->gamma = ";
  // G4cout << (electron.T/electron.Mass)+1.0 << "\t E = " << electron.E;
  // G4cout << "\t--> gamma = " << electron.E/electron.Mass << "\tbeta = ";
  // G4cout << v_over_c << G4endl;
  // G4cout << "Used in calculation: " << eDotJ/electron.E << G4endl;

  // Fill up histogram to check that event generator makes sense
  running_denom += fabs(v_over_c*cos(electron.Theta));
  if (cos(electron.Theta) > 0.0) {
    numPlus++;
  } else {
    numMins++;
  }
  //if (fillEvGenData) {
    //(*active_channels_)["T_GEN_ELE"] -> InsertData(electron.T);
    //(*active_channels_)["MU_GEN_ELE"] -> InsertData(cos(electron.Theta));
    //(*active_channels_)["MU_GEN_RECOIL"] -> InsertData(cos(daughter.Theta));
    //(*active_channels_)["ION_CHARGE"] -> InsertData(recoil_charge);
    ////    the_aggregator_ -> EndEvent()
  //}
}     // End makeEvent

void JTW_EventNoRecoilOrderEffects::ResetGeneratedCounters() {
  numPlus = 0.0;
  numMins = 0.0;
}

G4int JTW_EventNoRecoilOrderEffects::GetNumMins() {
  return numMins;
}

G4int JTW_EventNoRecoilOrderEffects::GetNumPlus() {
  return numPlus;
}



