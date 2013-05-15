// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37RunAction_h
#define K37RunAction_h 1

#include "G4UserRunAction.hh"
#include "K37RunMessenger.hh"
#include "globals.hh"

//----------------------------------

class G4Run;
class K37DetectorConstruction;
class K37PrimaryGeneratorAction;
class K37ListOfVolumeNames;
// class K37RunMessenger;
class K37AnnihilationPosition;
class K37AllPossibleEventInformation;
class K37HistogramManager;

class K37RunAction : public G4UserRunAction {
 public:
  K37RunAction(K37DetectorConstruction*, K37PrimaryGeneratorAction*,
               K37ListOfVolumeNames*, K37AnnihilationPosition*,
               K37AllPossibleEventInformation*, K37HistogramManager*);
  ~K37RunAction();

 public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  void SetAccepted() {
    accepted++;
  };
  void SetAcceptedWithSili2() {
    acceptedWithSili2++;
  };
  void SetAcceptedWithDedx2() {
    acceptedWithDedx2++;
  };
  // void SetAcceptedWithDedx2SecondaryFiredDedx() {
  //   acceptedWithDedx2SecondaryFiredDedx++;
  // };
  // void SetAcceptedWithDedx2SecondaryFiredSili() {
  //   acceptedWithDedx2SecondaryFiredSili++;
  // };
  // void SetAcceptedWithDedx2SecondaryFiredDedx2() {
  //   acceptedWithDedx2SecondaryFiredDedx2++;
  // };
  // void SetAcceptedWithDedx2SecondaryFiredSiliAndDedx2() {
  //   acceptedWithDedx2SecondaryFiredSiliAndDedx2++;
  // };
  // void SetAcceptedWithDedx2SecondaryFiredDedxAndSili() {
  //   acceptedWithDedx2SecondaryFiredDedxAndSili++;
  // };
  // void SetAcceptedWithDedx2SecondaryFiredDedxAndDedx2() {
  //   acceptedWithDedx2SecondaryFiredDedxAndDedx2++;
  // };
  // void SetAcceptedWithDedx2PrimaryFiredAll() {
  //   acceptedWithDedx2SPrimaryFiredAll++;
  // };
  // void SetAcceptedWithDedx2SecondaryFiredAll() {
  //   acceptedWithDedx2SecondaryFiredAll++;
  // };
  void SetFalsePositiveWithNone() {
    falsePositiveWithNone++;
  };
  void SetFalsePositiveWithNoneSecondaryFiredDedx() {
    falsePositiveWithNoneSecondaryFiredDedx++;
  };
  void SetFalsePositiveWithNoneSecondaryFiredSili() {
    falsePositiveWithNoneSecondaryFiredSili++;
  };
  void SetFalsePositiveWithNonePrimaryFiredBoth() {
    falsePositiveWithNonePrimaryFiredBoth++;
  };
  void SetFalsePositiveWithNoneSecondaryFiredBoth() {
    falsePositiveWithNoneSecondaryFiredBoth++;
  };

  void SetAcceptedWithNone() {
    acceptedWithNone++;
  };
  void SetAcceptedWithNoneSecondaryFiredDedx() {
    acceptedWithNoneSecondaryFiredDedx++;
  };
  void SetAcceptedWithNoneSecondaryFiredSili() {
    acceptedWithNoneSecondaryFiredSili++;
  };
  void SetAcceptedWithNonePrimaryFiredBoth() {
    acceptedWithNonePrimaryFiredBoth++;
  };
  void SetAcceptedWithNoneSecondaryFiredBoth() {
    acceptedWithNoneSecondaryFiredBoth++;
  };

  void SetAcceptedPrimaryScatteredOffHoops() {
    acceptedPrimaryScatteredOffHoops++;
  };
  void AddAccepted_bs() {
    accepted_bs++;
  };
  void Add_bs() {
    bs++;
  };
  // void SecondaryFiredSiLi         ()  { secondaryInSiLi ++;};
  // void SecondaryFiredDedx         ()  { secondaryInDedx ++;};
  // void SecondaryFiredBoth         ()  { secondaryInBoth ++;};
  void SetFalsePositive() {
    falsePositive++;
  };
  void SetFalsePositiveWithSili() {
    falsePositiveWithSili++;
  };
  void SetFalsePositiveWithDedx() {
    falsePositiveWithDedx++;
  };
  void SetFalsePositiveWithBoth() {
    falsePositiveWithBoth++;
  };
  void AddNonVetoed() {
    nonVetoed++;
  };
  void AddVetoed() {
    vetoed++;
  };
  void incrementPlusZ_vc(double vc) {
    plusZHits_vc += vc;
  };
  void incrementMinusZ_vc(double vc) {
    minusZHits_vc += vc;
  };
  void PrintEnergyLossTable(G4String);
  void PrintResultsToScreen();
  void PrintResultsToRunstat();
  void SetRecordAnnihilationPosition(G4bool _recordAnnihilationPosition) {
    recordAnnihilationPosition = _recordAnnihilationPosition;
  };
  G4bool GetRecordAnnihilationPosition() {
    return recordAnnihilationPosition;
  };
  void SetRecordVolumeNames(G4bool _recordVolumeNames) {
    recordVolumeNames= _recordVolumeNames;
  };
  G4bool GetRecordVolumeNames() {
    return recordVolumeNames;
  };
  void setFileName(G4String newName) {
    outFileName = newName;
  }

 private:
  G4int NbofEvents;

  G4int    accepted;
  G4int    accepted_bs;
  G4int    bs;
  // G4int    secondaryInSiLi;
  // G4int    secondaryInDedx;
  // G4int    secondaryInBoth;
  G4int    falsePositive;
  G4int    falsePositiveWithSili;
  G4int    falsePositiveWithDedx;
  G4int    falsePositiveWithBoth;
  G4int    acceptedWithSili2;

  G4int    acceptedWithDedx2;
  G4int    acceptedPrimaryScatteredOffHoops;

  /*G4int    acceptedWithDedx2SecondaryFiredDedx;
    G4int    acceptedWithDedx2SecondaryFiredSili;
    G4int    acceptedWithDedx2SecondaryFiredDedx2;
    G4int    acceptedWithDedx2SecondaryFiredSiliAndDedx2;
    G4int    acceptedWithDedx2SecondaryFiredDedxAndSili;
    G4int    acceptedWithDedx2SecondaryFiredDedxAndDedx2;
    G4int    acceptedWithDedx2SPrimaryFiredAll;
    G4int    acceptedWithDedx2SecondaryFiredAll;
  */

  G4int    acceptedWithNone;
  G4int    acceptedWithNoneSecondaryFiredDedx;
  G4int    acceptedWithNoneSecondaryFiredSili;
  G4int    acceptedWithNonePrimaryFiredBoth;
  G4int    acceptedWithNoneSecondaryFiredBoth;

  G4int    falsePositiveWithNone;
  G4int    falsePositiveWithNoneSecondaryFiredDedx;
  G4int    falsePositiveWithNoneSecondaryFiredSili;
  G4int    falsePositiveWithNonePrimaryFiredBoth;
  G4int    falsePositiveWithNoneSecondaryFiredBoth;

  G4double plusZHits_vc;
  // Counts in +z scintillator, each weighted by the event-by-event v/c.  When
  // +z hits, gets incremented by v/c for that event
  G4double minusZHits_vc;       // Counts in -z scintillator, weighted by v/c

  G4int    vetoed;
  G4int    nonVetoed;

  K37DetectorConstruction* detector;
  K37PrimaryGeneratorAction* kinematic;
  K37ListOfVolumeNames* listOfEnteredVolumes;
  K37RunMessenger* runMessenger;

  K37AnnihilationPosition* annihilationPosition;
  K37AllPossibleEventInformation* AllEventInformation;
  G4bool recordAnnihilationPosition;
  G4bool recordVolumeNames;
  G4bool recordEventInformation;

  K37HistogramManager * histograms;

  // ROOT output filename
  G4String outFileName;

  // Event generator testing 1/18/2013
  G4int numPlus, numMinus;
};

//----------------------------------

#endif





