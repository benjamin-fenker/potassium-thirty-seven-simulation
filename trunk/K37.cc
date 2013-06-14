// Authors: Spencer Behling and Benjamin Fenker 2013

#include <stdio.h>
#include <time.h>

#include <iomanip>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4UnitsTable.hh"
#include "G4TrajectoryDrawByParticleID.hh"

#include "K37HistogramManager.hh"

#include "K37EventGenerator.hh"
#include "Holstein_Event.hh"
#include "JTW_Event.hh"
#include "K37FermiFunction.hh"
#include "K37ListOfVolumeNames.hh"
#include "K37DetectorConstruction.hh"
#include "K37PhysicsList.hh"
#include "K37PrimaryGeneratorAction.hh"
#include "K37RunAction.hh"
#include "K37EventAction.hh"
#include "K37TrackingAction.hh"
#include "K37SteppingAction.hh"
#include "K37SteppingVerbose.hh"
#include "K37AnnihilationPosition.hh"
#include "K37AllPossibleEventInformation.hh"
#include "K37ElectricFieldSetup.hh"
#include "K37StackingAction.hh"
#include "K37Config.hh"
#include "Aggregator.hh"
#include "Generic_Channel.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

using std::ofstream;

int main(int argc, char** argv) {
   // G4cout << "=================================" << G4endl;
   // G4cout << "This is K37 revesion " << K37_VERSION << G4endl;
   // G4cout << "=================================" << G4endl;
   // G4bool shouldwe = 0;
   // G4bool nowewont = 0;
   // G4bool yeswewill = 0;
   // G4cin>> shouldwe>> nowewont>> yeswewill;

   K37AnnihilationPosition* annihilation =
      new K37AnnihilationPosition("annihilate.txt", 1000);

   K37AllPossibleEventInformation* APEI =
      new K37AllPossibleEventInformation("EventInformation.txt");

   //-------------------------------------------------------------
   //This is a minimally working exapmple of how the aggregator 
   //works. The order of operations is important
   // AGG::Aggregator *theAggregator= new AGG::Aggregator();
   // Generic_Channel *testChan  = new Generic_Channel("theTest", 1, "/D");
   // theAggregator->RegisterData(testChan);
   // theAggregator->RegisterIOMethod("IOconfiguration.mac");
   // theAggregator->BeginRun();
   // testChan->InsertData(10.0);
   // theAggregator->EndEvent();
   // testChan->InsertData(11.0);
   // theAggregator->EndEvent();
   // testChan->InsertData(13.0);
   // theAggregator->EndEvent();
   // testChan->InsertData(15.0);
   // theAggregator->EndEvent();
   // theAggregator->EndRun();
   // delete theAggregator; theAggregator = 0;
   // delete testChan; testChan = 0;
   //-------------------------------------------------------------

   K37HistogramManager* histo = new K37HistogramManager();

   CLHEP::Ranlux64Engine randomEngine;
   CLHEP::HepRandom::setTheEngine(&randomEngine);

   time_t seconds;
   seconds = time(NULL);
   G4long myseed = seconds;
   CLHEP::HepRandom::setTheSeed(myseed);

   ofstream runstat;
   runstat.open("runstat.txt", ofstream::out | ofstream::trunc);
   runstat << "Random seed: " << myseed << G4endl;
   runstat << "-----------------------" << G4endl;
   runstat.close();

   G4VSteppingVerbose* verbosity = new K37SteppingVerbose;
   G4VSteppingVerbose::SetInstance(verbosity);

   // Run manager
   G4RunManager* runManager = new G4RunManager();

   // User Initialization classes (mandatory)
   K37ElectricFieldSetup* field = new K37ElectricFieldSetup();

   K37DetectorConstruction* detector;
   runManager->SetUserInitialization(detector = new K37DetectorConstruction());

   G4VUserPhysicsList* physics = new K37PhysicsList();
   runManager->SetUserInitialization(physics);

   runManager->Initialize();

   // User Action classes
   K37EventGenerator * evGen = new JTW_Event();

   K37PrimaryGeneratorAction* gen_action;
   runManager->SetUserAction(gen_action = new K37PrimaryGeneratorAction(
            detector,
            APEI,
            evGen));

   K37ListOfVolumeNames *volumesTheBetaEntered =
      new K37ListOfVolumeNames("volumeNames.txt", 1000);

   K37RunAction* run_action;
   runManager->SetUserAction(run_action = new K37RunAction(detector,
            gen_action,
            volumesTheBetaEntered,
            annihilation, APEI,
            histo));

   K37EventAction* event_action;
   runManager->SetUserAction(event_action =
         new K37EventAction(run_action, volumesTheBetaEntered, APEI, histo));

   K37TrackingAction* tracking_action;
   runManager->SetUserAction(tracking_action= new K37TrackingAction());

   K37SteppingAction* stepping_action;
   runManager->SetUserAction(stepping_action =
         new K37SteppingAction(event_action,
            volumesTheBetaEntered,
            annihilation, APEI));

   // get the pointer to the User Interface manager
   runManager->SetUserAction(new K37StackingAction);

   G4int result = static_cast<G4int>(system("rm -rf detectors.root"));
   if (result != 0) G4cout << "Could not remove detectors.root." << G4endl;

#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive();
   visManager->Initialize();
   G4TrajectoryDrawByParticleID* model = new G4TrajectoryDrawByParticleID();
   model->SetDefault("cyan");
   model->Set("gamma", "green");
   model->Set("e+", "blue");
   model->Set("Ar37Minus", G4Colour(0.8, 0.1, 0.8));
   model->Set("Ar37Neutral", "cyan");
   model->Set("e-", "red");
   visManager->RegisterModel(model);
   visManager->SelectTrajectoryModel(model->Name());
#endif

   // Get the pointer to the User Interface manager
   //
   G4UImanager * UImanager = G4UImanager::GetUIpointer();

   if (argc != 1) {  // batch mode
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
   } else {       // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
      if (ui->IsGUI()) {
         UImanager->ApplyCommand("/control/execute gui.mac");
      }
      ui->SessionStart();
      delete ui;
#endif

#ifdef G4VIS_USE

      if (visManager) {
         delete visManager;
      }
      // if(model){delete model;}
      // delete visManager;
#endif
   }

   /*
      G4UImanager* UI = G4UImanager::GetUIpointer();



      if (argc!=1)   // batch mode
      {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
      }

      else           // interactive mode : define visualization and UI terminal
      {
#ifdef G4VIS_USE
G4VisManager* visManager = new G4VisExecutive;
visManager->Initialize();
#endif

G4UIsession * session = 0;

#ifdef G4UI_USE_TCSH
session = new G4UIterminal(new G4UItcsh);
#else
session = new G4UIterminal();
#endif

#ifdef G4VIS_USE
UI->ApplyCommand("/control/execute vis.mac");
#endif

session->SessionStart();
delete session;

#ifdef G4VIS_USE
delete visManager;
#endif
}
*/

// Job termination
// Free the store: user actions, physics_list and detector_description are
//                 owned and deleted by the run manager, so they should not
//                 be deleted in the main() program !
if (field)       {
   delete field;
}
if (runManager)  {
   delete runManager;
}
if (verbosity)   {
   delete verbosity;
}
if (annihilation) {
   delete annihilation;
}
if (APEI)        {
   delete APEI;
}
if (evGen)       {
   delete evGen;
}
if (volumesTheBetaEntered)       {
   delete volumesTheBetaEntered;
}
if (histo)       {
   delete histo;
}
// if(visManager)        {delete visManager;}
// delete betaGenerator;
return 0;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
