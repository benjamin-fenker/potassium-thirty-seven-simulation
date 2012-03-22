#include "JTW_Event.hh"
#include "K37FermiFunction.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <cmath>

void JTW_Event::MakeEvent()
{
	while(true)
	{
		testOmega = 3.*G4UniformRand();
		
		while(true)
		{
			while(true)
			{
				electron.T = electron.MaxT*G4UniformRand();
				electron.E = electron.T + electron.Mass ;
				electron.PmagSquared = (electron.T+electron.Mass)*(electron.T+electron.Mass) - electron.Mass*electron.Mass;
				electron.Pmag = sqrt(electron.PmagSquared);
				electron.Theata = acos (1.0 - 2.0*G4UniformRand());		
				electron.Phi = 2.0*pi*G4UniformRand();
			
				//x = r*sin(Theata)*cos(Phi)
				//y = r*sin(Theata)*sin(Phi)
				//z = r*cos(Theata)
			
				electron.unitX = sin(electron.Theata)*cos(electron.Phi);
				electron.unitY = sin(electron.Theata)*sin(electron.Phi);
				electron.unitZ = cos(electron.Theata);
			
				electron.X = electron.Pmag*electron.unitX;
				electron.Y = electron.Pmag*electron.unitY;
				electron.Z = electron.Pmag*electron.unitZ;
			
			
				neutrino.Theata = acos (1.0 - 2.0*G4UniformRand());
				neutrino.Phi = 2.0*pi*G4UniformRand();
				neutrino.unitX = sin(neutrino.Theata)*cos(neutrino.Phi);
				neutrino.unitY = sin(neutrino.Theata)*sin(neutrino.Phi);
				neutrino.unitZ = cos(neutrino.Theata);
			
				//V dot W = mag (V)*mag(W) cos(alpha)
				//where alpha is the angle between them for unit vectors this reduces to alpha = acos(V dot W) and cos(alpha) = V dot W
			
				cosBetaNeutrinoAngle =  electron.unitX*neutrino.unitX + electron.unitY*neutrino.unitY + electron.unitZ*neutrino.unitZ; 
			
				neutrino.E =  parent.Mass*(electron.MaxE - electron.E)/(parent.Mass - electron.E + electron.Pmag * cosBetaNeutrinoAngle);
			
				if(neutrino.E < neutrino.MaxE)
				{
					break;
				}
				
				continue;
			}
		
			neutrino.T = neutrino.E;
			neutrino.Pmag = neutrino.E;
		
			neutrino.X = neutrino.Pmag*neutrino.unitX;
			neutrino.Y = neutrino.Pmag*neutrino.unitY;
			neutrino.Z = neutrino.Pmag*neutrino.unitZ;
		
			neutrino.PmagSquared = neutrino.Pmag*neutrino.Pmag;
		
			daughter.X = -1.0*(electron.X + neutrino.X) ; 
			daughter.Y = -1.0*(electron.Y + neutrino.Y) ; 
			daughter.Z = -1.0*(electron.Z + neutrino.Z) ; 
		
			daughter.Pmag = sqrt(daughter.X*daughter.X + daughter.Y*daughter.Y + daughter.Z*daughter.Z);
		
			daughter.unitX = daughter.X/daughter.Pmag;
			daughter.unitY = daughter.Y/daughter.Pmag;
			daughter.unitZ = daughter.Z/daughter.Pmag;
		
			daughter.E = sqrt(daughter.Pmag*daughter.Pmag + daughter.Mass*daughter.Mass);
			//daughter.T =  daughter.E-daughter.Mass-electron.Mass;
			daughter.T =  daughter.E-daughter.Mass;
		
			daughter.Theata =  acos(daughter.Z/daughter.Pmag);
			daughter.Phi =  atan2(daughter.Y,daughter.X);
			if(daughter.Phi < 0.0)
			{
				daughter.Phi = 2.0*pi + daughter.Phi ;
			}
		
		
			if(daughter.E < daughter.MaxE)
			{
			
				break;
			}
		
			continue;
		}
	
	
		// since j is (0,0,1) eMomentum dot j is just eMomentumZ
	 
		eDotJ = electron.Z;
		nDotJ = neutrino.Z;
	
		eDotn = electron.X*neutrino.X + electron.Y*neutrino.Y + electron.Z*neutrino.Z;
	
		//(1.0/(pow((2.0*pi),5.0)))
		//(1.0/(4.0*pow(pi,3.0)))
			 
		Omega = (1.0/(4.0*pow(pi,3.0)))
		*(FF->getVFF(electron.T))
		*electron.Pmag
		*electron.E
		*pow(( electron.MaxE - electron.E), 2.0)
		*Zhi*(1.0 
			  + LittleA*(eDotn)/(electron.E*neutrino.E) 
			  - LittleC*(eDotn)/(3.0*electron.E*neutrino.E)
			  +LittleC*eDotJ*nDotJ/(electron.E*neutrino.E) 
			  + BigA*eDotJ/electron.E 
			  + BigB*nDotJ/neutrino.E );
		
		//omegaFile<< Omega << G4endl;

		if(Omega >= testOmega)
		{
			break;
		}
		
		continue;
	}
}
