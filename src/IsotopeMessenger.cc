// Authors: Spencer Behling 2014

#include "IsotopeMessenger.hh"

// ----------------------------------

IsotopeMessenger::IsotopeMessenger(ISO::Isotope *iso_)
:isotope(iso_)
{

  isotope_dir = new G4UIdirectory("/K37/isotope/");
  isotope_dir->SetGuidance("Control Isotope Properties That Affect The Decay");

  set_rho = new G4UIcmdWithADouble("/K37/isotope/SetRho", this);
  set_rho -> SetGuidance("Change the Rho Value for the Decay.");
  set_rho -> SetParameterName("Polarization", true);
  set_rho -> SetDefaultValue(0.576468);

}

// ----------------------------------

IsotopeMessenger::~IsotopeMessenger()
{
  delete isotope_dir;
  delete set_rho;
}

// ----------------------------------

void IsotopeMessenger::SetNewValue(G4UIcommand* command,
      G4String newValue)
{
   if (command == set_rho)
   {
      isotope -> ChangeRho(set_rho->GetNewDoubleValue(newValue));
   }
}

// ----------------------------------

