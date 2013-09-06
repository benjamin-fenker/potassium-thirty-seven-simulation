// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37EventAction_h
#define K37EventAction_h 1

#include <map>
#include <string>
#include <vector>

#include "globals.hh"
#include "G4THitsMap.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryContainer.hh"
#include "G4UserEventAction.hh"
#include "G4Electron.hh"

#include "Aggregator.hh"
#include "Generic_Channel.hh"
#include "K37EventMessenger.hh"
#include "K37PrimaryGeneratorAction.hh"
#include "K37StripDetectorHit.hh"

using std::map;
using std::string;
using std::vector;
using AGG::Aggregator;
using K37_ABC::K37_Data;

class K37ContainerForStripInformation;
class K37EventMessenger;
class K37RunAction;

class K37EventAction : public G4UserEventAction {
 public:
  K37EventAction(K37RunAction* ra);
  ~K37EventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  void BackscatterFlag() {bs_flag = 1;}
  void setEnteringDedx(G4ThreeVector);
  void setStartingDirection(G4ThreeVector);
  void setScatterOffHoopFlag() {primaryScatteredOffHoops = true;}
  void SetAccepted() {accepted = 0;}
  void PrintEvent(const G4Event*);
  G4double GetAccepted() {return accepted;}
  // Will return beta = v/c for the particle
  G4double GetRelativisticFactor(G4double particleMass, G4double totalE);
  void SetUpperScintillatorThreshold(double t)
  {upper_scintillator_threshold_ = t;}
  void SetLowerScintillatorThreshold(double t)
  {lower_scintillator_threshold_ = t;}
  void SetElectronMCPthreshold(double t) {electron_mcp_threshold_ = t;}
  G4double GetUpperScintillatorThreshold() {
    return upper_scintillator_threshold_;
  }
  G4double GetLowerScintillatorThreshold() {
    return lower_scintillator_threshold_;
  }
  G4double GetElectronMCPthreshold() {return electron_mcp_threshold_;}
  void SetAggregator(Aggregator *aggregator) {the_aggregator_ = aggregator;}
  void SetActiveChannels(map<string, K37_Data*> *active_channels) {
    active_channels_ = active_channels;
  }
  void SetPrimaryGenerator(K37PrimaryGeneratorAction *primary_generator) {
    primary_generator_ = primary_generator;
  }

 private:
  // V1190 TDC Clock beats every 97.65625 ns; output from analyzer is in
  // number of ticks.  See manual pg 13 for details.  I want to output the
  // number of ticks to the ntuple to match the analyzer.  To convert from ns
  // to number of ticks, divide by this number.
  const G4double v1190_factor_ns;

  // G4THitsMap<G4double>* evtMap;

  // ******VERY IMPORTANT**********
  // When using this function, always pass it the x-start
  // for the SD you wnat to fill!!!
  void fillSDNtuples(vector<G4double> EDep_strip, G4int ntuple_number_start);
  void fillSDNtuples(vector<G4double> energy_strip, G4String name);
  // *******************************
  vector<G4double>GetEDepVector(K37StripDetectorHitsCollection *collection,
                                G4int coordinate);
  vector<G4double>GetEDepVectorX(K37StripDetectorHitsCollection *collection);
  vector<G4double>GetEDepVectorY(K37StripDetectorHitsCollection *collection);
  bool EventPassesTrigger(double upper_scintillator_energy,
                          double lower_scintillator_energy,
                          double electron_mcp_energy);
  void LookAtEvent(const G4Event *event);
  //  G4THitsMap<G4double>* energyBTdedxMap;
  //  G4THitsMap<G4double>* energyBTscintillatorMap;
  // G4THitsMap<G4double>* energyMirror2Map;
  // G4THitsMap<G4double>* energyBeryllium2Map;
  // G4THitsMap<G4double>* energyDedx2Map;
  // G4THitsMap<G4double>* energyMountMap;
  // G4THitsMap<G4double>* energyBerylliumMap;
  // G4THitsMap<G4double>* energyFaceMap;

  K37RunAction*  runAct;
  K37ContainerForStripInformation* stripHandler;

  G4int fullenergy1CollID;
  G4int fullenergy2CollID;
  G4int dedx1CollID;
  G4int dedx2CollID;
  //  G4int mirrorCollID;
  G4int recoil_mcp_collection_id;
  G4int electron_mcp_collection_id;
  //  G4int BTdedxID;
  //  G4int BTscintillatorID;

  //  G4int Mirror2ID;
  //  G4int Beryllium2ID;
  //  G4int Dedx2ID;
  //  G4int SiLi2ID;
  //  G4int MountID;
  //  G4int BerylliumID;
  //  G4int FaceID;

  G4Electron *theElectron;
  G4double emass;
  G4double energyDedx;
  G4double energyDedx_Primaries;
  G4double energyDedx_Secondaries;
  G4double energyDedx2;
  G4double energyDedx2_Primaries;
  G4double energyDedx2_Secondaries;

  //  G4double energyBTdedx;
  //  G4double energyBTscintillator;

  //  G4double energyMirror2;
  //  G4double energyBeryllium2;
  // G4double energyDedx2;
  // G4double energyMount;
  //  G4double energyBeryllium;
  //  G4double energyFace;

  //  G4double energyfront;
  //  G4double energyback;

  G4int accepted;
  G4int interesting;
  G4int bs_flag;

  G4bool isThereEnergySiliPrimary;
  G4bool isThereEnergySiliSecondary;
  G4bool isThereEnergySili;

  G4bool isThereEnergySili2Primary;
  G4bool isThereEnergySili2Secondary;
  G4bool isThereEnergySili2;

  G4bool isThereEnergyDedxPrimary;
  G4bool isThereEnergyDedxSecondary;
  G4bool isThereEnergyDedx;

  G4bool isThereEnergyDedx2Primary;
  G4bool isThereEnergyDedx2Secondary;
  G4bool isThereEnergyDedx2;

  G4bool VetoedEvent;
  G4bool primaryScatteredOffHoops;
  // G4bool scatterOffHoopFlag;

  G4double X;
  G4double Y;
  G4double Z;
  G4double Radius;

  //  G4double theta;
  //  G4double thetaFromStart;
  //  G4double difference;
  //  G4double cosDifference;
  //  G4double thetafromfunction;
  //  G4double cosfromfunction_old;
  //  G4double cosfromfunction_new;
  //  G4double cosfromthetaFromStart;

  vector< G4ThreeVector > spot;
  vector< G4ThreeVector > start;

  unsigned int sizeOfListOfEnteredVolumes;
  //  int differenceInListSizeFromStartToEnd;

  double upper_scintillator_threshold_;
  double lower_scintillator_threshold_;
  double electron_mcp_threshold_;

  K37EventMessenger *event_messenger_;
  Aggregator *the_aggregator_;
  map<string, K37_Data*> *active_channels_;

  //  G4double svn_version_number;
  K37PrimaryGeneratorAction *primary_generator_;
};

#endif


