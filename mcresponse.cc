#include "ArgumentParser.hh"
#include "MCFileHandler.hh"
#include "DetectorResponse.hh"
#include "DetectorCharacteristics.hh"
#include "PseudoDataHandler.hh"

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
void MergeTPI(std::vector<TPulseIsland*>& v, int pedestal);
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
	args.Register("d", "detectors", "", "Detector text file to use.", true,
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
	std::string rfname = args.Get("f");

	MCFileHandler files;
	if (args.Enabled("f")) {
		files.RegisterFile(rfname);
	} else {
		if (!files.Register(runs)) {
			std::cout << "Could not register runs!" << std::endl;
			return 2;
		}
	}
	files.Load();

	DetectorCharacteristics dets;
	if (args.Enabled("d"))
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

	/*** Prepare For Heavy Lifting ***/
	/********************************************/
	// Declare the stuff we're going to use
	const int nPhysDets = 10;
	const int nBanks = 15;
	std::vector < DetectorResponse > dets;
	PseudoDataHandler pdh(&gData);
	std::vector < std::string > banknames;
	// Some constants
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
	int physdets[nPhysDets] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11};
	int physdets_banks[nBanks] = {0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 7, 8, 11};
	// Prepare the stuff we're going to use
	for (int iDet = 0; iDet < nBanks; iDet++)
		dets.push_back(detchars.GetDetectorResponse(banknames[iDet]));
	pdh.Add(nPhysDets, physdets);
	for(int iBank = 0; iBank < nBanks; iBank++)
		pdh.RegisterResponse(physdets_banks[iBank], banknames[iBank], dets[iBank]);
	/********************************************/
	// Prepare for loop
	TRandom3 randtime;
	int nEvt = mcevents->GetEntries();
	int nEDeps = 0;
	int det;
	double energy, time;
	double eventtimestamp = 0.;
	// Make pseudodata!
	for (int iEvt = 0; iEvt < nEvt; iEvt++) {
		if (iEvt % 10000 == 0)
			std::cout << "Working on event " << iEvt << " of " << nEvt << "..."
					<< std::endl;

		// Finish a block
		if (eventtimestamp > evtwindow) {
			// Block is over. Save leaf...
			pdh.PrepareBlock();
			mcdata.Fill();
			// Clear data for reuse
			eventtimestamp = 0.;
			pdh.Clear();;
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
			pdh.ProcessDeposition(det, energy, time+eventtimestamp);
		}
	}
	mcdata.Write();
	mctruth.Write();
}
