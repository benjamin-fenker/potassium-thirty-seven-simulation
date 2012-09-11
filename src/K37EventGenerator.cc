#include "K37EventGenerator.hh"
#include "K37FermiFunction.hh"

K37EventGenerator::K37EventGenerator()
    :FF(0)
{
  FF = new K37FermiFunction();
  pi = 3.14159265358979323846;
  electron.Mass = 0.510998;           // MeV/c^2
  parent.Mass = 36.97337589*931.46;     //37K
  daughter.Mass = 36.96677632*931.46;   //37Ar
  neutrino.Mass = 0.0;

  Zhi = 2.69001;
  BigA = -0.573938;
  BigB = -0.779145;
  LittleC = 0.205207;
  LittleA = 0.657989;

  QValue = -1.0 * (daughter.Mass - parent.Mass  + 2. * electron.Mass);
  electron.MaxE = (pow(parent.Mass ,2.0) + pow(electron.Mass,2.0)
                   - pow((neutrino.Mass + daughter.Mass + electron.Mass), 2.0))
                  / (2.0 * parent.Mass);
  neutrino.MaxE = (pow(parent.Mass, 2.0) + pow(neutrino.Mass, 2.0)
                   - pow((electron.Mass + daughter.Mass + electron.Mass), 2.0))
                  /(2.0 * parent.Mass);
  daughter.MaxE = (pow(parent.Mass, 2.0) + pow((daughter.Mass + electron.Mass),
                   2.0)
                   - pow((electron.Mass+neutrino.Mass), 2.0))
                  /(2.0 * parent.Mass );
  electron.MaxT = electron.MaxE - electron.Mass;
  neutrino.MaxT = neutrino.MaxE - neutrino.Mass;
  daughter.MaxT = daughter.MaxE - daughter.Mass - electron.Mass;

  //omegaFile.open ("omegaFile.txt", std::ofstream::out | std::ofstream::trunc);
  //omegaFile.close();
  //omegaFile.open ("omegaFile.txt", std::ofstream::out | std::ofstream::app);
  //this ->MakeEvent();
  v_over_c_running = 0.0;
  running_num = 0.0;
  running_denom = 0.0;
}

K37EventGenerator::~K37EventGenerator()
{
  G4cout << "BF LOOK HERE: total v/c = " << v_over_c_running << G4endl;
  G4cout << "num = " << running_num << " denom: " << running_denom << G4endl;
  delete FF;
}
