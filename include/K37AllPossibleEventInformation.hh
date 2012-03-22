

#ifndef K37AllPossibleEventInformation_H
#define K37AllPossibleEventInformation_H 1

#include "globals.hh"
#include "K37AllPossibleEventInformation.hh"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "G4ThreeVector.hh"


class K37AllPossibleEventInformation
{

public:
	K37AllPossibleEventInformation( std::string _filename);
	~K37AllPossibleEventInformation();

	void printEventInformation();
	void clearEventInformation();
	void touchFile();	
	void deleteFile();	
	G4bool getShouldEventInformationBeRecorded(){return shouldEventInformationBeRecorded;};	
	void setShouldEventInformationBeRecorded(){ shouldEventInformationBeRecorded= true;};	
	//G4double get(){return;};	
	//void set(G4double _){=_;};	
	//G4double get(){return;};	
	//void set(G4double _){=_;};	
	G4double getOriginalTheta(){return originalTheta;};	
	void setOriginalTheta(G4double _originalTheta){ originalTheta=_originalTheta;};	

	G4bool getIsTheVertexSet(){return IsTheVertexSet;};	
	void setIsTheVertexSetToTrue(){ IsTheVertexSet= true;};	

	G4bool getGammaFiredStripDetectorPlusZ(){return gammaFiredStripDetectorPlusZ;};	
	void setGammaFiredStripDetectorPlusZToTrue(){ gammaFiredStripDetectorPlusZ= true;};	

	G4bool getGammaFiredStripDetectorMinusZ(){return gammaFiredStripDetectorMinusZ;};	
	void setGammaFiredStripDetectorMinusZToTrue(){ gammaFiredStripDetectorMinusZ= true;};	

	G4bool getGammaFiredScintillatorMinusZ(){return gammaFiredScintillatorMinusZ;};	
	void setGammaFiredScintillatorMinusZToTrue(){ gammaFiredScintillatorMinusZ= true;};	

	G4bool getGammaFiredScintillatorPlusZ(){return gammaFiredScintillatorPlusZ;};	
	void setGammaFiredScintillatorPlusZToTrue(){ gammaFiredScintillatorPlusZ= true;};	

	G4bool getPlusZDetectorsFired(){return PlusZDetectorsFired;};	
	void setPlusZDetectorsFiredToTrue(){ PlusZDetectorsFired= true;};	

	G4ThreeVector getVertexPosition(){return vertexPosition;};	
	void setVertexPosition(G4ThreeVector _vertexPosition){ vertexPosition= _vertexPosition;};	

	G4ThreeVector getDaughterMomentum(){return daughterMomentum;};	
	void setDaughterMomentum(G4ThreeVector _daughterMomentum){ daughterMomentum= _daughterMomentum;};	
	G4double getTotalEnergyInScintillator(){return totalEnergyInScintillator;};	
	void setTotalEnergyInScintillator(G4double _totalEnergyInScintillator){ totalEnergyInScintillator= _totalEnergyInScintillator;};	

	G4double getTotalEnergyInStripDetector(){return totalEnergyInStripDetector;};	
	void setTotalEnergyInStripDetector(G4double _totalEnergyInStripDetector){ totalEnergyInStripDetector=_totalEnergyInStripDetector;};	

	G4double getMetaStableTimeOfDeath(){return metaStableTimeOfDeath;};	
	void setMetaStableTimeOfDeath(G4double _metaStableTimeOfDeath){ metaStableTimeOfDeath= _metaStableTimeOfDeath;};	

	G4double getStartStripDetectorTime(){return stripDetectorStartTime;};	
	void setStartStripDetectorTime(G4double _stripDetectorStartTime){ stripDetectorStartTime= _stripDetectorStartTime;};	

	G4bool getIsThisAnAcceptedEvent(){return acceptedEvent;};	
	void setThisIsAnAccepterEvent(){ acceptedEvent= true;};	
	void setEnteringStripDetector(G4int, G4ThreeVector);
	void setEnteringShakeOffElectronDetector(G4ThreeVector);
	void setEnteringShakeOffElectronDetectorTime(G4double);
	void setEnteringScintillator(G4int, G4ThreeVector);
	void setMinusZStrip_X_beta(G4int,G4double);
	void setMinusZStrip_Y_beta(G4int,G4double);
	void setMinusZStrip_X_gamma(G4int,G4double);
	void setMinusZStrip_Y_gamma(G4int,G4double);
	void setPlusZStrip_X_beta(G4int,G4double);
	void setPlusZStrip_Y_beta(G4int,G4double);
	void setPlusZStrip_X_gamma(G4int,G4double);
	void setPlusZStrip_Y_gamma(G4int,G4double);
	void EndOfEventActions ();
  
private:  
	std::ofstream fileOfVolumeEventInformation;
	G4bool shouldEventInformationBeRecorded;
	G4bool acceptedEvent;

	//This is what I came up with as what we would want for each event
	std::string filename;
	G4bool doubleHitInfinitePrecision;
	G4bool gammaFiredStripDetectorPlusZ;
	G4bool gammaFiredScintillatorPlusZ;
	G4bool gammaFiredStripDetectorMinusZ;
	G4bool gammaFiredScintillatorMinusZ;
	G4bool doubleHitDifferentStrip;
	G4bool PlusZDetectorsFired; //value of 1 means that it is the plus Z telescope fired 0 is minus z
	G4bool IsTheVertexSet; //value of 1 means that it is set. 
	std::map<G4int, G4double> stripEnergyFromBeta_X_MinusZ;
	std::map<G4int, G4double> stripEnergyFromGamma_X_MinusZ;
	std::map<G4int, G4double> stripEnergyFromBeta_Y_MinusZ;
	std::map<G4int, G4double> stripEnergyFromGamma_Y_MinusZ;
	std::map<G4int, G4double> stripEnergyFromBeta_X_PlusZ;
	std::map<G4int, G4double> stripEnergyFromGamma_X_PlusZ;
	std::map<G4int, G4double> stripEnergyFromBeta_Y_PlusZ;
	std::map<G4int, G4double> stripEnergyFromGamma_Y_PlusZ;
	std::vector< std::string > listOfVolumeNames;
	//std::vector <G4ThreeVector> enteringStripDetector;
	std::vector <G4ThreeVector> enteringShakeOffElectronDetector;
	std::vector <G4double> enteringShakeOffElectronDetectorTime;
	std::multimap<G4int,G4ThreeVector> enteringStripDetector;
	std::multimap<G4int,G4ThreeVector> enteringScintillator;
	//std::vector <G4ThreeVector> enteringScintillator;
	G4double originalTheta;
	G4double stripDetectorStartTime;
	G4ThreeVector vertexPosition;
	G4ThreeVector daughterMomentum;
	G4double measuredTheta;
	G4double energyFromBetaInScintillator;
	G4double energyFromGammaInScintillator;
	G4double totalEnergyInScintillator;//This is done.
	G4double totalEnergyInStripDetector;//This is done.
	G4double annihilationPositionX;
	G4double annihilationPositionY;
	G4double annihilationPositionZ;
	G4double metaStableTimeOfDeath;
};

#endif

