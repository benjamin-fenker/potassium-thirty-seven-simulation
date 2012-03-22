
#ifndef K37ContainerForStripInformation_h
#define K37ContainerForStripInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <map>
#include <list>

class K37AllPossibleEventInformation;

class K37ContainerForStripInformation 
{
  public:

    K37ContainerForStripInformation(K37AllPossibleEventInformation*);
   ~K37ContainerForStripInformation();

   void StoreStripInformation(G4ThreeVector, G4double, G4bool, G4bool);
   void BeginingOfEventClear();
   void PrintMaps();
   void PassThePlusZDetectors();
   void PassTheMinusZDetectors();

 private:
   void InitializeDetectorMap();
   K37AllPossibleEventInformation * EventInformation;
   G4int FindStripNumberForHit(G4double);
   std::map<G4int, G4double> mapOfDetectorStrips; 
   std::map<G4int,G4double>::iterator iteratorOfDetectorMap;
   std::multimap<G4int,G4double>::iterator iteratorOfValues;
   std::pair<std::multimap<G4int, G4double>::iterator,std::multimap<G4int, G4double>::iterator> ret;
   std::multimap<G4int, G4double> DetectorPlusZ_X_secondary; 
   std::multimap<G4int, G4double> DetectorPlusZ_Y_secondary; 
   std::multimap<G4int, G4double> DetectorPlusZ_X_beta; 
   std::multimap<G4int, G4double> DetectorPlusZ_Y_beta; 
   std::multimap<G4int, G4double> DetectorMinusZ_X_secondary; 
   std::multimap<G4int, G4double> DetectorMinusZ_Y_secondary; 
   std::multimap<G4int, G4double> DetectorMinusZ_X_beta; 
   std::multimap<G4int, G4double> DetectorMinusZ_Y_beta; 
   std::list<G4int>::iterator listIterator;
   std::list<G4int> listPlusZ_Y_secondary;
   std::list<G4int> listPlusZ_Y_beta;
   std::list<G4int> listMinusZ_Y_secondary;
   std::list<G4int> listMinusZ_Y_beta;
   std::list<G4int> listPlusZ_X_secondary;
   std::list<G4int> listPlusZ_X_beta;
   std::list<G4int> listMinusZ_X_secondary;
   std::list<G4int> listMinusZ_X_beta;

};

#endif
