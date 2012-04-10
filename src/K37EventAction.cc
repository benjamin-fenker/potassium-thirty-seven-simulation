

#include "K37HistogramManager.hh"
#include "K37EventAction.hh"
#include "K37ScintillatorHit.hh"
//#include "K37ScintillatorMinusZHit.hh"
#include "K37StripDetectorHit.hh"
//#include "K37StripDetectorMinusZHit.hh"
#include "K37MirrorHit.hh"
#include "K37RunAction.hh"
#include "K37ListOfVolumeNames.hh"
#include "K37AllPossibleEventInformation.hh"
#include "K37ContainerForStripInformation.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include <math.h>

#define PI 3.14159265

//#include <vector>
using namespace std;



K37EventAction::K37EventAction(K37RunAction* run, K37ListOfVolumeNames* list, K37AllPossibleEventInformation* APEI, K37HistogramManager * his)
:runAct(run), listOfEnteredVolumes(list), stripHandler(0), EventInformation(APEI), histograms(his)
{
	
	stripHandler = new K37ContainerForStripInformation(EventInformation);

	spot.clear();
	start.clear();
	
	fullenergy1CollID = -1;
	fullenergy2CollID = -1;
	dedx1CollID = -1;
	dedx2CollID = -1;
	//mirrorCollID = -1;
	//Mount2ID = -1;
	
	//BTdedxID= -1;
	//BTscintillatorID= -1;
	
	energySiLi=0;
	energySiLi_Primaries=0;
	energySiLi_Secondaries=0;
	
	energySiLi2=0;
	energySiLi2_Primaries=0;
	energySiLi2_Secondaries=0;
	
	energyDedx=0;
	energyDedx_Primaries=0;
	energyDedx_Secondaries=0;
	
	energyDedx2=0;
	energyDedx2_Primaries=0;
	energyDedx2_Secondaries=0;
	
	//energyMount2=0;
	//energyBTdedx=0;
	//energyBTscintillator=0;
	
	//energyMirror=0;
	
	accepted=0;
	interesting=0;
	bs_flag=0;
	
	X=0;
	Y=0;
	Z=0;
	Radius=0;

	isThereEnergySiliPrimary=false;
	isThereEnergySiliSecondary=false;
	isThereEnergySili=false;
	isThereEnergySili2Primary=false;
	isThereEnergySili2Secondary=false;
	isThereEnergySili2=false;
	
	isThereEnergyDedxPrimary=false;
	isThereEnergyDedxSecondary=false;
	isThereEnergyDedx=false;
	isThereEnergyDedx2Primary=false;
	isThereEnergyDedx2Secondary=false;
	isThereEnergyDedx2=false;
	
	VetoedEvent = false;
	primaryScatteredOffHoops = false;
	
	//vector< double > spot;
	
}

K37EventAction::~K37EventAction()
{
	delete stripHandler;
}

void K37EventAction::BeginOfEventAction(const G4Event*)
{
	//spot.clear();
if(listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded())
{	
	sizeOfListOfEnteredVolumes = listOfEnteredVolumes->checkSizeOfList();
}	
	G4SDManager * SDman = G4SDManager::GetSDMpointer();
		//if(fullenergy1CollID<0||dedx1CollID<0 ||mirrorCollID<0)
			if(fullenergy1CollID<0||dedx1CollID<0 )
			{
				G4String colNam;
				fullenergy1CollID = SDman->GetCollectionID(colNam="scintillatorPlusZHC");
				fullenergy2CollID = SDman->GetCollectionID(colNam="scintillatorMinusZHC");
				dedx1CollID = SDman->GetCollectionID(colNam="dsssdPlusZHC");
				dedx2CollID = SDman->GetCollectionID(colNam="dsssdMinusZHC");
				//mirrorCollID = SDman->GetCollectionID(colNam="mirrorCollection");
				//BTdedxID = SDman->GetCollectionID(colNam="BTdedx/eDepBTdedx");
				//BTscintillatorID = SDman->GetCollectionID(colNam="BTScintillator/eDepBTScintillator");				
			}

	energySiLi=0;
	energySiLi_Primaries=0;
	energySiLi_Secondaries=0;
	
	energySiLi2=0;
	energySiLi2_Primaries=0;
	energySiLi2_Secondaries=0;
	
	energyDedx=0;
	energyDedx_Primaries=0;
	energyDedx_Secondaries=0;
	
	energyDedx2=0;
	energyDedx2_Primaries=0;
	energyDedx2_Secondaries=0;
	
	//energyMirror=0;
	
	//energyMount2=0;
	//energyBTdedx=0;
	//energyBTscintillator=0;
	
	interesting=0;
	bs_flag=0;
	isThereEnergySiliPrimary=false;
	isThereEnergySiliSecondary=false;
	isThereEnergySili=false;
	isThereEnergySili2Primary=false;
	isThereEnergySili2Secondary=false;
	isThereEnergySili2=false;
	
	isThereEnergyDedxPrimary=false;
	isThereEnergyDedxSecondary=false;
	isThereEnergyDedx=false;
  	isThereEnergyDedx2Primary=false;
	isThereEnergyDedx2Secondary=false;
	isThereEnergyDedx2=false;
	
	primaryScatteredOffHoops = false;
	
	VetoedEvent = false;
	X=0;
	Y=0;
	Z=0;
	Radius=0;
	stripHandler->BeginingOfEventClear();
	
}

void K37EventAction::EndOfEventAction(const G4Event* evt)
{
 // 
if(listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded())
{	
	differenceInListSizeFromStartToEnd = int (listOfEnteredVolumes->checkSizeOfList() - sizeOfListOfEnteredVolumes);
}
	//G4cout << "Size of List" <<differenceInListSizeFromStartToEnd<< G4endl;

	
	if(bs_flag>0)
		{
			runAct->Add_bs();
		}
	
	//if(fullenergy1CollID<0||dedx1CollID<0||mirrorCollID<0) return;
	if(fullenergy1CollID<0||dedx1CollID<0) return;
	
	G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
	K37StripDetectorHitsCollection* DEDX1HC = 0;
	K37StripDetectorHitsCollection* DEDX2HC = 0;
	K37ScintillatorHitsCollection* FE1HC = 0;
	K37ScintillatorHitsCollection* FE2HC = 0;
	
	//K37MirrorHitsCollection* MHC = 0;

	//energyMount2Map = 0;
	//energyBTdedxMap=0;
	//energyBTscintillatorMap=0;
	
	if(HCE)
	{
		FE1HC = (K37ScintillatorHitsCollection*)(HCE->GetHC(fullenergy1CollID));
		FE2HC = (K37ScintillatorHitsCollection*)(HCE->GetHC(fullenergy2CollID));
		DEDX1HC = (K37StripDetectorHitsCollection*)(HCE->GetHC(dedx1CollID));
		DEDX2HC = (K37StripDetectorHitsCollection*)(HCE->GetHC(dedx2CollID));
		//MHC = (K37MirrorHitsCollection*)(HCE->GetHC(mirrorCollID)); 
		
		//energyMount2Map = (G4THitsMap<G4double>*)(HCE->GetHC(Mount2ID));
		//energyBTdedxMap = (G4THitsMap<G4double>*)(HCE->GetHC(BTdedxID));
		//energyBTscintillatorMap = (G4THitsMap<G4double>*)(HCE->GetHC(BTscintillatorID));

		
	}

	if(FE1HC)
		{
			int n_hit = FE1HC->entries();
			for(int i=0;i<n_hit;i++)
			{ 
				energySiLi += (*FE1HC)[i]->GetEdep();
				if((*FE1HC)[i]->GetPrimary())
				{
					energySiLi_Primaries +=(*FE1HC)[i]->GetEdep();
				}
				else
				{
					 energySiLi_Secondaries+=(*FE1HC)[i]->GetEdep();
				}
	
		
			}

			if(energySiLi>0)
			{
				isThereEnergySili = true;
				if(energySiLi_Primaries==0)
				{
					EventInformation->setGammaFiredScintillatorPlusZToTrue();
				}	
				//G4cout<< "energySiLi: " << energySiLi << G4endl;

			}
		}
	
	if(FE2HC)
	{
		int n_hit = FE2HC->entries();
		for(int i=0;i<n_hit;i++)
		{ 
			energySiLi2 += (*FE2HC)[i]->GetEdep();
			if((*FE2HC)[i]->GetPrimary())
			{
				energySiLi2_Primaries +=(*FE2HC)[i]->GetEdep();
			}
			else
			{
				energySiLi2_Secondaries+=(*FE2HC)[i]->GetEdep();
			}


		}

		if(energySiLi2>0)
		{
			isThereEnergySili2 = true;
			if(energySiLi2_Primaries==0)
			{
				EventInformation->setGammaFiredScintillatorMinusZToTrue();
			}	

			//G4cout<< "energySiLi2: " << energySiLi2 << G4endl;
		}

	}
	
	if(DEDX2HC)
	{
		int n_hit = DEDX2HC->entries();
		for(int i=0;i<n_hit;i++)
		{ 
			energyDedx2 += (*DEDX2HC)[i]->GetEdep();
			if((*DEDX2HC)[i]->GetPrimary())
			{
				energyDedx2_Primaries +=(*DEDX2HC)[i]->GetEdep();
				stripHandler->StoreStripInformation((*DEDX2HC)[i]->GetPos2(),(*DEDX2HC)[i]->GetEdep(),true,false);
			}
			else
			{
				energyDedx2_Secondaries +=(*DEDX2HC)[i]->GetEdep();
				stripHandler->StoreStripInformation((*DEDX2HC)[i]->GetPos2(),(*DEDX2HC)[i]->GetEdep(),false,false);
			}

			
		}

		if(energyDedx2>0)
		{
			isThereEnergyDedx2 = true;

			if(energyDedx2_Primaries ==0)
			{
				EventInformation->setGammaFiredStripDetectorMinusZToTrue();
			}	
			
			//G4cout<< "energyDedx2: " << energyDedx2 << G4endl;
		}
		
	}


	if(DEDX1HC)
	{
		int n_hit = DEDX1HC->entries();
		for(int i=0;i<n_hit;i++)
		{ 

			
			energyDedx += (*DEDX1HC)[i]->GetEdep();
			if((*DEDX1HC)[i]->GetPrimary())
			{
				energyDedx_Primaries +=(*DEDX1HC)[i]->GetEdep();
				stripHandler->StoreStripInformation((*DEDX1HC)[i]->GetPos2(),(*DEDX1HC)[i]->GetEdep(),true,true);
			}
			else
			{
				energyDedx_Secondaries +=(*DEDX1HC)[i]->GetEdep();
				stripHandler->StoreStripInformation((*DEDX1HC)[i]->GetPos2(),(*DEDX1HC)[i]->GetEdep(),false,true);
			}

		}
		
		if(energyDedx>0)
		{
			isThereEnergyDedx = true;

			if(energyDedx_Primaries ==0)
			{
				EventInformation->setGammaFiredStripDetectorPlusZToTrue();
			}	
			//G4cout<< "energyDedx: " << energyDedx << G4endl;

		}
		//G4cout<< isThereEnergySili << " " << isThereEnergySiliPrimary << " "<< isThereEnergySiliSecondary << " "<<isThereEnergyDedx << " " << isThereEnergyDedxPrimary << " "<< isThereEnergyDedxSecondary<< " " << isThereEnergySili2  << " " << isThereEnergySili2Primary << " "<< isThereEnergySili2Secondary<< " "<<isThereEnergyDedx2 << " " << isThereEnergyDedx2Primary << " "<< isThereEnergyDedx2Secondary<< G4endl;
		
	}

	

	
	if((isThereEnergyDedx == true && isThereEnergySili == true && isThereEnergyDedx2 == false && isThereEnergySili2 == false) ||  (isThereEnergyDedx == false && isThereEnergySili == false && isThereEnergyDedx2 == true && isThereEnergySili2 == true ))
	{
		accepted++;
		runAct->SetAccepted();
		EventInformation->setThisIsAnAccepterEvent();
		
		//runAct->SetAcceptedPrimaryScatteredOffHoops();
			
		

			
			if(isThereEnergyDedx == true )
			{
				//stripHandler->PrintMaps();
				stripHandler->PassThePlusZDetectors();

				ofstream detectorADA;
				detectorADA.open ("detectorADA.txt", ofstream::out | ofstream::app);
				detectorADA.setf( std::ios::fixed, std::ios::floatfield );
				detectorADA << setw(15)<< left << energyDedx/keV << setw(15)<< left << energySiLi/keV  << G4endl;
				detectorADA.close();

				histograms->FillNtuple(1,0,energySiLi);
				histograms->FillNtuple(0,0,energyDedx);
				histograms->AddRowNtuple(1);
				histograms->AddRowNtuple(0);

				EventInformation->setTotalEnergyInScintillator(energySiLi);
				EventInformation->setTotalEnergyInStripDetector(energyDedx);
				EventInformation->setPlusZDetectorsFiredToTrue();
				EventInformation->setStartStripDetectorTime((*DEDX1HC)[0]->GetTime());

				if(listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded())
				{	
					listOfEnteredVolumes->setEnteredVolumeName("----------------------A", 0, 0 );
				}
				
			}
			
			if(isThereEnergyDedx2 == true )
			{
				stripHandler->PassTheMinusZDetectors();

				ofstream detectorODA;
				detectorODA.open ("detectorODA.txt", ofstream::out | ofstream::app);
				detectorODA.setf( std::ios::fixed, std::ios::floatfield );
				detectorODA << setw(15)<< left << energyDedx2/keV << setw(15)<< left << energySiLi2/keV   << G4endl;
				detectorODA.close();

				histograms->FillNtuple(3,0,energySiLi2);
				histograms->FillNtuple(2,0,energyDedx2);
				histograms->AddRowNtuple(3);
				histograms->AddRowNtuple(2);

				EventInformation->setTotalEnergyInScintillator(energySiLi2);
				EventInformation->setTotalEnergyInStripDetector(energyDedx2);
				EventInformation->setStartStripDetectorTime((*DEDX2HC)[0]->GetTime());
				
				if(listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded())
				{	
					listOfEnteredVolumes->setEnteredVolumeName("----------------------O", 0, 0 );
				}
			}
			
			
			//G4cout<< start[0].theta()<<G4endl;
			/*
			if(spot.size() >= 1)
			{
				std::ofstream thetaOut;
				thetaOut.open ("theta.txt", std::ofstream::out | std::ofstream::app);
				thetaOut << setw (15) << left << start[0].theta()<< setw (15) << left << spot[0].theta()<< G4endl;
				thetaOut.close();
			
				//G4cout<<start[0].theta()<< " "<<spot[0].theta()<< G4endl;
			}
			*/
		
			//listOfEnteredVolumes->setEnteredVolumeName("----------------------" );
			


	}
	else
	{
		if(listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded())
		{	
			listOfEnteredVolumes->deleteEnteriesFromList(differenceInListSizeFromStartToEnd);
		}
	}

	
	if(listOfEnteredVolumes-> getShouldVolumeNamesBeRecorded())
	{	
		listOfEnteredVolumes->checkIfPrintIsNeeded(false);
	}
	
	if ((evt->GetEventID())%50000==0)
		{
			G4cout << ">>> End of Event " << evt->GetEventID() << G4endl;
		}
	

	 
	spot.clear();
	start.clear();

	if (EventInformation->getShouldEventInformationBeRecorded())
	{
		if(EventInformation->getIsThisAnAcceptedEvent())
		{
			EventInformation->EndOfEventActions();
			EventInformation->printEventInformation();
		}
		EventInformation->clearEventInformation();
	}
	
}

//----------------
void K37EventAction::setEnteringDedx(G4ThreeVector enteringPosition)
{
	spot.push_back(enteringPosition);
		
}

void K37EventAction::setStartingDirection(G4ThreeVector startingPosition)
{
	start.push_back(startingPosition);
	
}




