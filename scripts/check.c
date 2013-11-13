#include <iostream>
#include <ctime>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TString.h"
#include "TCanvas.h"
#include "TPaveText.h"

#include "rootlibs/TGlobalData.hh"
#include "rootlibs/TPulseIsland.hh"

void check(TString fname, TString channel, int tpauseus = 100000) {
	TCanvas* can = new TCanvas();
	TFile f(fname);
	TTree* tr = (TTree*) f.Get("EventTree");
	TGlobalData* gData = NULL;
	tr->SetBranchAddress("Event", &gData);


	std::string s;
	std::vector<TPulseIsland*> p;
	TPulseIsland* tp = NULL;
	std::vector<int> tv;
	int n = tr->GetEntries();

	TPaveText info;
	info.SetX1NDC(0.7);
	info.SetX2NDC(0.9);
	info.SetY1NDC(0.7);
	info.SetY2NDC(0.9);
	std::stringstream infotxt;
	
	int nsamps = 0;
	TH1I pulse;
	
	printf("There are %d entries to check...\n",n);
	
	for (int i = 0; i < n; i++) {
		printf("Now on entry %d...\n",i);
		tr->GetEntry(i);
		p = gData->fPulseIslandToChannelMap.at(channel.Data());
		printf("We see %u islands in channel %s.\n",p.size(),channel.Data());
		for (unsigned int j = 0; j < p.size(); j++) {
			tp = p.at(j);
			tv = tp->GetSamples();
			nsamps = tv.size();
			pulse = TH1I("pulse", "Pulse ("+channel+");Time(ns);ADC", nsamps, 0., (double)nsamps);
			pulse.SetStats(0);
			pulse.GetYaxis()->SetRangeUser(0.,4096.);
			info.Clear();
			infotxt << "Event: " << i;
			info.AddText(infotxt.str().c_str());
			infotxt.str("");
			infotxt << "Hit: " << j;
			info.AddText(infotxt.str().c_str());
			infotxt.str("");
			infotxt << "Time in Ticks: " << tp->GetTimeStamp();
			info.AddText(infotxt.str().c_str());
			infotxt.str("");
			infotxt << "Tick Length: " << tp->GetClockTickInNs();
			info.AddText(infotxt.str().c_str());
			infotxt.str("");
			for (int k = 0; k < nsamps; k++)
				pulse.SetBinContent(k + 1, tv[k]);
			pulse.Draw();
			info.Draw("SAME");
			can->Update();
			usleep(tpauseus);
			gDirectory->Delete("pulse");
		}
	}

}
