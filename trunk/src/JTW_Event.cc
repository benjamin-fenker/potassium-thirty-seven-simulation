// Authors: Spencer Behling and Benjamin Fenker 2013

#include <cmath>
#include "K37HistogramManager.hh"
#include "K37Analysis.hh"
#include "JTW_Event.hh"
#include "K37FermiFunction.hh"
#include "K37AnalysisNumbering.hh"
#include "globals.hh"
#include "Randomize.hh"

void JTW_Event::MakeEvent() {
  // Set cuts on production for testing the EV Generator
  G4double minCosTheta = -1.0;
  G4double minElectronT = 0.0;  // MeV --> beta_min = 0.995
  // radius 20 mm 98.5 mm from trap center
  electron.Theta = M_PI/2.0;
  electron.T = 0.0;
  //  while((fabs(cos(electron.Theta)) < minCosTheta)) {
  while ((electron.T < minElectronT) ||
        (fabs(cos(electron.Theta)) < minCosTheta)) {
    while (true) {
      testOmega = 3.*G4UniformRand();
      while (true) {
        while (true) {
          // For faster testing:
          electron.T = electron.MaxT;
          electron.T -=  (electron.MaxT - minElectronT) * G4UniformRand();
          //  electron.T = electron.MaxT*G4UniformRand(); // MeV
          electron.E = electron.T + electron.Mass;   // MeV
          electron.PmagSquared = (electron.T+electron.Mass)*
            (electron.T+electron.Mass) -
            electron.Mass*electron.Mass;  // MeV/c with c = 1
          electron.Pmag = sqrt(electron.PmagSquared);

          G4double mu;    // cos(electron.Theta)
          // Picks mu from -1 to 1 uniformly
          // mu = 1.0 - 2.0*G4UniformRand();
          // Picks mu uniformly from abs(mu) = minCosTheta to 1
          mu = 1.0 - (1.0-minCosTheta)*G4UniformRand();
          if (G4UniformRand() > 0.5) mu *= -1.0;
          electron.Theta = acos(mu);
          //                electron.Theta = acos (1.0 - 2.0*G4UniformRand());
          electron.Phi = 2.0*pi*G4UniformRand();

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
          neutrino.Phi = 2.0*pi*G4UniformRand();
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
            break;
          }

          continue;
        }  // Innermost while(true)

        neutrino.T = neutrino.E;
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
          daughter.Phi = 2.0*pi + daughter.Phi;
        }
        if (daughter.E < daughter.MaxE) {
          break;
        }
        continue;
      }  // Middle while(true)
      // since j is (0,0,1) eMomentum dot j is just eMomentumZ

      eDotJ = electron.Z;
      nDotJ = neutrino.Z;

      eDotn = electron.X*neutrino.X + electron.Y*neutrino.Y +
        electron.Z*neutrino.Z;

      // (1.0/(pow((2.0*pi),5.0)))
      // (1.0/(4.0*pow(pi,3.0)))
      Omega = (1.0/(4.0*pow(pi, 3.0)))
        *(FF->getVFF(electron.T))
        *electron.Pmag
        *electron.E
        *pow((electron.MaxE - electron.E), 2.0)
        *Zhi*(1.0
              + LittleA*(eDotn)/(electron.E*neutrino.E)
              - LittleC*(eDotn)/(3.0*electron.E*neutrino.E)
              + LittleC*eDotJ*nDotJ/(electron.E*neutrino.E)
              + BigA*eDotJ/electron.E
              + BigB*nDotJ/neutrino.E);

      // omegaFile<< Omega << G4endl;

      if (Omega >= testOmega) {
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

  // Fill up histogram to check that event generator makes sense
  G4AnalysisManager *anMan = G4AnalysisManager::Instance();
  running_denom += fabs(v_over_c*cos(electron.Theta));
  anMan -> FillH1(hist_e_angle_to_pol_generated, cos(electron.Theta));
  anMan -> FillH1(hist_e_energy_generated, electron.E);
  anMan -> FillH1(hist_v_over_c_generated, v_over_c);
  anMan -> FillNtupleDColumn(ntup_v_over_c_generated, v_over_c);
  anMan -> FillNtupleDColumn(ntup_theta_e_generated, cos(electron.Theta));
  // G4cout << "mu_generated = " << cos(electron.Theta) << G4endl;
  anMan -> AddNtupleRow();
}     // End makeEvent
