// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37RunAction_h
#define K37RunAction_h 1

#include <map>
#include <string>
#include <vector>

#include "G4UserRunAction.hh"

#include "Aggregator.hh"
#include "Generic_Channel.hh"
#include "globals.hh"
#include "K37_Data.hh"
#include "K37RunMessenger.hh"
#include "TDC_Channel.hh"

using AGG::Aggregator;
using K37_ABC::K37_Data;
using std::map;
using std::string;

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
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  void SetAccepted() {accepted++;}
  void SetAcceptedWithSili2() {acceptedWithSili2++;}
  void SetAcceptedWithDedx2() {acceptedWithDedx2++;}
  void SetFalsePositiveWithNone() {falsePositiveWithNone++;}
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
  void SetAcceptedWithNone() {acceptedWithNone++;}
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
  void AddAccepted_bs() {accepted_bs++;}
  void Add_bs() {bs++;}
  void SetFalsePositive() {falsePositive++;}
  void SetFalsePositiveWithSili() {falsePositiveWithSili++;}
  void SetFalsePositiveWithDedx() {falsePositiveWithDedx++;}
  void SetFalsePositiveWithBoth() {falsePositiveWithBoth++;}
  void AddNonVetoed() {nonVetoed++;}
  void AddVetoed() {vetoed++;}
  void incrementPlusZ_vc(double vc) {plusZHits_vc += vc;}
  void incrementMinusZ_vc(double vc) {minusZHits_vc += vc;}
  void PrintEnergyLossTable(G4String);
  void PrintResultsToScreen();
  void PrintResultsToRunstat();
  void SetRecordAnnihilationPosition(G4bool _recordAnnihilationPosition) {
    recordAnnihilationPosition = _recordAnnihilationPosition;
  };
  G4bool GetRecordAnnihilationPosition() {return recordAnnihilationPosition;}
  void SetRecordVolumeNames(G4bool _recordVolumeNames) {
    recordVolumeNames= _recordVolumeNames;
  };
  G4bool GetRecordVolumeNames() {return recordVolumeNames;}
  void setFileName(G4String newName) {outFileName = newName;}
  void SetAggregator(Aggregator *the_aggregator) {
    the_aggregator_ = the_aggregator;
  }
  void SetConfigurationFileName(const char *filename) {
    snprintf(configuration_filename_, sizeof(configuration_filename_),
             "%s", filename);
  }
  char* GetConfigurationFileName() {return configuration_filename_;}
  void SetOutputDirectory(const char *directory_name) {
    snprintf(output_directory_, sizeof(output_directory_),
             "%s", directory_name);
  }
  char* GetOutputDirectory() {return output_directory_;}
  void SetActiveChannels(map<string, K37_Data*> *active_channels) {
    active_channels_ = active_channels;
  }

 private:
  void RegisterChannel(K37_Data* channel);
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
  char configuration_filename_[200];
  char output_directory_[200];

  // Event generator testing 1/18/2013
  G4int numPlus, numMinus;
  Aggregator *the_aggregator_;
  Generic_Channel *qdc_upper_pmt_, *qdc_lower_pmt_;
  Generic_Channel *dl_x_pos_, *dl_z_pos_;
  Generic_Channel *strip_detector_upper_x_[40];
  Generic_Channel *strip_detector_upper_y_[40];
  Generic_Channel *strip_detector_lower_x_[40];
  Generic_Channel *strip_detector_lower_y_[40];
  TDC_Channel *tdc_scint_top_, *tdc_scint_bottom_;
  TDC_Channel *tdc_ion_mcp_, *tdc_electron_mcp_;
  Generic_Channel *electron_kinetic_energy_generated_;
  Generic_Channel *electron_mu_generated_;
  Generic_Channel *recoil_mu_generated_;

  map<string, K37_Data*> *active_channels_;
};

//----------------------------------

#endif





