
#ifndef K37HistogramManager_h
#define K37HistogramManager_h 1

#include "globals.hh"

//-----------------------------------------

namespace AIDA
{
	class IAnalysisFactory;
	class ITree;
	class ITuple;
}

const G4int MaxNtupl = 4;

//-----------------------------------------

class K37HistogramManager
{
	public:

		K37HistogramManager();
		~K37HistogramManager();

		void book();
		void save();   
		void FillNtuple(G4int id, G4int column, G4double value);
		void AddRowNtuple(G4int id);

	private:

		G4String                 fileName[2];
		G4String                 fileType;
		G4String                 fileOption;    
		AIDA::IAnalysisFactory*  af;
		AIDA::ITree*             tree;
		AIDA::ITuple*      	 ntupl0; // StripDetectorPlusZ   
		AIDA::ITuple*            ntupl1; // ScintillatorPlusZ   
		AIDA::ITuple*            ntupl2; // StripDetectorMinusZ  
		AIDA::ITuple*            ntupl3; // ScintillatorMinusZ   

		G4bool                   factoryOn;
    
};

//-----------------------------------------

#endif

