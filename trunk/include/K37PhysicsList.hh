// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37PhysicsList_hh
#define K37PhysicsList_hh 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//----------------------------------------------------------------------------
class G4Decay;
class G4StepLimiter;

class K37PhysicsList: public G4VUserPhysicsList {
 public:
  K37PhysicsList();
  ~K37PhysicsList();

 protected:
  // Construct particle and physics
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

  // these methods Construct physics processes and register them
  void ConstructEM();
  void AddStepMax();
  void ConstructDecay();
 private:
  G4Decay* theDecayProcess;
  G4StepLimiter* stepLimiter;
};

//----------------------------------------------------------------------------

#endif


