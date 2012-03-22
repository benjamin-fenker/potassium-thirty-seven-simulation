
#include "K37AnnihilationPosition.hh"
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include <stdio.h>
#include <sys/stat.h> 

K37AnnihilationPosition::K37AnnihilationPosition(std::string _filename)
{
	filename = _filename;
	sizeOfVector = 1000;
	xPos.clear();
	yPos.clear();
	zPos.clear();
	this->deleteFile();
	usefulness = false;
}

K37AnnihilationPosition::K37AnnihilationPosition(std::string _filename, int _sizeOfVector)
{
	filename = _filename;
	sizeOfVector = _sizeOfVector;
	xPos.clear();
	yPos.clear();
	zPos.clear();
	this->deleteFile();
	usefulness = false;
}

K37AnnihilationPosition::~K37AnnihilationPosition()
{;}

void K37AnnihilationPosition::touchFile()
{
	fileOfK37AnnihilationPositions.open (filename.c_str(), std::ofstream::out | std::ofstream::trunc);
	fileOfK37AnnihilationPositions.close();
}

void K37AnnihilationPosition::deleteFile()
{
	
	std::string  strFilename = filename;
	struct stat stFileInfo; 
	int intStat; 

	 intStat = stat(strFilename.c_str(),&stFileInfo); 
	 if(intStat == 0)
	 { 
		 remove(strFilename.c_str()); 
		 //G4cout<<"File "<<strFilename<<" removed."<<G4endl;
	 }
	 //else
	 //{ 
		 //G4cout<<"File "<<strFilename<<" did not exist."<<G4endl;
		 //} 

}

void K37AnnihilationPosition::setPosition(G4double x, G4double y, G4double z)
{
	xPos.push_back(x);
	yPos.push_back(y);
	zPos.push_back(z);
	this->checkIfPrintIsNeeded(false);
}
	
void K37AnnihilationPosition::clearList()
{
	xPos.clear();
	yPos.clear();
	zPos.clear();
}
	
void K37AnnihilationPosition::deleteEnteryFromList()
{
	xPos.pop_back();
	yPos.pop_back();
	zPos.pop_back();
}
	
void K37AnnihilationPosition::printPosition()
{	
	
	fileOfK37AnnihilationPositions.open (filename.c_str(), std::ofstream::out | std::ofstream::app);

	//for (unsigned int i=0; i<xPos.size(); i++)
	for (int i=0; i<int(xPos.size()); i++)
	{
		fileOfK37AnnihilationPositions<< std::setw(15) << std::left<< xPos[int(i)] << std::setw(15)<< yPos[int(i)] << std::setw(15)<< zPos[int (i)]<< G4endl;
	}

	fileOfK37AnnihilationPositions.close();

}

void K37AnnihilationPosition::checkIfPrintIsNeeded(G4bool isThisTheLastEvent)
{
	if(isThisTheLastEvent == true)
	{
		this->printPosition();
		xPos.clear();
		yPos.clear();
		zPos.clear();
	}
	else
	{
		if(xPos.size() >= sizeOfVector)
		{
			this->printPosition();
			xPos.clear();
			yPos.clear();
			zPos.clear();
		}
	}
}

unsigned int K37AnnihilationPosition::checkSizeOfList()
{	
	return xPos.size();
}
