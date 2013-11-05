#include "ArgumentParser.hh"
#include "MCFileHandler.hh"
#include "DetectorResponse.hh"
#include "DetectorCharacteristics.hh"

#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TRandom3.h"

#include "TTrackerHit.hh"
#include "TEvent.hh"

#include "TGlobalData.hh"
#include "TPulseIsland.hh"

#include <string>
#include <vector>
#include <utility>
#include <iostream>

void ClearVectorTPI(std::vector<TPulseIsland*>& v);
void TimeOrderTPI(std::vector<TPulseIsland*>& v);
void MergeTPI(std::vector<TPulseIsland*>& v);
void producemcdata(TChain*, DetectorCharacteristics&, TString);

int main(int argc, char* argv[]) {

	ArgumentParser args;
	args.Register("", "usage", "", "Print help message and exit.", false,
			false);
	args.Register("h", "help", "", "Print help message and exit.", false,
			false);
	args.Register("r", "runs", "",
			"Comma/Dash separated list of MC runs to produce data for. (ex: 203-567,1002,2700-3199)",
			true, true);
	args.Register("d", "detectors", "",
			"Detector text file to use. (To be implemented...)", true, false);
	args.Register("f", "runlist", "",
			"Test file with runlist to load. (To be implemented...)", true,
			false);
	args.Register("o", "output-file", "mcdata.root",
			"Output file name for pseudo-data.", true, false);
	args.Register("f", "input-file", "",
			"Input file name with MC data (To be implemented...)", true, false);
	args.Parse(argc, argv);

	// Asked for help or error
	if (args.Enabled("h") || args.Enabled("help") || args.Enabled("usage")) {
		args.Print();
		return 0;
	}
	if (!args.Valid()) {
		std::cout << "Argument error!" << std::endl;
		args.Print();
		return 1;
	}

	std::string runs = args.Get("r");
	std::string ofname = args.Get("o");
	std::string detfname = args.Get("d");

	MCFileHandler files;
	if (!files.Register(runs)) {
		std::cout << "Could not register runs!" << std::endl;
		return 2;
	}
	files.Load();

	DetectorCharacteristics dets;
	if(args.Enabled("d"))
		dets.LoadFile(detfname);

	dets.Print();

	std::cout << "Output file: " << ofname << "." << std::endl;

	TChain* runchain = files.GetEventChain();
	producemcdata(runchain, dets, TString(ofname));

	std::cout << "Succesfully reached the end of the program!" << std::endl;
	return 0;
}

void producemcdata(TChain* mcevents, DetectorCharacteristics& detchars,
		TString ofname) {

	// Parameters
	const double beamtau = 100000.; // ns, 10 kHz beam
	const double evtwindow = 10000000.; // ns, 10 ms event size
	const double hitwindow = 5.; // ns

	// Data pointers
	TEvent* ev = NULL;
	TGlobalData* gData = NULL;
	std::vector<double> *t, *e;
	std::vector<int> *d, *p;

	// Prepare input
	mcevents->SetBranchStatus("*", 0);
	mcevents->SetBranchStatus("evt", 1);
	mcevents->SetBranchAddress("evt", &ev);

	// Prepare output
	TFile ofile(ofname, "RECREATE");
	TTree mcdata("EventTree", "All MC Events");
	TTree mctruth("TruthTree_INCOMPLETE",
			"Truth Information (Not Yet Implemented)");
	mcdata.Branch("Event", "TGlobalData", &gData, 64000, 1);

	// Prepare detectors
	std::vector<std::string> banknames;
	banknames.push_back("MC00");
	banknames.push_back("MCF1");
	banknames.push_back("MCS1");
	banknames.push_back("MC02");
	banknames.push_back("MCF3");
	banknames.push_back("MCS3");
	banknames.push_back("MCF4");
	banknames.push_back("MCS4");
	banknames.push_back("MC05");
	banknames.push_back("MCF6");
	banknames.push_back("MCS6");
	banknames.push_back("MCF7");
	banknames.push_back("MCS7");
	banknames.push_back("MC08");
	banknames.push_back("MC11");
	int nBanks = banknames.size();
	std::vector<DetectorResponse> dets;
	for (int iDet = 0; iDet < nBanks; iDet++)
		dets.push_back(detchars.GetDetectorResponse(banknames[iDet]));
	std::vector<std::vector<TPulseIsland*> > tpis(nBanks);

	TRandom3 randtime;
	int nEvt = mcevents->GetEntries();
	int nEDeps = 0;

	int det;
	double energy, time;

	double eventtimestamp = 0.;

	for (int iEvt = 0; iEvt < nEvt; iEvt++) {
		if (iEvt % 10000 == 0)
			std::cout << "Working on event " << iEvt << " of " << nEvt << "..."
					<< std::endl;

		// Finish a run
		if (eventtimestamp > evtwindow) {
			// Time order data and merge those close in time
			// Then add to global data pointer
			for (int iDet = 0; iDet < nBanks; iDet++) {
				TimeOrderTPI(tpis[iDet]);
				MergeTPI(tpis[iDet]);
				gData->fPulseIslandToChannelMap.insert(
						std::pair<std::string, std::vector<TPulseIsland*> >(
								banknames[iDet], tpis[iDet]));
			}
			mcdata.Fill();
			// Clear data for reuse
			eventtimestamp = 0.;
			gData->fPulseIslandToChannelMap.clear();
			for (int iDet = 0; iDet < nBanks; iDet++)
				ClearVectorTPI(tpis[iDet]);
		}

		eventtimestamp += randtime.Exp(beamtau);
		mcevents->GetEntry(iEvt);
		nEDeps = ev->GetDetectorResponse(&p, &d, &e, &t);
		for (int iDep = 0; iDep < nEDeps; iDep++) {
			det = d->at(iDep);
			if (det == 12 || det == 9)
				continue;
			time = t->at(iDep);
			energy = e->at(iDep);
			while (iDep < nEDeps && det == d->at(iDep)
					&& std::abs(time - d->at(iDep)) < hitwindow) {
				energy += e->at(iDep);
				iDep++;
			}
			if (det == 0) {
				if (dets[0].IsOverThreshold(energy))
					tpis[0].push_back(
							dets[0].GetResponse(energy, time + eventtimestamp,
									"MC00"));
			} else if (det == 1) {
				if (dets[1].IsOverThreshold(energy))
					tpis[1].push_back(
							dets[1].GetResponse(energy,
									time + eventtimestamp, "MCF1"));
				if (dets[2].IsOverThreshold(energy))
					tpis[2].push_back(
							dets[2].GetResponse(energy,
									time + eventtimestamp, "MCS1"));
			} else if (det == 2) {
				if (dets[3].IsOverThreshold(energy))
					tpis[3].push_back(
							dets[3].GetResponse(energy,
									time + eventtimestamp, "MC02"));
			} else if (det == 3) {
				if (dets[4].IsOverThreshold(energy))
					tpis[4].push_back(
							dets[4].GetResponse(energy,
									time + eventtimestamp, "MCF3"));
				if (dets[5].IsOverThreshold(energy))
					tpis[5].push_back(
							dets[5].GetResponse(energy,
									time + eventtimestamp, "MCS3"));
			} else if (det == 4) {
				if (dets[6].IsOverThreshold(energy))
					tpis[6].push_back(
							dets[6].GetResponse(energy,
									time + eventtimestamp, "MCF4"));
				if (dets[7].IsOverThreshold(energy))
					tpis[7].push_back(
							dets[7].GetResponse(energy,
									time + eventtimestamp, "MCS4"));
			} else if (det == 5) {
				if (dets[8].IsOverThreshold(energy))
					tpis[8].push_back(
							dets[8].GetResponse(energy,
									time + eventtimestamp, "MC05"));
			} else if (det == 6) {
				if (dets[9].IsOverThreshold(energy))
					tpis[9].push_back(
							dets[9].GetResponse(energy,
									time + eventtimestamp, "MCF6"));
				if (dets[10].IsOverThreshold(energy))
					tpis[10].push_back(
							dets[10].GetResponse(energy,
									time + eventtimestamp, "MCS6"));
			} else if (det == 7) {
				if (dets[11].IsOverThreshold(energy))
					tpis[11].push_back(
							dets[11].GetResponse(energy,
									time + eventtimestamp, "MCF7"));
				if (dets[12].IsOverThreshold(energy))
					tpis[12].push_back(
							dets[12].GetResponse(energy,
									time + eventtimestamp, "MCS7"));
			} else if (det == 8) {
				if (dets[13].IsOverThreshold(energy))
					tpis[13].push_back(
							dets[13].GetResponse(energy,
									time + eventtimestamp, "MC08"));
			} else if (det == 11) {
				if (dets[14].IsOverThreshold(energy))
					tpis[14].push_back(
							dets[14].GetResponse(energy,
									time + eventtimestamp, "MC11"));
			} else {
				std::cout << "Error: Unknown detector:\t" << det << std::endl;
			}
		}
	}
	mcdata.Write();
	mctruth.Write();
}

void ClearVectorTPI(std::vector<TPulseIsland*>& v) {
	for (unsigned int i = 0; i < v.size(); i++)
		if (v[i])
			delete v[i];
	v.clear();
}

void TimeOrderTPI(std::vector<TPulseIsland*>& v) {
	static TPulseIsland* pTPI;
	static int n;
	static int i, j;
	static int min, t;

	n = v.size();
	if (n == 0)
		return;
	for (i = 0; i < n - 1; i++) {
		min = i;
		t = v.at(i)->GetTimeStamp();
		for (j = i + 1; j < n; j++) {
			if (v.at(j)->GetTimeStamp() < t) {
				min = j;
				t = v.at(j)->GetTimeStamp();
			}
		}
		if (min != i) {
			pTPI = v.at(min);
			v[min] = v.at(i);
			v[i] = pTPI;
		}
	}
}

void MergeTPI(std::vector<TPulseIsland*>& v) {
	static TPulseIsland* pTPI;
	static int nSamp;
	static unsigned int i;
	static int j;
	static int t, dt;
	static double tick;
	static std::string name;
	if (v.size() <= 1)
		return;

	i = 0;
	pTPI = v.at(0);
	nSamp = pTPI->GetSamples().size();
	tick = pTPI->GetClockTickInNs();
	name = pTPI->GetBankName();
	while (i < v.size() - 1) {
		while ((dt = std::abs(
				(t = v.at(i)->GetTimeStamp()) - v.at(i + 1)->GetTimeStamp()))
				< nSamp) {
			std::vector<int> s2 = v.at(i + 1)->GetSamples();
			std::vector<int> s1 = v.at(i)->GetSamples();
			for (j = dt; j < nSamp; j++)
				s1[j] += s2[j - dt];
			delete v.at(i);
			delete v.at(i + 1);
			v[i] = new TPulseIsland(t, s1, tick, name);
			v.erase(v.begin() + i + 1);
			if (i == v.size() - 1)
				return;
		}
		i++;
	}
}
