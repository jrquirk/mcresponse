{
  gROOT->ProcessLine(".include rootlibs");
  gROOT->ProcessLine(".L rootlibs/TTrackerHit.cpp+");
  gROOT->ProcessLine(".L rootlibs/TEvent.cpp+");
  gROOT->ProcessLine(".L rootlibs/TPulseIsland.cpp+");
  gROOT->ProcessLine(".L rootlibs/TGlobalData.cpp+");
  gROOT->ProcessLine(".L scripts/check.c+");
}
