

#ifndef K37DetectorMessenger_h
#define K37DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class K37DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

//-----------------------------

class K37DetectorMessenger: public G4UImessenger
{
  public:
    K37DetectorMessenger(K37DetectorConstruction*);
   ~K37DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    K37DetectorConstruction* myDetector;
    
    G4UIdirectory*	geometryDir;
    G4UIcmdWithAString*	ChangeMirrorMaterial;    
    G4UIcmdWithABool*   ShouldTheMirrorBeWFEDM;    
    G4UIcmdWithoutParameter* UpdateGeometryCommand;    
    G4UIcmdWithoutParameter* PrintAvailableMaterials;    
};

//-----------------------------

#endif

