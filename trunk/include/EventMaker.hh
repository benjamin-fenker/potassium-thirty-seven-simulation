#ifndef EventMaker_h
#define EventMaker_h 1

#include "globals.hh"
//#include <fstream>

struct particle
{
	G4double MaxE;			//Max Total Energy
	G4double MaxT;			//Max Kinetic Energy
	G4double Mass;			//Mass of particle
	G4double E;				//Total Energy
	G4double T;				//Kinetic Energy
	G4double Pmag;			//Magnitude of Momentum
	G4double PmagSquared;	//Magnitude of Momentum Squared
	G4double Theata;		//Theata of particle with respect to the polarization axis 
	G4double Phi;			//0 to 2pi around the polarization axis
	G4double unitX;			//unit vector of momentum in X 
	G4double unitY;			//unit vector of momentum in Y 
	G4double unitZ;			//unit vector of momentum in Z coordinate
	G4double X;				//X component of momentum
	G4double Y;				//Y component of momentum
	G4double Z;				//Z component of momentum
	
};

class FermiFunction;

class EventMaker
{
public:
	EventMaker();
	~EventMaker();

public:
    void MakeEvent();
	
	G4double eMomentumX()		{return electron.X;};
	G4double eMomentumY()		{return electron.Y;};
	G4double eMomentumZ()		{return electron.Z;};
	G4double eMomentumTheta()	{return electron.Theata;};
	G4double eMomentumPhi()		{return electron.Phi;};
	G4double eMomentumPmag()	{return electron.Pmag;};
	G4double eKineticEnergy()	{return electron.T;};
	
	
private:
	
	particle electron;
	particle parent;
	particle daughter;
	particle neutrino;
	
	G4double pi;
	G4double QValue;
	G4double cosBetaNeutrinoAngle;
	G4double eDotJ;
	G4double nDotJ;
	G4double eDotn;
	
	
	G4double Omega;
	G4double testOmega;
	G4double Zhi;
	G4double BigA;
	G4double BigB;
	G4double LittleC;
	G4double LittleA;
	FermiFunction* FF;	
	//std::ofstream omegaFile;
	
};

#endif
