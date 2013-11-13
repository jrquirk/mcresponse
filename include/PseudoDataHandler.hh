#ifndef _PSEUDODATAHANDLER_HH_
#define _PSEUDODATAHANDLER_HH_

#include "DetectorResponse.hh"

#include <vector>
#include <string>

class TGlobalData;

class PseudoDataHandler {

	TGlobalData** fGData;
	int fNPhysDets;
	std::vector<int> fPhysicalDetectors;
	std::vector<int> fNResponses;
	std::vector<std::vector<std::string> > fBankNames;
	std::vector<std::vector<DetectorResponse> > fResponses;
	std::vector< std::vector< std::vector<TPulseIsland*> > > fTPIs;

public:
	PseudoDataHandler(TGlobalData**);
	~PseudoDataHandler();

public:
	void Add(int);
	void Add(int, int[]);
	void RegisterResponse(int,std::string&,DetectorResponse&);
	bool ProcessDeposition(int,double,double);
	void PrepareBlock();
	void Clear();
	void Print();
	
private:
	void TimeOrder();
	void Merge();
};

#endif
