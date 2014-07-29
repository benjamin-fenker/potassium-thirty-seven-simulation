// Authors: Spencer Behling 2014

#ifndef IsotopeMessenger_h
#define IsotopeMessenger_h 1

#include "G4UIcmdWithADouble.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

#include "Isotope.hh"

//----------------------------------

class IsotopeMessenger: public G4UImessenger {
 public:
  explicit IsotopeMessenger(ISO::Isotope* iso_);
  virtual  ~IsotopeMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  ISO::Isotope            *isotope;
  G4UIdirectory      *isotope_dir;
  G4UIcmdWithADouble *set_rho;
};

//----------------------------------

#endif

