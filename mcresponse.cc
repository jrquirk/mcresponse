#include "ArgumentParser.hh"
#include "MCFileHandler.hh"
#include "DetectorResponse.hh"

#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TRandom3.h"

#include "rootlibs/TTrackerHit.h"
#include "rootlibs/TEvent.h"

#include "TGlobalData.hh"
#include "TPulseIsland.hh"

#include <string>
#include <vector>
#include <utility>
#include <iostream>

void ClearVectorTPI(std::vector<TPulseIsland*>& v);
void TimeOrderTPI(std::vector<TPulseIsland*>& v);
void MergeTPI(std::vector<TPulseIsland*>& v);
void producemcdata(TChain*, TString);

int main(int argc, char* argv[]) {

	ArgumentParser args;
	args.Register("", "usage", "", "Print help message and exit.", false,
			false);
	args.Register("h", "help", "", "Print help message and exit.", false,
			false);
	args.Register("r", "runs", "", "Runs to produce data for. (ex: 203-567,1002,2700-3199)", true, true);
	args.Register("d", "detectors", "", "Detector text file to use.", true,
			false);
	args.Register("f", "runlist", "", "Test file with runlist to load.", true,
			false);
	args.Register("o", "output-file", "mcdata.root",
			"Output file name for pseudo-data.", true, false);
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
	MCFileHandler files;
	if (!files.Register(runs)) {
		std::cout << "Could not register runs!" << std::endl;
		return 2;
	}
	files.Load();

	std::cout << "Output file: " << ofname << "." << std::endl;

	TChain* runchain = files.GetEventChain();
	producemcdata(runchain, TString(ofname));

	std::cout << "Succesfully reached the end of the program!" << std::endl;
	return 0;
}

void producemcdata(TChain* mcevents, TString ofname) {

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
	std::vector<TPulseIsland*> musc, musca, targ_f, targ_s, veto, sil1_f,
			sil1_s, sil2_f, sil2_s, scl, sir1_f, sir1_s, sir2_f, sir2_s, scr;

	// Prepare detectors
	DetectorResponse si_tfa, si_amp, sc;
	si_tfa.SetPulseProperties(25., 50., 1000., 100., 1);
	si_tfa.SetDigitizationProperties(0.170, 10000., 12, 0, 100, 20);
	si_tfa.SetCalibration(10.);
	si_amp.SetPulseProperties(500., 1000., 1000., 100., 1);
	si_amp.SetDigitizationProperties(0.017, 10000., 12, 0, 100, 20);
	si_amp.SetCalibration(3.);
	sc.SetPulseProperties(5., 10., 1000., 100., -1);
	sc.SetDigitizationProperties(0.170, 1000., 12, 4095, 100, 20);
	sc.SetCalibration(5.);

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
		if (eventtimestamp > evtwindow) {
			// Time order data
			TimeOrderTPI(musc);
			TimeOrderTPI(targ_f);
			TimeOrderTPI(targ_s);
			TimeOrderTPI(veto);
			TimeOrderTPI(sil1_f);
			TimeOrderTPI(sil1_s);
			TimeOrderTPI(sil2_f);
			TimeOrderTPI(sil2_s);
			TimeOrderTPI(scl);
			TimeOrderTPI(sir1_f);
			TimeOrderTPI(sir1_s);
			TimeOrderTPI(sir2_f);
			TimeOrderTPI(sir2_s);
			TimeOrderTPI(scr);
			TimeOrderTPI(musca);
			//Merge
			MergeTPI(musc);
			MergeTPI(targ_f);
			MergeTPI(targ_s);
			MergeTPI(veto);
			MergeTPI(sil1_f);
			MergeTPI(sil1_s);
			MergeTPI(sil2_f);
			MergeTPI(sil2_s);
			MergeTPI(scl);
			MergeTPI(sir1_f);
			MergeTPI(sir1_s);
			MergeTPI(sir2_f);
			MergeTPI(sir2_s);
			MergeTPI(scr);
			MergeTPI(musca);
			// Fill data structure
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MC00",
							musc));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCF1",
							targ_f));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCS1",
							targ_s));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MC02",
							veto));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCF3",
							sil1_f));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCS3",
							sil1_s));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCF4",
							sil2_f));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCS4",
							sil2_s));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MC05",
							scl));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCF6",
							sir1_f));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCS6",
							sir1_s));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCF7",
							sir2_f));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MCS7",
							sir2_s));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MC08",
							scr));
			gData->fPulseIslandToChannelMap.insert(
					std::pair<std::string, std::vector<TPulseIsland*> >("MC11",
							musca));
			mcdata.Fill();
			// Clear data for reuse
			eventtimestamp = 0.;
			gData->fPulseIslandToChannelMap.clear();
			ClearVectorTPI(musc);
			ClearVectorTPI(musc);
			ClearVectorTPI(musca);
			ClearVectorTPI(targ_f);
			ClearVectorTPI(targ_s);
			ClearVectorTPI(veto);
			ClearVectorTPI(sil1_f);
			ClearVectorTPI(sil1_s);
			ClearVectorTPI(sil2_f);
			ClearVectorTPI(sil2_s);
			ClearVectorTPI(scl);
			ClearVectorTPI(sir1_f);
			ClearVectorTPI(sir1_s);
			ClearVectorTPI(sir2_f);
			ClearVectorTPI(sir2_s);
			ClearVectorTPI(scr);
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
				if (sc.IsOverThreshold(energy))
					musc.push_back(
							sc.GetExponentialResponse(energy,
									time + eventtimestamp, "MC00"));
			} else if (det == 1) {
				if (si_tfa.IsOverThreshold(energy))
					targ_f.push_back(
							si_tfa.GetExponentialResponse(energy,
									time + eventtimestamp, "MCF1"));
				if (si_amp.IsOverThreshold(energy))
					targ_s.push_back(
							si_amp.GetGaussianResponse(energy,
									time + eventtimestamp, "MCS1"));
			} else if (det == 2) {
				if (sc.IsOverThreshold(energy))
					veto.push_back(
							sc.GetExponentialResponse(energy,
									time + eventtimestamp, "MC02"));
			} else if (det == 3) {
				if (si_tfa.IsOverThreshold(energy))
					sir1_f.push_back(
							si_tfa.GetExponentialResponse(energy,
									time + eventtimestamp, "MCF3"));
				if (si_amp.IsOverThreshold(energy))
					sir1_s.push_back(
							si_amp.GetGaussianResponse(energy,
									time + eventtimestamp, "MCS3"));
			} else if (det == 4) {
				if (si_tfa.IsOverThreshold(energy))
					sir2_f.push_back(
							si_tfa.GetExponentialResponse(energy,
									time + eventtimestamp, "MCF4"));
				if (si_amp.IsOverThreshold(energy))
					sir2_s.push_back(
							si_amp.GetGaussianResponse(energy,
									time + eventtimestamp, "MCS4"));
			} else if (det == 5) {
				if (sc.IsOverThreshold(energy))
					scr.push_back(
							sc.GetExponentialResponse(energy,
									time + eventtimestamp, "MC05"));
			} else if (det == 6) {
				if (si_tfa.IsOverThreshold(energy))
					sil1_f.push_back(
							si_tfa.GetExponentialResponse(energy,
									time + eventtimestamp, "MCF6"));
				if (si_amp.IsOverThreshold(energy))
					sil1_s.push_back(
							si_amp.GetGaussianResponse(energy,
									time + eventtimestamp, "MCS6"));
			} else if (det == 7) {
				if (si_tfa.IsOverThreshold(energy))
					sil2_f.push_back(
							si_tfa.GetExponentialResponse(energy,
									time + eventtimestamp, "MCF7"));
				if (si_amp.IsOverThreshold(energy))
					sil2_s.push_back(
							si_amp.GetGaussianResponse(energy,
									time + eventtimestamp, "MCS7"));
			} else if (det == 8) {
				if (sc.IsOverThreshold(energy))
					scl.push_back(
							sc.GetExponentialResponse(energy,
									time + eventtimestamp, "MC08"));
			} else if (det == 11) {
				if (sc.IsOverThreshold(energy))
					musca.push_back(
							sc.GetExponentialResponse(energy,
									time + eventtimestamp, "MC11"));
			} else {
				std::cout << "Error: Unknown detector:\t" << det << std::endl;
			}
		}
	}
	mcdata.Write();
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
