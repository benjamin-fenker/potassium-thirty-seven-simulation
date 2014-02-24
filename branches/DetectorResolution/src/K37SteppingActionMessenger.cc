#include "K37SteppingActionMessenger.hh"

K37SteppingActionMessenger::K37SteppingActionMessenger(
      K37SteppingAction *stepping_action_)
   :stepping_action(stepping_action_)
{
      main_directory = new G4UIdirectory("/K37/");
      main_directory -> SetGuidance(
            "This is where all of the K37 specific commands live.");

      stepping_directory = new G4UIdirectory("/K37/SteppingControl/");
      stepping_directory -> SetGuidance(
            "Stepping parameters such as volume name tracking.");

      set_track_volumes =
         new G4UIcmdWithABool("/K37/SteppingControl/SetTrackVolumes", this);
      set_track_volumes->
         SetGuidance("If set to true then a list of volume entered ");
      set_track_volumes->
         SetGuidance("by the e+ will be kept track of for each event.");
      set_track_volumes->
         SetGuidance("Enabling this command should result in a large ");
      set_track_volumes->
         SetGuidance("performance penalty.");
      set_track_volumes -> SetParameterName("Should_I_Track", true);
      set_track_volumes -> SetDefaultValue(false);
}

K37SteppingActionMessenger::~K37SteppingActionMessenger()
{
   delete main_directory;
   delete stepping_directory;
   delete set_track_volumes;
}

void K37SteppingActionMessenger::SetNewValue(G4UIcommand *command,
      G4String new_values)
{
   if (command == set_track_volumes)
   {
      stepping_action ->
         TrackNames(set_track_volumes -> GetNewBoolValue(new_values));
   }
   else
   {
      G4cout << "Unknown command in event messenger" << G4endl;
   }
}
