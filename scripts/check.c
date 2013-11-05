#include <iostream>
#include <ctime>

#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TString.h"
#include "TCanvas.h"

#include "rootlibs/TGlobalData.hh"
#include "rootlibs/TPulseIsland.hh"

void check(TString fname, TString channel) {
	TCanvas* can = new TCanvas();
	TFile f(fname);
	TTree* tr = (TTree*) f.Get("EventTree");
	TGlobalData* gData = NULL;
	tr->SetBranchAddress("Event", &gData);
	TH1I pulse("pulse", "Pulse ("+channel+");Time(ns);ADC", 100, 0., 100.);
	pulse.SetStats(0);
	pulse.GetYaxis()->SetRangeUser(0.,4096.);

	std::string s;
	std::vector<TPulseIsland*> p;
	TPulseIsland* tp = NULL;
	std::vector<int> tv;
	int n = tr->GetEntries();

	for (int i = 0; i < n; i++) {
		tr->GetEntry(i);
		p = gData->fPulseIslandToChannelMap.at(channel.Data());
		for (unsigned int j = 0; j < p.size(); j++) {
			tp = p.at(j);
			tv = tp->GetSamples();
			std::cout << "Time: " << tp->GetTimeStamp();
			std::cout << "\tTicks: " << tp->GetClockTickInNs();
			std::cout << "\tBank: " << tp->GetBankName();
			std::cout << std::endl;
			for (int k = 0; k < 100; k++)
				pulse.SetBinContent(k + 1, tv[k]);
			pulse.Draw();
			can->Update();
			std::cout << std::endl;
			usleep(50000);
		}
	}

}
