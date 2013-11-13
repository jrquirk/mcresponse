#include "PseudoDataHandler.hh"
#include "TPulseIsland.hh"
#include "TGlobalData.hh"

#include <vector>
#include <string>
#include <iostream>

PseudoDataHandler::PseudoDataHandler(TGlobalData** gData) :
		fNPhysDets(0) {
	fGData = gData;
}

PseudoDataHandler::~PseudoDataHandler() {
}

void PseudoDataHandler::Add(int physdet) {
	fNPhysDets++;
	fPhysicalDetectors.push_back(physdet);
	fNResponses.push_back(0);
	fBankNames.push_back(std::vector<std::string>());
	fResponses.push_back(std::vector<DetectorResponse>());
	fTPIs.push_back(std::vector<std::vector<TPulseIsland*> >());
}

void PseudoDataHandler::Add(int ndets, int dets[]) {
	for (int i = 0; i < ndets; i++)
		Add(dets[i]);
}

void PseudoDataHandler::RegisterResponse(int physdet, std::string& bankname,
		DetectorResponse& detresp) {
	for (int i = 0; i < fNPhysDets; i++) {
		if (fPhysicalDetectors[i] == physdet) {
			fBankNames[i].push_back(bankname);
			fResponses[i].push_back(detresp);
			fTPIs[i].push_back(std::vector<TPulseIsland*>());
			fNResponses[i]++;
			return;
		}
	}
	std::cout << "Error: No physical detector like that! (" << physdet << ")"
			<< std::endl;
}

bool PseudoDataHandler::ProcessDeposition(int det, double energy, double time) {
	int iDet = -1;
	for (int i = 0; i < fNPhysDets; i++) {
		if (fPhysicalDetectors[i] == det) {
			iDet = i;
			break;
		}
	}
	if (iDet == -1)
		return false;

	TPulseIsland* tmpTPI;
	for (int i = 0; i < fNResponses[iDet]; i++) {
		tmpTPI = fResponses[iDet][i].GetResponse(energy, time,
				fBankNames[iDet][i]);
		if (tmpTPI)
			fTPIs[iDet][i].push_back(tmpTPI);
	}
	return true;
}

void PseudoDataHandler::PrepareBlock() {
	TimeOrder();
	Merge();
	for (int iPhysDet = 0; iPhysDet < fNPhysDets; iPhysDet++)
		for (int iResp = 0; iResp < fNResponses[iPhysDet]; iResp++)
			(*fGData)->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >(
							fBankNames[iPhysDet][iResp],
							fTPIs[iPhysDet][iResp]));
}

void PseudoDataHandler::TimeOrder() {
	static TPulseIsland* tmpTPI;
	static std::vector<TPulseIsland*>* tmpTPIVec;
	static int nTPIs;
	static int iPhysDet, iResp;
	static int iTPI, jTPI;
	static int min, t;

	for (iPhysDet = 0; iPhysDet < fNPhysDets; iPhysDet++) {
		for (iResp = 0; iResp < fNResponses[iPhysDet]; iResp++) {
			tmpTPIVec = &fTPIs[iPhysDet][iResp];

			nTPIs = tmpTPIVec->size();
			if (nTPIs == 0)
				continue;
			for (iTPI = 0; iTPI < nTPIs - 1; iTPI++) {
				min = iTPI;
				t = tmpTPIVec->at(iTPI)->GetTimeStamp();
				for (jTPI = iTPI + 1; jTPI < nTPIs; jTPI++) {
					if (tmpTPIVec->at(jTPI)->GetTimeStamp() < t) {
						min = jTPI;
						t = tmpTPIVec->at(jTPI)->GetTimeStamp();
					}
				}
				if (min != iTPI) {
					tmpTPI = tmpTPIVec->at(min);
					tmpTPIVec->at(min) = tmpTPIVec->at(iTPI);
					tmpTPIVec->at(iTPI) = tmpTPI;
				}
			}
		}
	}
}

void PseudoDataHandler::Merge() {
	static std::vector<TPulseIsland*>* tmpTPIVec;
	static TPulseIsland *tmpTPI1, *tmpTPI2;
	static int nSamp;
	static int pedestal;
	static int iPhysDet, iResp;
	static unsigned int iTPI;
	static int iSamp;
	static int t1, t2, dt;
	static double tick;
	static std::string name;
	static std::vector<int> samps1, samps2;

	for (iPhysDet = 0; iPhysDet < fNPhysDets; iPhysDet++) {
		for (iResp = 0; iResp < fNResponses[iPhysDet]; iResp++) {
			tmpTPIVec = &fTPIs[iPhysDet][iResp];
			pedestal = fResponses[iPhysDet][iResp].GetPedestal();
			
			if (tmpTPIVec->size() <= 1)
				continue;

			iTPI = 0;
			tmpTPI1 = tmpTPIVec->at(0);
			nSamp = tmpTPI1->GetSamples().size();
			tick = tmpTPI1->GetClockTickInNs();
			name = tmpTPI1->GetBankName();
			while (iTPI < tmpTPIVec->size() - 1) {
				tmpTPI1 = tmpTPIVec->at(iTPI);
				tmpTPI2 = tmpTPIVec->at(iTPI + 1);
				t1 = tmpTPI1->GetTimeStamp();
				t2 = tmpTPI2->GetTimeStamp();
				dt = t2 - t1;
				if (dt < nSamp) {
					samps1 = tmpTPI1->GetSamples();
					samps2 = tmpTPI2->GetSamples();
					for (iSamp = dt; iSamp < nSamp; iSamp++)
						samps1[iSamp] += samps2[iSamp - dt] - pedestal;
					delete tmpTPI1;
					delete tmpTPI2;
					tmpTPIVec->at(iTPI) = new TPulseIsland(t1, samps1, tick, name);
					tmpTPIVec->erase(tmpTPIVec->begin() + iTPI + 1);
				} else {
					iTPI++;
				}
			}
		}
	}
}

void PseudoDataHandler::Clear() {
	static int iPhysDet, iResp;
	static unsigned int iTPI;
	static TPulseIsland* cTPI;
	
	(*fGData)->fPulseIslandToChannelMap.clear();
	for (iPhysDet = 0; iPhysDet < fNPhysDets; iPhysDet++) {
		for (iResp = 0; iResp < fNResponses[iPhysDet]; iResp++) {
			std::vector<TPulseIsland*>& cTPIVec = fTPIs[iPhysDet][iResp];
			for(iTPI = 0; iTPI < cTPIVec.size(); iTPI++) {
				TPulseIsland* cTPI = cTPIVec[iTPI];
				if(cTPI)
					delete cTPI;
			}
			cTPIVec.clear();
		}
	}
}

void PseudoDataHandler::Print() {
	for (int iPhysDet = 0; iPhysDet < fNPhysDets; iPhysDet++) {
		std::cout<<fPhysicalDetectors[iPhysDet]<<'\t';
		for (int iResp = 0; iResp < fNResponses[iPhysDet]; iResp++) {
			std::cout<<fBankNames[iPhysDet][iResp]<<'\t';
			std::cout<<std::endl<<"\t";
		}
		std::cout<<std::endl;
	}
}