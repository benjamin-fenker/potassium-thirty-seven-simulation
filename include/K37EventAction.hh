// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37EventAction_h
#define K37EventAction_h 1

#include <vector>

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include "G4THitsMap.hh"

//#include "K37EventMessenger.hh"
#include "K37StripDetectorHit.hh"

using std::vector;

class K37AllPossibleEventInformation;
class K37ContainerForStripInformation;
//class K37EventMessenger;
class K37HistogramManager;
class K37ListOfVolumeNames;
class K37RunAction;

class K37EventAction : public G4UserEventAction {
 public:
  K37EventAction(K37RunAction* ra, K37ListOfVolumeNames* lovn,
                 K37AllPossibleEventInformation* apei, K37HistogramManager* hm);
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

 private:
  // G4THitsMap<G4double>* evtMap;

  // ******VERY IMPORTANT**********
  // When using this function, always pass it the x-start
  // for the SD you wnat to fill!!!
  void fillSDNtuples(vector<G4double> EDep_strip, G4int ntuple_number_start);
  // *******************************
  vector<G4double>GetEDepVector(K37StripDetectorHitsCollection *collection,
                                G4int coordinate);
  vector<G4double>GetEDepVectorX(K37StripDetectorHitsCollection *collection);
  vector<G4double>GetEDepVectorY(K37StripDetectorHitsCollection *collection);
  bool EventPassesTrigger(double upper_scintillator_energy,
                          double lower_scintillator_energy,
                          double electron_mcp_energy);

  G4THitsMap<G4double>* energyBTdedxMap;
  G4THitsMap<G4double>* energyBTscintillatorMap;
  // G4THitsMap<G4double>* energyMirror2Map;
  // G4THitsMap<G4double>* energyBeryllium2Map;
  // G4THitsMap<G4double>* energyDedx2Map;
  // G4THitsMap<G4double>* energyMountMap;
  // G4THitsMap<G4double>* energyBerylliumMap;
  // G4THitsMap<G4double>* energyFaceMap;

  K37RunAction*  runAct;
  K37ListOfVolumeNames* listOfEnteredVolumes;
  K37ContainerForStripInformation* stripHandler;

  G4int fullenergy1CollID;
  G4int fullenergy2CollID;
  G4int dedx1CollID;
  G4int dedx2CollID;
  G4int mirrorCollID;
  G4int recoil_mcp_collection_id;
  G4int electron_mcp_collection_id;
  G4int BTdedxID;
  G4int BTscintillatorID;

  G4int Mirror2ID;
  G4int Beryllium2ID;
  G4int Dedx2ID;
  G4int SiLi2ID;
  G4int MountID;
  G4int BerylliumID;
  G4int FaceID;

  G4double energyDedx;
  G4double energyDedx_Primaries;
  G4double energyDedx_Secondaries;
  G4double energyDedx2;
  G4double energyDedx2_Primaries;
  G4double energyDedx2_Secondaries;

  G4double energyBTdedx;
  G4double energyBTscintillator;

  G4double energyMirror2;
  G4double energyBeryllium2;
  // G4double energyDedx2;
  G4double energyMount;
  G4double energyBeryllium;
  G4double energyFace;

  G4double energyfront;
  G4double energyback;

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

  G4double theta;
  G4double thetaFromStart;
  G4double difference;
  G4double cosDifference;
  G4double thetafromfunction;
  G4double cosfromfunction_old;
  G4double cosfromfunction_new;
  G4double cosfromthetaFromStart;

  vector< G4ThreeVector > spot;
  vector< G4ThreeVector > start;

  unsigned int sizeOfListOfEnteredVolumes;
  int differenceInListSizeFromStartToEnd;
  K37AllPossibleEventInformation* EventInformation;
  K37HistogramManager * histograms;

  double upper_scintillator_threshold_;
  double lower_scintillator_threshold_;
  double electron_mcp_threshold_;

  //K37EventMessenger *event_messenger_;
};

#endif

