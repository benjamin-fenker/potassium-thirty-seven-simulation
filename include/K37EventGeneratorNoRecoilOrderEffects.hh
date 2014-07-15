// Authors: Spencer Behling
// The point of this class is to hack in a quick 2% branch. It is 
// following the venerable software practice of cut and paste programming.

#ifndef K37EventGeneratorNoRecoilOrderEffects_H
#define K37EventGeneratorNoRecoilOrderEffects_H 1

#include <map>
#include <string>

#include "globals.hh"

#include "Aggregator.hh"
#include "K37_Data.hh"
#include "K37EventGenerator.hh"

using AGG::Aggregator;
using K37_ABC::K37_Data;
using std::map;
using std::string;

class K37FermiFunction;
//struct particle {
  //G4double MaxE;             // Max Total Energy
  //G4double MaxT;             // Max Kinetic Energy
  //G4double Mass;             // Mass of particle
  //G4double E;                // Total Energy
  //G4double T;                // Kinetic Energy
  //G4double Pmag;             // Magnitude of Momentum
  //G4double PmagSquared;      // Magnitude of Momentum Squared
  //G4double Theta;           // Theta of particle with respect to the
  //// polarization axis
  //G4double Phi;              // 0 to 2pi around the polarization axis
  //G4double unitX;            // unit vector of momentum in X
  //G4double unitY;            // unit vector of momentum in Y
  //G4double unitZ;            // unit vector of momentum in Z coordinate
  //G4double X;                // X component of momentum
  //G4double Y;                // Y component of momentum
  //G4double Z;                // Z component of momentum
//};

class K37EventGeneratorNoRecoilOrderEffects {
 protected:
  /*int width, height;*/
  particle electron, parent, daughter, neutrino;
  G4double QValue, cosBetaNeutrinoAngle, eDotJ, nDotJ, eDotn;
  G4double Omega, testOmega, Zhi, BigA, BigB, LittleC, LittleA;
  K37FermiFunction* FF;
  G4double v_over_c_running;    // Will keep a running tally of v/c coming out
  Aggregator *the_aggregator_;
  map<string, K37_Data*> *active_channels_;
  // of event generator for testing purposes
  // Used to only simulate events in a cone around the detectors.
  // Angular distributions are accurate within this cone
  G4double min_cos_theta_;

 public:
  G4double running_num;
  G4double running_denom;
  K37EventGeneratorNoRecoilOrderEffects();
  virtual ~K37EventGeneratorNoRecoilOrderEffects();
  virtual void MakeEvent()=0;
  virtual void MakeEvent(G4double polarization, G4double alignment,
                         G4double recoil_charge)  = 0;
  /*void set_values (int a, int b)*/
  /*{ width=a; height=b; }*/
  /*virtual int area (void) =0;*/
  /*void printarea (void)*/
  /*{ cout << this->area() << endl; }*/
  G4double eMomentumX() {return electron.X;}
  G4double eMomentumY() {return electron.Y;}
  G4double eMomentumZ() {return electron.Z;}
  G4double eMomentumTheta() {return electron.Theta;}
  G4double eMomentumPhi() {return electron.Phi;}
  G4double eMomentumPmag() {return electron.Pmag;}
  G4double eKineticEnergy() {return electron.T;}
  G4double dMomentumX() {return daughter.X;}
  G4double dMomentumY() {return daughter.Y;}
  G4double dMomentumZ() {return daughter.Z;}
  G4double dMomentumTheta() {return daughter.Theta;}
  G4double dMomentumPhi() {return daughter.Phi;}
  G4double dMomentumPmag() {return daughter.Pmag;}
  G4double dKineticEnergy() {return daughter.T;}
  void SetAggregator(Aggregator *aggregator) {the_aggregator_ = aggregator;}
  void SetActiveChannels(map<string, K37_Data*> *active_channels) {
    active_channels_ = active_channels;
  }
  void SetBigA(G4double BigA_);
  void SetMinCosTheta(G4double mct);
  void SetConeHalfAngle(G4double angle);
};

#endif
